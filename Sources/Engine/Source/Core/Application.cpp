#include "stdafx.hpp"
#include "Core/DebugMemoryManager.hpp"
#include "Core/Application.hpp"
#include "Event/EventManager.hpp"
#include "Event/IEventListener.hpp"
#include "Event/FrameEndEvent.hpp"
#include "Event/FrameStartEvent.hpp"
#include "Event/UserEvent.hpp"
#include "Event/ProcessUserInputEvent.hpp"
#include "Module/ModuleManager.hpp"
#include "Module/ServiceProvider.hpp"
#include "Event/IEventManagerImplementor.hpp"
#include "Event/EventManagerImplementor.hpp"
#include "Event/RenderEvent.hpp"
#include "Scene/SceneManager.hpp"
#include "Core/Frame.hpp"
#include "Core/IFrameProvider.hpp"

namespace Croissant
{
	namespace Core
	{
		template<>
		class ENGINE_API Pimpl<Application>::Implementation
		{
		public:
			inline Implementation(Application* app);
			inline ~Implementation();

			inline Croissant::Event::EventManager* EventManager() const;
			inline void SetEventManager(std::unique_ptr<Croissant::Event::EventManager> eventManager);
			inline Croissant::Module::ModuleManager* ModuleManager() const;
			inline void SetModuleManager(std::unique_ptr<Croissant::Module::ModuleManager> moduleManager);
			inline Croissant::Scene::SceneManager* SceneManager() const;
			inline void SetSceneManager(std::unique_ptr<Croissant::Scene::SceneManager> sceneManager);
			inline bool Init();
			inline void Shutdown();
			inline int Run(Application* application);
			inline void Quit();

		private:
			bool m_init;
			std::unique_ptr<Croissant::Event::EventManager> m_eventManager;
			std::unique_ptr<Croissant::Module::ModuleManager> m_moduleManager;
			std::unique_ptr<Croissant::Scene::SceneManager> m_sceneManager;
			bool m_run;
			Croissant::Module::ServiceProvider m_serviceProvider;

			Application* m_app;
			Frame m_currentFrame;
		};

		typedef Pimpl<Application>::Implementation ApplicationImpl;

		class ProcessUserEvent : public Croissant::Event::IEventListener
		{
		public:
			ProcessUserEvent(Application* application)
				: m_application(application)
			{
				assert(nullptr != application);
			}

			virtual ~ProcessUserEvent()
			{
			}

			virtual void ProcessEvent( Croissant::Event::Event* evt )
			{
				auto uevt = reinterpret_cast<Croissant::Event::UserEvent*>(evt);

				switch (uevt->Kind())
				{
				case Croissant::Event::UserEvent::UserQuit:
					m_application->Quit();
					break;
				}
			}

		private:

			Application* m_application;
		};

		class FrameProvider : public IFrameProvider
		{
		public:
			FrameProvider(Frame& frame)
				: m_frame(frame)
			{
			}

			virtual ~FrameProvider() = default;

			virtual const Frame& CurrentFrame() const
			{
				return m_frame;
			}

		private:
			Frame& m_frame;
		};

		// ----------------------------------- Application::Pimpl impl�mentation

		ApplicationImpl::Implementation(Application* app)
			: m_init(false), m_eventManager(nullptr), m_moduleManager(nullptr), m_run(false), m_serviceProvider(), m_app(app)
			, m_currentFrame {}
		{
		}

		ApplicationImpl::~Implementation()
		{
		}

		Croissant::Event::EventManager* ApplicationImpl::EventManager() const
		{
			return m_eventManager.get();
		}

		void ApplicationImpl::SetEventManager(std::unique_ptr<Croissant::Event::EventManager> eventManager)
		{
			auto old = m_eventManager.get();

			if (nullptr != old)
			{
				m_eventManager.release();
				CROISSANT_DELETE(old);
			}

			m_eventManager = std::move(eventManager);
		}

		Croissant::Module::ModuleManager* ApplicationImpl::ModuleManager() const
		{
			return m_moduleManager.get();
		}

		void ApplicationImpl::SetModuleManager(std::unique_ptr<Croissant::Module::ModuleManager> moduleManager)
		{
			auto old = m_moduleManager.get();

			if (nullptr != old)
			{
				m_moduleManager.release();
				CROISSANT_DELETE(old);
			}

			m_moduleManager = std::move(moduleManager);
		}

		Croissant::Scene::SceneManager* ApplicationImpl::SceneManager() const
		{
			return m_sceneManager.get();
		}

		void ApplicationImpl::SetSceneManager(std::unique_ptr<Croissant::Scene::SceneManager> sceneManager)
		{
			m_sceneManager = std::move(sceneManager);
		}

		bool ApplicationImpl::Init()
		{
			if (m_init)
			{
				return true;
			}

			if (nullptr == m_eventManager.get())
			{
				SetEventManager(std::unique_ptr<Croissant::Event::EventManager>(CROISSANT_NEW Croissant::Event::EventManager()));
			}

			if (nullptr == m_moduleManager.get())
			{
				SetModuleManager(std::unique_ptr<Croissant::Module::ModuleManager>(CROISSANT_NEW Croissant::Module::ModuleManager()));
			}

			if (nullptr == m_sceneManager.get())
			{
				SetSceneManager(std::unique_ptr<Croissant::Scene::SceneManager>(CROISSANT_NEW Croissant::Scene::SceneManager()));
			}

			if (!m_moduleManager->Init(m_eventManager.get(), m_serviceProvider))
			{
				return false;
			}

			m_serviceProvider.Provide<Croissant::Event::IEventManagerImplementor>([this]() -> void* {
				return CROISSANT_NEW Croissant::Event::EventManagerImplementor(*this->m_eventManager);
			});

			m_serviceProvider.Provide<IFrameProvider>([this]() -> void* {
				return CROISSANT_NEW FrameProvider(this->m_currentFrame);
			});


			m_init = true;
			m_eventManager->ProcessEvents();
			return true;
		}

		void ApplicationImpl::Shutdown()
		{
            assert(true == m_init);

            //m_sceneManager->Shutdown();
            CROISSANT_DELETE(m_sceneManager.release());
            //m_moduleManager->Shutdown();
            CROISSANT_DELETE(m_moduleManager.release());
            CROISSANT_DELETE(m_eventManager.release());
            m_init = false;
		}

		void ApplicationImpl::Quit()
		{
			m_run = false;
		}

		int ApplicationImpl::Run(Application* application)
		{
			ProcessUserEvent listener(application);

			//m_eventManager->RegisterListener("Game::Event", &listener);
			m_eventManager->RegisterListener("Game::Event", [&listener](Croissant::Event::Event* evt) -> void {
				listener.ProcessEvent(evt);
			});
			m_eventManager->RegisterListener("Frame::Start", [this](Croissant::Event::Event*) -> void {
				this->m_currentFrame = Frame(this->m_currentFrame);
			});
			m_run = true;
			while (m_run)
			{
				m_eventManager->AddEvent(std::unique_ptr<Croissant::Event::FrameStartEvent>(CROISSANT_NEW Croissant::Event::FrameStartEvent()), Croissant::Event::EventPriority::START_FRAME);
				m_eventManager->AddEvent(std::unique_ptr<Croissant::Event::FrameEndEvent>(CROISSANT_NEW Croissant::Event::FrameEndEvent()), Croissant::Event::EventPriority::END_FRAME);
				m_eventManager->AddEvent(std::unique_ptr<Croissant::Event::ProcessUserInputEvent>(CROISSANT_NEW Croissant::Event::ProcessUserInputEvent()), Croissant::Event::EventPriority::USER_ACTION);
				m_eventManager->AddEvent(std::unique_ptr<Croissant::Event::RenderEvent>(CROISSANT_NEW Croissant::Event::RenderEvent()), Croissant::Event::EventPriority::RENDER);
				m_eventManager->ProcessEvents();
			}

			return 0;
		}

		// ----------------------------------- Application impl�mentation

		Application::Application(void)
			: Croissant::Core::Pimpl<Application>::Detail()
		{
		}


		Application::~Application(void)
		{
		}

		bool Application::Init()
		{
			Pimpl<Application>::Implementation& imp = **this;

			return imp.Init();
		}

        void Application::Shutdown()
        {
			Pimpl<Application>::Implementation& imp = **this;

            imp.Shutdown();
        }

		void Application::SetEventManager( std::unique_ptr<Croissant::Event::EventManager> eventManager )
		{
			Pimpl<Application>::Implementation& imp = **this;

			imp.SetEventManager(std::move(eventManager));
		}

		void Application::SetSceneManager(std::unique_ptr<Croissant::Scene::SceneManager> sceneManager)
		{
			Pimpl<Application>::Implementation& imp = **this;

			imp.SetSceneManager(std::move(sceneManager));
		}

		Croissant::Scene::SceneManager* Application::SceneManager() const
		{
			const Pimpl<Application>::Implementation& imp = **this;

			return imp.SceneManager();
		}

		int Application::Run()
		{
			Pimpl<Application>::Implementation& imp = **this;

			return imp.Run(this);
		}

		void Application::Quit()
		{
			Pimpl<Application>::Implementation& imp = **this;

			imp.Quit();
		}

	}
}
