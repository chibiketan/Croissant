#ifndef CROISSANT_ENGINE_GRAPHIC_WINDOWEVENTKEYPRESS_HPP_INC
#  define CROISSANT_ENGINE_GRAPHIC_WINDOWEVENTKEYPRESS_HPP_INC
#  pragma once

#  include "Engine.hpp"
#  include "Graphic/WindowEvent.hpp"

namespace Croissant
{
	namespace Graphic
	{
		enum class WindowKey
		{
			Up,
			Down,
			Left,
			Right,
			PageUp,
			PageDown
		};

		enum class WindowEventKeyType
		{
			Press,
			Release
		};

		class ENGINE_API WindowEventKey : public WindowEvent
		{
		public:
			WindowEventKey(WindowEventKeyType const& type, WindowKey const& key);
			WindowKey const& Key() const;
			WindowEventKeyType const& Type() const;

		private:
			WindowKey			m_key;
			WindowEventKeyType	m_type;
		};

	}
}


#endif // !CROISSANT_ENGINE_GRAPHIC_WINDOWEVENTKEYPRESS_HPP_INC
