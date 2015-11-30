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

		class ENGINE_API Window
		{
		public:
#if defined(CROISSANT_WINDOWS)
			using SystemHandle = HWND;
#elif defined(CROISSANT_LINUX)
			using SystemHandle = ::Window
#else
#error "système non supporté pour les fenêtres"
#endif

			Window(uint32_t width, uint32_t height, const std::string& title);
			~Window();

			std::unique_ptr<WindowEvent const> PeekEvent();
			const SystemHandle& GetSystemHandle() const;
			void Close();
			void Open();
			void SetTitle(std::string const& title);

		private:

			SystemHandle m_windowHandle;
			std::string const& m_title;
			uint32_t m_width;
			uint32_t m_height;
#if defined(CROISSANT_WINDOWS)
			char const* m_className;
#elif defined(CROISSANT_LINUX)
			Display* m_display;

#endif
		};
	}
}



#endif /* WINDOW_HPP_INC */
