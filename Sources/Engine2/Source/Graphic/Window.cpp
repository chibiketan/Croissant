/*
 * Window.cpp
 *
 *  Created on: 1 sept. 2014
 *      Author: Gregory_Compte
 */

#include <string>
#include "Debug/MemoryManager.hpp"
#include "Graphic/Window.hpp"
#include "Graphic/WindowEventNone.hpp"
#include "Graphic/WindowEventClose.hpp"
#include "Exception/CroissantException.hpp"

using WindowException = Croissant::Exception::CroissantException;
using string = std::string;

namespace Croissant
{
	namespace Graphic
	{
		namespace
		{
			//--------------------------------------------------------------------------------------
			// Called every time the application receives a message
			//--------------------------------------------------------------------------------------
			LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
			{
				//PAINTSTRUCT ps;
				//HDC hdc;

				switch( message )
				{
				//case WM_PAINT:
				//	hdc = BeginPaint( hWnd, &ps );
				//	EndPaint( hWnd, &ps );
				//	break;
				case WM_CLOSE:
					PostMessage(hWnd, message, wParam, lParam);
					break;
//					case WM_DESTROY:
//						PostQuitMessage( 0 );
//						break;

				default:
					return DefWindowProc( hWnd, message, wParam, lParam );
				}

				return 0;
			}

		}

		// --------------------------------------------- Window imp
		Window::Window(uint32_t width, uint32_t height, const std::string& title)
			: m_windowHandle { nullptr }, m_title { title }, m_width { width }, m_height{ height }
#if defined(CROISSANT_WINDOWS)
				, m_className { nullptr }
#endif
		{
			m_className = "TutorialWindowClassTest";
			// Register class
			WNDCLASSEX wcex;
			wcex.cbSize = sizeof( WNDCLASSEX );
			wcex.style = CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc = WndProc;
			//wcex.lpfnWndProc = nullptr;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = 0;
			//wcex.hInstance = hInstance;
			wcex.hInstance = nullptr;
			//wcex.hIcon = LoadIcon( hInstance, ( LPCTSTR )IDI_TUTORIAL1 );
			//wcex.hIcon = LoadIcon( nullptr, ( LPCTSTR )IDI_TUTORIAL1 );
			wcex.hIcon = nullptr;
			wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
			wcex.hbrBackground = reinterpret_cast<HBRUSH>( COLOR_WINDOW + 1 );
			wcex.lpszMenuName = NULL;
			wcex.lpszClassName = m_className;
			//wcex.hIconSm = LoadIcon( wcex.hInstance, ( LPCTSTR )IDI_TUTORIAL1 );
			wcex.hIconSm = nullptr;
			if( !RegisterClassEx( &wcex ) )
			{
				throw WindowException("Erreur lors de l'enregistrement de la classe pour la fenêtre");
			}

			// Create window
			//g_hInst = hInstance;
			RECT rc = { 0, 0, static_cast<int32_t>(m_width), static_cast<int32_t>(m_height) };
			HINSTANCE hinstance = nullptr;
			AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
			//m_hWnd = CreateWindow( L"TutorialWindowClass", L"Direct3D 11 Tutorial 2: Rendering a Triangle",
			//	WS_OVERLAPPEDWINDOW,
			//	CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
			//	NULL );
			m_windowHandle = CreateWindow( m_className, m_title.c_str(),
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hinstance,
				NULL );
			if( !m_windowHandle )
			{
				throw WindowException("Erreur lors de la création de la fenêtre");
			}
		}

		Window::~Window()
		{
			Close();
			DestroyWindow(m_windowHandle);
			UnregisterClass(m_className, nullptr);
		}

		void Window::Close()
		{
			ShowWindow(m_windowHandle, 0);
		}

		void Window::Open()
		{
			ShowWindow(m_windowHandle, 1);
		}


		const Window::SystemHandle& Window::GetSystemHandle() const
		{
			return m_windowHandle;
		}

		void Window::SetTitle(string const& title) {
			SetWindowText(m_windowHandle, title.c_str());
		}

		std::unique_ptr<WindowEvent const> Window::PeekEvent()
		{
			auto hnd = GetSystemHandle();

			MSG msgTest;
			while (PeekMessage(&msgTest, hnd, 0, 0, PM_REMOVE))
			{
			    switch (msgTest.message)
			    {
//			    case WM_KEYDOWN:
//			        // key codes : http://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx
//			        switch (msgTest.wParam)
//			        {
//			        case 0x51:
//						// TODO send end app request
//						m_eventManager->AddEventAtUserInputLevel(std::unique_ptr<Croissant::Event::UserEvent>(CROISSANT_NEW Croissant::Event::UserEvent(Croissant::Event::UserEvent::UserQuit)));
//			            break;
//			        }
//			        break;
			    case WM_CLOSE:
			    case WM_DESTROY:
			    case WM_QUIT:
					return std::unique_ptr<WindowEvent const>(CROISSANT_NEW WindowEventClose());
			    	break;
//			        // TODO send end app request
//					m_eventManager->AddEventAtUserInputLevel(std::unique_ptr<Croissant::Event::UserEvent>(CROISSANT_NEW Croissant::Event::UserEvent(Croissant::Event::UserEvent::UserQuit)));
//			        break;
			    default:
			        TranslateMessage( &msgTest );
			        DispatchMessage( &msgTest );
			        break;
			        //return WindowEvent();
			    }
			}

			return std::unique_ptr<WindowEvent const>(CROISSANT_NEW WindowEventNone());
		}
	}
}
