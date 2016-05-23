/*
 * Window.hpp
 *
 *  Created on: 1 sept. 2014
 *      Author: Gregory_Compte
 */

#ifndef WINDOW_HPP_INC
#define WINDOW_HPP_INC

#include <string>
#include "Engine.hpp"
#include "Graphic/WindowEventType.hpp"
#include "Math/Point2.hpp"
#include <memory>

#if defined(CROISSANT_LINUX)
#  include <X11/X.h>
#  include <X11/Xlib.h>
#elif defined(CROISSANT_WINDOWS)
#  include <Windows.h>
#endif

namespace Croissant
{
	namespace Graphic
	{
		class WindowEvent;
		class WindowInternal;

		class ENGINE_API Window
		{
		public:
#if defined(CROISSANT_WINDOWS)
			using SystemHandle = HWND;
#elif defined(CROISSANT_LINUX)

			class InternalHandle
			{
			public:
				::Display*	m_display;
				::Window 	m_window;
			};

			using SystemHandle = InternalHandle;
#else
#error "système non supporté pour les fenêtres"
#endif

			Window(uint32_t width, uint32_t height, const std::string& title);
			~Window();

			std::unique_ptr<WindowEvent const>	PeekEvent();
			SystemHandle const&					GetSystemHandle() const;
			void								Close();
			void								Open();
			void								SetTitle(std::string const& title);
			void								SetPosition(Math::Point2 const& position);
			Math::Point2 const&					GetPosition() const;
			uint32_t							Width() const;
			uint32_t							Height() const;
			void								CenterCursor();

		private:

			SystemHandle		m_windowHandle;
			std::string const&	m_title;
			Math::Point2		m_position;
			uint32_t			m_width;
			uint32_t			m_height;
			Math::Point2		m_mouseLastPosition;
			std::unique_ptr<WindowInternal>	m_win;
#if defined(CROISSANT_WINDOWS)
			char const*			m_className;
#endif
		};
	}
}



#endif /* WINDOW_HPP_INC */
