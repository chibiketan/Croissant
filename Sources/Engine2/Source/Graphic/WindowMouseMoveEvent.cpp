#include "Graphic/WindowMouseMoveEvent.hpp"

namespace Croissant
{
	namespace Graphic
	{
		WindowMouseMoveEvent::WindowMouseMoveEvent(int32_t deltaX, int32_t deltaY)
			: WindowEvent(WindowEventType::MOUSEMOVE),
			m_deltaX { deltaX }, m_deltaY { deltaY }
		{
		}

		int32_t WindowMouseMoveEvent::DeltaX() const
		{
			return m_deltaX;
		}

		int32_t WindowMouseMoveEvent::DeltaY() const
		{
			return m_deltaY;
		}
	}
}