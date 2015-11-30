#include "Core/DebugMemoryManager.hpp"
#include "OpenGL/WindowsInputModule.hpp"

#include <assert.h>
#include "Event/Event.hpp"
#include "Event/UserEvent.hpp"
#include "OpenGL/Window.hpp"
#include "Module/ServiceProvider.hpp"
#include "Event/EventManagerImplementor.hpp"

namespace Croissant
{
	namespace Module
	{
		namespace OpenGL
		{
			const String WindowsInputModule::s_name = "OpenGL::WindowsInput";

			WindowsInputModule::WindowsInputModule(std::shared_ptr<Window> window)
				: m_window(window), m_eventManager(nullptr), m_processUserInputDelegate([this](Croissant::Event::Event* evt) -> void {
					ProcessEvent(evt);
				})
			{
				assert(nullptr != window.get());
			}

			WindowsInputModule::~WindowsInputModule()
			{
			}

			bool WindowsInputModule::Init( Croissant::Module::ServiceProvider& serviceProvider )
			{
				serviceProvider.Resolve<Croissant::Event::IEventManagerImplementor>(m_eventManager);
				m_eventManager->RegisterListener("Game::ProcessUserInput", m_processUserInputDelegate);

				return m_window->Init();
			}

			const String& WindowsInputModule::Name() const
			{
				return s_name;
			}

			void WindowsInputModule::ProcessEvent(Croissant::Event::Event*)
			{
				auto hnd = m_window->GetHandle();

				MSG msgTest;
				while (PeekMessage(&msgTest, hnd, 0, 0, PM_REMOVE))
				{
				    switch (msgTest.message)
				    {
				    case WM_KEYDOWN:
				        // key codes : http://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx
				        switch (msgTest.wParam)
				        {
				        case 0x51:
							// TODO send end app request
							m_eventManager->AddEventAtUserInputLevel(std::unique_ptr<Croissant::Event::UserEvent>(CROISSANT_NEW Croissant::Event::UserEvent(Croissant::Event::UserEvent::UserQuit)));
				            break;
				        }
				        break;
				    case WM_CLOSE:
				    case WM_DESTROY:
				    case WM_QUIT:
				        // TODO send end app request
						m_eventManager->AddEventAtUserInputLevel(std::unique_ptr<Croissant::Event::UserEvent>(CROISSANT_NEW Croissant::Event::UserEvent(Croissant::Event::UserEvent::UserQuit)));
				        break;
				    default:
				        TranslateMessage( &msgTest );
				        DispatchMessage( &msgTest );
				        break;
				    }
				}
			}

			void WindowsInputModule::Cleanup()
			{
				m_eventManager->UnregisterListener("Game::ProcessUserInput", m_processUserInputDelegate);
				m_window->Cleanup();
				// TODO: add cleanup proc
			}

		}
	}
}
