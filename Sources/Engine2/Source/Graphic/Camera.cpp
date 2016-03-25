#include "Graphic/Camera.hpp"

namespace Croissant
{
	namespace Graphic
	{
		Camera::Camera()
			: m_position { 0.0f, 0.0f, -1.0f },
			m_lookDirection{ Math::Vector4::UnitZ },
			m_upDirection{ Math::Vector4::UnitY },
			m_rightDirection{ Math::Vector4::UnitX },
			m_frustum{  },
			m_projectionMatrix{},
			m_viewMatrix{}
		{
			SetFrustum(90.0f, 1.0f, 1.0f, 10000.0f);
		}

		void Camera::SetFrustum(float fieldOfViewDegree, float aspectRatio, float nearView, float distantView)
		{
			auto halfAngleRadians = 0.5f * fieldOfViewDegree * static_cast<float>(DEG_TO_RAD);
			m_frustum[static_cast<uint32_t>(FrustumIndex::UpMax)] = nearView*tan(halfAngleRadians);
			m_frustum[static_cast<uint32_t>(FrustumIndex::RightMax)] = aspectRatio*m_frustum[static_cast<uint32_t>(FrustumIndex::UpMax)];
			m_frustum[static_cast<uint32_t>(FrustumIndex::UpMin)] = -m_frustum[static_cast<uint32_t>(FrustumIndex::UpMax)];
			m_frustum[static_cast<uint32_t>(FrustumIndex::RightMin)] = -m_frustum[static_cast<uint32_t>(FrustumIndex::RightMax)];
			m_frustum[static_cast<uint32_t>(FrustumIndex::DirectionMin)] = nearView;
			m_frustum[static_cast<uint32_t>(FrustumIndex::DirectionMax)] = distantView;
			OnFrustumChange();
		}
	}
}