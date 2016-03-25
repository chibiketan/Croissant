#ifndef CROISSANT_ENGINE_GRAPHIC_HPP_INC
#  define CROISSANT_ENGINE_GRAPHIC_HPP_INC
#  pragma once

#include "Engine.hpp"
#include "Math/Point4.hpp"
#include "Math/Vector4.hpp"
#include "Math/Matrix4.hpp"

// consist of PI / 180.0
#define DEG_TO_RAD 0.0174532925199433

namespace Croissant
{
	namespace Graphic
	{
		class ENGINE_API Camera final
		{
		public:
			Camera();
			void SetFrustum(float fieldOfViewDegree, float ratio, float nearView, float distantView);

		private:
			void OnFrustumChange();

			Math::Point4	m_position;
			Math::Vector4	m_lookDirection;
			Math::Vector4	m_upDirection;
			Math::Vector4	m_rightDirection;
			std::array<float, 6>	m_frustum;
			Math::Matrix4	m_projectionMatrix;
			Math::Matrix4	m_viewMatrix;

			enum class FrustumIndex
			{
				DirectionMin = 0,	// near
				DirectionMax = 1,	// far
				UpMin = 2,			// bottom
				UpMax = 3,			// top
				RightMin = 4,		// left
				RightMax = 5,		// right
			};
		};
	}
}


#endif // !CROISSANT_ENGINE_GRAPHIC_HPP_INC