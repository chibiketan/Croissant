#include "Debug/MemoryManager.hpp"
#include "Graphic/Window.hpp"
#include "Graphic/WindowMouseMoveEvent.hpp"
#include "Graphic/WindowEventType.hpp"
#include "Graphic/WindowEventKey.hpp"
#include <windowsx.h>

namespace Croissant
{
	namespace Graphic
	{
		namespace
		{
			//--------------------------------------------------------------------------------------
			// Called every time the application receives a message
			//--------------------------------------------------------------------------------------
			LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
			{
				//PAINTSTRUCT ps;
				//HDC hdc;

				switch (message)
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
					return DefWindowProc(hWnd, message, wParam, lParam);
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
			///Cr�e un �v�nement WindowMouseMoveEvent et modifie la position fourni en param�tre
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
					// si pas de mouvement, on ne g�n�re pas d�v�nement
					return nullptr;
				}

				position.X(xPos);
				position.Y(yPos);

				return std::unique_ptr<WindowEvent const>(CROISSANT_NEW WindowMouseMoveEvent(deltaX, deltaY));
			}
		}


		class WindowInternal
		{
		public:
			WindowInternal(Math::Point2 position, uint32_t width, uint32_t height);
			~WindowInternal();
			Window::SystemHandle const&    GetSystemHandle() const;
			void        Resize(uint32_t width, uint32_t height);
			void        Position(Math::Point2 const& position);
			void        Show() const;
			void        Hide() const;
			void        SetTitle(std::string const& title);
			void        SetCursorPosition(int x, int y);
			std::unique_ptr<WindowEvent const> PeekEvent();

		private:
			Window::SystemHandle    m_handle;
			char const*				m_className;
			Math::Point2			m_position;
			uint32_t				m_width;
			uint32_t				m_height;
			Math::Point2			m_mouseLastPosition;
		};

		inline WindowInternal::WindowInternal(Math::Point2 position, uint32_t width, uint32_t height)
			: m_handle{}, m_className{ nullptr }, m_position{ position }
			, m_width{ width }, m_height{ height }, m_mouseLastPosition{ 0, 0 }
		{
			m_className = "TutorialWindowClassTest";
			// Register class
			WNDCLASSEX wcex;
			wcex.cbSize = sizeof(WNDCLASSEX);
			wcex.style = CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc = WndProc;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = 0;
			wcex.hInstance = nullptr;
			wcex.hIcon = nullptr;
			wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
			wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
			wcex.lpszMenuName = NULL;
			wcex.lpszClassName = m_className;
			wcex.hIconSm = nullptr;
			if (!RegisterClassEx(&wcex))
			{
				throw Exception::CroissantException("Erreur lors de l'enregistrement de la classe pour la fen�tre");
			}

			// Create window
			RECT rc = { static_cast<int32_t>(position.X()), static_cast<int32_t>(position.Y())
               , static_cast<int32_t>(width + position.X()), static_cast<int32_t>(height + position.Y()) };
			HINSTANCE hinstance = nullptr;
			AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
			m_handle = CreateWindow(m_className, "toto",
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hinstance,
				NULL);
			if (!m_handle)
			{
				throw Exception::CroissantException("Erreur lors de la cr�ation de la fen�tre");
			}

			UpdateWindowsPlacement(m_handle, m_position, m_width, m_height);
		}

		inline WindowInternal::~WindowInternal()
		{
			DestroyWindow(m_handle);
			UnregisterClass(m_className, nullptr);
		}

		inline Window::SystemHandle const& WindowInternal::GetSystemHandle() const
		{
			return m_handle;
		}

		inline void WindowInternal::Resize(uint32_t width, uint32_t height)
		{
			m_width = width;
			m_height = height;
			UpdateWindowsPlacement(m_handle, m_position, m_width, m_height);
		}

		inline void WindowInternal::Position(Math::Point2 const& position)
		{
			m_position = position;
			UpdateWindowsPlacement(m_handle, m_position, m_width, m_height);
		}

		inline void WindowInternal::Show() const
		{
			ShowWindow(m_handle, 1);
		}

		inline void WindowInternal::Hide() const
		{
			ShowWindow(m_handle, 0);
		}

		inline void WindowInternal::SetTitle(std::string const& title)
		{
			SetWindowText(m_handle, title.c_str());
		}

		inline void WindowInternal::SetCursorPosition(int x, int y)
		{
			m_mouseLastPosition.X(x);
			m_mouseLastPosition.Y(y);
			SetCursorPos(x, y);
		}

		inline std::unique_ptr<WindowEvent const> WindowInternal::PeekEvent()
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
					return std::make_unique<WindowEventClose const>();
					break;
					//			        // TODO send end app request
					//					m_eventManager->AddEventAtUserInputLevel(std::unique_ptr<Croissant::Event::UserEvent>(CROISSANT_NEW Croissant::Event::UserEvent(Croissant::Event::UserEvent::UserQuit)));
					//			        break;
				default:
					TranslateMessage(&msgTest);
					DispatchMessage(&msgTest);
					break;
					//return WindowEvent();
				}
			}

			return std::make_unique<WindowEventNone const>();
		}
	}
}