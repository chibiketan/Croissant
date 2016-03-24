#ifndef CROISSANT_ENGINE_GRAPHIC_HPP_INC
#  define CROISSANT_ENGINE_GRAPHIC_HPP_INC
#  pragma once

#include "Engine.hpp"
#include "Math/Point4.hpp"
#include "Math/Vector4.hpp"

namespace Croissant
{
	namespace Graphic
	{
		class ENGINE_API Camera final
		{
		public:
			Camera();

		private:
			Math::Point4	m_position;
			Math::Vector4	m_direction;
		};
	}
}


#endif // !CROISSANT_ENGINE_GRAPHIC_HPP_INC