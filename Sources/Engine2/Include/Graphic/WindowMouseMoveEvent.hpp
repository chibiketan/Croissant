#ifndef CROISSANT_ENGINE_GRAPHIC_WINDOWMOUSEMOVEEVENT_HPP_INC
#  define CROISSANT_ENGINE_GRAPHIC_WINDOWMOUSEMOVEEVENT_HPP_INC

#  include "Engine.hpp"
#  include "Graphic/WindowEvent.hpp"
#  include <cstdint>
#include <algorithm>

namespace Croissant
{
	namespace Graphic
	{
		class ENGINE_API WindowMouseMoveEvent final : public WindowEvent
		{
		public:
			WindowMouseMoveEvent(int32_t deltaX, int32_t deltaY);
			int32_t DeltaX() const;
			int32_t DeltaY() const;
		private:
			int32_t					m_deltaX;
			int32_t					m_deltaY;
		};
	}
}


#endif // !CROISSANT_ENGINE_GRAPHIC_WINDOWMOUSEMOVEEVENT_HPP_INC