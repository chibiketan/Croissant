#include "Graphic/WindowEventKey.hpp"

namespace Croissant
{
	namespace Graphic
	{
		WindowEventKey::WindowEventKey(WindowEventKeyType const& type, WindowKey const& key)
			: WindowEvent(type == WindowEventKeyType::Press ? WindowEventType::KEYDOWN : WindowEventType::KEYUP),
			m_key { key },
			m_type { type }
		{
		}

		WindowKey const& WindowEventKey::Key() const
		{
			return m_key;
		}

		WindowEventKeyType const& WindowEventKey::Type() const
		{
			return m_type;
		}
	}
}