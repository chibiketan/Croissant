#include "Debug/MemoryManager.hpp"
#include "Core/Application.hpp"
#include "Module/ServiceProvider.hpp"
#include "Module/ServiceProviderInternal.hpp"
#include "Core/LogManager.hpp"


namespace Croissant
{
	namespace Core
	{
//		class ProcessUserEvent : public Croissant::Event::IEventListener
//		{
//		public:
//			ProcessUserEvent(Application* application)
//				: m_application(application)
//			{
//				assert(nullptr != application);
//			}
//
//			virtual ~ProcessUserEvent()
//			{
//			}
//
//			virtual void ProcessEvent( Croissant::Event::Event* evt )
//			{
//				auto uevt = reinterpret_cast<Croissant::Event::UserEvent*>(evt);
//
//				switch (uevt->Kind())
//				{
//				case Croissant::Event::UserEvent::UserQuit:
//					m_application->Quit();
//					break;
//				}
//			}
//
//		private:
//
//			Application* m_application;
//		};

//		class FrameProvider : public IFrameProvider
//		{
//		public:
//			FrameProvider(Frame& frame)
//				: m_frame(frame)
//			{
//			}
//
//			virtual ~FrameProvider() = default;
//
//			virtual const Frame& CurrentFrame() const
//			{
//				return m_frame;
//			}
//
//		private:
//			Frame& m_frame;
//		};

		class Application::Impl final
		{
		public:
			Impl(Application const&) = delete;
			Impl& operator=(Impl const&) = delete;
			Impl(Application&&) = delete;
			Impl& operator=(Impl&&) = delete;

			Impl(void);
			~Impl(void);
			//Croissant::Event::EventManager* EventManager() const;
			//void SetEventManager(std::unique_ptr<Croissant::Event::EventManager> eventManager);
			//Croissant::Module::ModuleManager* ModuleManager() const;
			//Croissant::Scene::SceneManager* SceneManager() const;
			//void SetSceneManager(std::unique_ptr<Croissant::Scene::SceneManager> sceneManager);
			LogManager& GetLogManager();
			bool Init();
			void Shutdown();
			int Run();
			void Quit();

		private:
			bool m_init;
			bool m_run;
			/*Croissant::Module::ServiceProviderInternal m_serviceProviderInternal;*/
			LogManager m_logManager;
			/*std::unique_ptr<Croissant::Module::ServiceProvider> m_serviceProvider;*/
			//std::unique_ptr<Croissant::Event::EventManager> m_eventManager;
			//std::unique_ptr<Croissant::Module::ModuleManager> m_moduleManager;
			//std::unique_ptr<Croissant::Scene::SceneManager> m_sceneManager;
			//Croissant::Module::ServiceProvider m_serviceProvider;

			//Frame m_currentFrame;
		};


		// ----------------------------------- Application implémentation

		Application::Impl::Impl()
			: m_init { false }, m_run { false }/*, m_serviceProviderInternal {}*/
			/*, m_eventManager(nullptr), m_moduleManager(nullptr)*//*, m_serviceProvider()
			, m_currentFrame {}*/
			, m_logManager { "CroissantApplication" }/*, m_serviceProvider { nullptr }*/
		{
		}

		Application::Impl::~Impl()
		{
		}

		LogManager& Application::Impl::GetLogManager()
		{
			return m_logManager;
		}

//		Croissant::Event::EventManager* Application::Impl::EventManager() const
//		{
//			return m_eventManager.get();
//		}

//		void Application::Impl::SetEventManager(std::unique_ptr<Croissant::Event::EventManager> eventManager)
//		{
//			auto old = m_eventManager.get();
//
//			if (nullptr != old)
//			{
//				m_eventManager.release();
//				CROISSANT_DELETE(old);
//			}
//
//			m_eventManager = std::move(eventManager);
//		}

//		Croissant::Module::ModuleManager* Application::Impl::ModuleManager() const
//		{
//			return m_moduleManager.get();
//		}

//		void Application::Impl::SetModuleManager(std::unique_ptr<Croissant::Module::ModuleManager> moduleManager)
//		{
//			auto old = m_moduleManager.get();
//
//			if (nullptr != old)
//			{
//				m_moduleManager.release();
//				CROISSANT_DELETE(old);
//			}
//
//			m_moduleManager = std::move(moduleManager);
//		}

//		Croissant::Scene::SceneManager* Application::Impl::SceneManager() const
//		{
//			return m_sceneManager.get();
//		}

//		void Application::Impl::SetSceneManager(std::unique_ptr<Croissant::Scene::SceneManager> sceneManager)
//		{
//			m_sceneManager = std::move(sceneManager);
//		}

		bool Application::Impl::Init()
		{
			if (m_init)
			{
				return true;
			}

			m_logManager.Init();
//			if (nullptr == m_eventManager.get())
//			{
//				SetEventManager(std::unique_ptr<Croissant::Event::EventManager>(CROISSANT_NEW Croissant::Event::EventManager()));
//			}

//			if (nullptr == m_moduleManager.get())
//			{
//				SetModuleManager(std::unique_ptr<Croissant::Module::ModuleManager>(CROISSANT_NEW Croissant::Module::ModuleManager()));
//			}

//			if (nullptr == m_sceneManager.get())
//			{
//				SetSceneManager(std::unique_ptr<Croissant::Scene::SceneManager>(CROISSANT_NEW Croissant::Scene::SceneManager()));
//			}

//			if (!m_moduleManager->InitializeToZero(m_eventManager.get(), m_serviceProvider))
//			{
//				return false;
//			}

//			m_serviceProvider.Provide<Croissant::Event::IEventManagerImplementor>([this]() -> void* {
//				return CROISSANT_NEW Croissant::Event::EventManagerImplementor(*this->m_eventManager);
//			});

//			m_serviceProvider.Provide<IFrameProvider>([this]() -> void* {
//				return CROISSANT_NEW FrameProvider(this->m_currentFrame);
//			});


			m_init = true;
//			m_eventManager->ProcessEvents();
			m_logManager.Write("Démarrage de l'application");
			return true;
		}

		void Application::Impl::Shutdown()
		{
            assert(true == m_init);
			m_logManager.Write("Arrêt de l'application");
            m_logManager.Shutdown();
            //m_sceneManager->Shutdown();
//            CROISSANT_DELETE(m_sceneManager.release());
            //m_moduleManager->Shutdown();
//            CROISSANT_DELETE(m_moduleManager.release());
//            CROISSANT_DELETE(m_eventManager.release());
            m_init = false;
		}

		void Application::Impl::Quit()
		{
			m_run = false;
		}

		int Application::Impl::Run()
		{
//			ProcessUserEvent listener(application);

			//m_eventManager->RegisterListener("Game::Event", &listener);
//			m_eventManager->RegisterListener("Game::Event", [&listener](Croissant::Event::Event* evt) -> void {
//				listener.ProcessEvent(evt);
//			});
//			m_eventManager->RegisterListener("Frame::Start", [this](Croissant::Event::Event*) -> void {
//				this->m_currentFrame = Frame(this->m_currentFrame);
//			});
			m_run = true;
//			while (m_run)
//			{
//				m_eventManager->AddEvent(std::unique_ptr<Croissant::Event::FrameStartEvent>(CROISSANT_NEW Croissant::Event::FrameStartEvent()), Croissant::Event::EventPriority::START_FRAME);
//				m_eventManager->AddEvent(std::unique_ptr<Croissant::Event::FrameEndEvent>(CROISSANT_NEW Croissant::Event::FrameEndEvent()), Croissant::Event::EventPriority::END_FRAME);
//				m_eventManager->AddEvent(std::unique_ptr<Croissant::Event::ProcessUserInputEvent>(CROISSANT_NEW Croissant::Event::ProcessUserInputEvent()), Croissant::Event::EventPriority::USER_ACTION);
//				m_eventManager->AddEvent(std::unique_ptr<Croissant::Event::RenderEvent>(CROISSANT_NEW Croissant::Event::RenderEvent()), Croissant::Event::EventPriority::RENDER);
//				m_eventManager->ProcessEvents();
//			}

			return 0;
		}

		// ----------------------------------- Application implémentation

		Application::Application()
			: m_impl { new Application::Impl {} }
		{
		}

		Application::~Application()
		{
		}

//		Croissant::Event::EventManager* Application::EventManager() const
//		{
//			return m_eventManager.get();
//		}

//		void Application::SetEventManager(std::unique_ptr<Croissant::Event::EventManager> eventManager)
//		{
//			auto old = m_eventManager.get();
//
//			if (nullptr != old)
//			{
//				m_eventManager.release();
//				CROISSANT_DELETE(old);
//			}
//
//			m_eventManager = std::move(eventManager);
//		}

//		Croissant::Module::ModuleManager* Application::ModuleManager() const
//		{
//			return m_moduleManager.get();
//		}

//		void Application::SetModuleManager(std::unique_ptr<Croissant::Module::ModuleManager> moduleManager)
//		{
//			auto old = m_moduleManager.get();
//
//			if (nullptr != old)
//			{
//				m_moduleManager.release();
//				CROISSANT_DELETE(old);
//			}
//
//			m_moduleManager = std::move(moduleManager);
//		}

//		Croissant::Scene::SceneManager* Application::SceneManager() const
//		{
//			return m_sceneManager.get();
//		}

//		void Application::SetSceneManager(std::unique_ptr<Croissant::Scene::SceneManager> sceneManager)
//		{
//			m_sceneManager = std::move(sceneManager);
//		}

		LogManager& Application::GetLogManager()
		{
			return m_impl->GetLogManager();
		}

		bool Application::Init()
		{
			return m_impl->Init();
		}

		void Application::Shutdown()
		{
			m_impl->Shutdown();
		}

		void Application::Quit()
		{
			m_impl->Quit();
		}

		int Application::Run()
		{
			return m_impl->Run();
		}

	}
}
