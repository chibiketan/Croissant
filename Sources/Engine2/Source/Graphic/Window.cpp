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
#include <Graphic/WindowEventKey.hpp>
#include <Graphic/WindowMouseMoveEvent.hpp>
#include <windowsx.h>

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

			std::unique_ptr<WindowEvent const> GenerateKeyEvent(MSG const& message, bool isRelease)
			{
				// https://msdn.microsoft.com/en-us/library/windows/desktop/ms646280%28v=vs.85%29.aspx?f=255&MSPPError=-2147217396
				auto type = (isRelease ? WindowEventKeyType::Release : WindowEventKeyType::Press);

				if (!isRelease)
				{
					// dans le cas KeyPress, ne prendre en compte que le changement de touche
					auto oldState = message.lParam & 0b01000000'00000000'00000000'00000000;

					if (oldState != 0)
					{
						return nullptr;
					}
				}

				switch (message.wParam)
				{
				case VK_UP:
					return std::unique_ptr<WindowEvent const>(CROISSANT_NEW WindowEventKey(type, WindowKey::Up));
				case VK_DOWN:
					return std::unique_ptr<WindowEvent const>(CROISSANT_NEW WindowEventKey(type, WindowKey::Down));
				case VK_LEFT:
					return std::unique_ptr<WindowEvent const>(CROISSANT_NEW WindowEventKey(type, WindowKey::Left));
				case VK_RIGHT:
					return std::unique_ptr<WindowEvent const>(CROISSANT_NEW WindowEventKey(type, WindowKey::Right));
				case VK_PRIOR:
					return std::unique_ptr<WindowEvent const>(CROISSANT_NEW WindowEventKey(type, WindowKey::PageUp));
				case VK_NEXT:
					return std::unique_ptr<WindowEvent const>(CROISSANT_NEW WindowEventKey(type, WindowKey::PageDown));
				default:
					break;
				}

				return nullptr;
			}

			void UpdateWindowsPlacement(HWND handle, Math::Point2 const& position, uint32_t width, uint32_t height)
			{
				UINT flags = 0;
				SetWindowPos(handle, HWND_NOTOPMOST, position.X(), position.Y(), width, height, flags);
			}

			///<summary>
			///Crée un évènement WindowMouseMoveEvent et modifie la position fourni en paramètre
			///</summary>
			std::unique_ptr<const WindowEvent> GenerateMouseMoveEvent(MSG& message, Math::Point2& position)
			{
				int32_t xPos = GET_X_LPARAM(message.lParam); 
				int32_t yPos = GET_Y_LPARAM(message.lParam);

				POINT t;
				t.x = xPos;
				t.y = yPos;
				ClientToScreen(message.hwnd, &t);

				auto deltaX = position.X() - t.x;
				auto deltaY = position.Y() - t.y;

				if (0 == deltaX && 0 == deltaY)
				{
					// si pas de mouvement, on ne génère pas dévènement
					return nullptr;
				}

				position.X(xPos);
				position.Y(yPos);

				return std::unique_ptr<WindowEvent const>(CROISSANT_NEW WindowMouseMoveEvent(deltaX, deltaY));
			}

		}

		// --------------------------------------------- Window imp
		Window::Window(uint32_t width, uint32_t height, const std::string& title)
			: m_windowHandle{ nullptr }, m_title{ title }, m_position{ 0, 0 }, m_width{ width }, m_height{ height }, m_mouseLastPosition{ 0, 0 }
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
			UpdateWindowsPlacement(m_windowHandle, m_position, m_width, m_height);
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

		void Window::SetPosition(Math::Point2 const& position)
		{
			m_position = position;
			UpdateWindowsPlacement(m_windowHandle, m_position, m_width, m_height);
		}

		Math::Point2 const& Window::GetPosition() const
		{
			return m_position;
		}

		uint32_t Window::Width() const
		{
			return m_width;
		}

		uint32_t Window::Height() const
		{
			return m_height;
		}

		void Window::CenterCursor()
		{
			POINT pt;
			pt.x = m_position.X() + (m_width / 2.0f);
			pt.y = m_position.Y() + (m_height / 2.0f);
//			ClientToScreen(m_windowHandle, &pt);
			//SetCursorPos(m_position.X() + 50, m_position.Y() + 50);
			m_mouseLastPosition.X(pt.x);
			m_mouseLastPosition.Y(pt.y);
			SetCursorPos(pt.x, pt.y);
		}

		std::unique_ptr<WindowEvent const> Window::PeekEvent()
		{
			auto hnd = GetSystemHandle();

			MSG msgTest;
			std::unique_ptr<WindowEvent const> event { nullptr };

			while (PeekMessage(&msgTest, hnd, 0, 0, PM_REMOVE))
			{
			    switch (msgTest.message)
			    {
				case WM_KEYDOWN:
					event = GenerateKeyEvent(msgTest, false);
					if (nullptr != event)
					{
						return event;
					}

					TranslateMessage(&msgTest);
					DispatchMessage(&msgTest);
					break;
				case WM_KEYUP:
					event = GenerateKeyEvent(msgTest, true);
					if (nullptr != event)
					{
						return event;
					}

					break;
				case WM_MOUSEMOVE:
					event = GenerateMouseMoveEvent(msgTest, m_mouseLastPosition);
					if (nullptr != event)
					{
						return event;
					}

					break;
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

			return std::make_unique<WindowEventNone const>();
			//return std::unique_ptr<WindowEvent const>(CROISSANT_NEW WindowEventNone());
		}
	}
}
