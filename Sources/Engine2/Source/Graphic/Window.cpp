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
#if defined(CROISSANT_WINDOWS)
#  include "../../Source/Graphic/WindowInternal.Windows.inl"
#elif defined(CROISSANT_LINUX)
#  include "../../Source/Graphic/WindowInternal.Linux.cpp"
#endif

using WindowException = Croissant::Exception::CroissantException;
using string = std::string;



namespace Croissant
{
	namespace Graphic
	{

		// --------------------------------------------- Window imp
		Window::Window(uint32_t width, uint32_t height, const std::string& title)
			: m_title{ title }, m_position{ 0, 0 }, m_width{ width }, m_height{ height }, m_mouseLastPosition{ 0, 0 }
			  , m_win{ std::make_unique<WindowInternal>() }
		{
			m_win->SetTitle(title);
			m_win->Position(m_position);
			m_win->Resize(m_width, m_height);
		}

		Window::~Window()
		{
			Close();
		}

		void Window::Close()
		{
            m_win->Hide();
		}

		void Window::Open()
		{
            m_win->Show();
		}


		Window::SystemHandle const& Window::GetSystemHandle() const
		{
			return m_win->GetSystemHandle();
		}

		void Window::SetTitle(string const& title) {
			m_win->SetTitle(title);
		}

		void Window::SetPosition(Math::Point2 const& position)
		{
			m_position = position;
            m_win->Position(position);
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
			int x, y;
			x = m_position.X() + (m_width / 2.0f);
			y = m_position.Y() + (m_height / 2.0f);
//			ClientToScreen(m_windowHandle, &pt);
			//SetCursorPos(m_position.X() + 50, m_position.Y() + 50);
			m_mouseLastPosition.X(x);
			m_mouseLastPosition.Y(y);
			m_win->SetCursorPosition(x, y);
		}

		std::unique_ptr<WindowEvent const> Window::PeekEvent()
		{
			return m_win->PeekEvent();
		}
	}
}
