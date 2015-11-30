#include "OpenGL/Window.hpp"
#include <iostream>
#include "core/DebugMemoryManager.hpp"

namespace Croissant
{
	namespace Module
	{
		namespace OpenGL
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

			class Window::Pimpl
			{
			public:
				Pimpl();

				bool Init();
				void Cleanup();
				const HWND& GetHandle() const;
			private:

				volatile int m_initCount;
				LPCSTR m_className;
				HWND m_hWnd;
			};


			// ------------------------------- class Window::Pimpl

			Window::Pimpl::Pimpl()
				: m_initCount(0), m_className(nullptr), m_hWnd(nullptr)
			{
			}

			bool Window::Pimpl::Init()
			{
				auto isInit = (m_initCount++ != 0);

				if (isInit)
				{
					return true;
				}

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
				wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
				wcex.lpszMenuName = NULL;
				wcex.lpszClassName = m_className;
				//wcex.hIconSm = LoadIcon( wcex.hInstance, ( LPCTSTR )IDI_TUTORIAL1 );
				wcex.hIconSm = nullptr;
				if( !RegisterClassEx( &wcex ) )
				{
					return false;
				}

				// Create window
				//g_hInst = hInstance;
				RECT rc = { 0, 0, 640, 480 };
				HINSTANCE hinstance = nullptr;
				AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
				//m_hWnd = CreateWindow( L"TutorialWindowClass", L"Direct3D 11 Tutorial 2: Rendering a Triangle",
				//	WS_OVERLAPPEDWINDOW,
				//	CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
				//	NULL );
				m_hWnd = CreateWindow( m_className, "Classe Window",
					WS_OVERLAPPEDWINDOW,
					CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hinstance,
					NULL );
				if( !m_hWnd )
				{
					return false;
				}

				ShowWindow(m_hWnd, 1);

				return true;
			}

			void Window::Pimpl::Cleanup()
			{
				bool needClean = (--m_initCount == 0);

				if (!needClean)
				{
					return;
				}

				ShowWindow(m_hWnd, 0);
				DestroyWindow(m_hWnd);
				UnregisterClass(m_className, nullptr);
			}

			const HWND& Window::Pimpl::GetHandle() const
			{
				return m_hWnd;
			}

			// ---------------------------------------- class Window
			Window::Window() : m_pimpl(CROISSANT_NEW Pimpl())
			{

			}

			Window::~Window()
			{
				m_pimpl->Cleanup();
			}

			bool Window::Init()
			{
				return m_pimpl->Init();
			}

			void Window::Cleanup()
			{
				m_pimpl->Cleanup();
			}

			const HWND& Window::GetHandle() const
			{
				return m_pimpl->GetHandle();
			}

		}
	}
}
