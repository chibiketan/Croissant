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

		void Camera::SetAxes(Math::Vector4 const& lookVector, Math::Vector4 const& upVector, Math::Vector4 const& rightVector)
		{
			m_lookDirection = lookVector;
			m_upDirection = upVector;
			m_rightDirection = rightVector;
			OnFrameChange();
		}

		void Camera::SetPosition(Math::Point4 const& position)
		{
			m_position = position;
			OnFrameChange();
		}

		Math::Matrix4 const& Camera::GetProjectionViewMatrix() const
		{
			return m_projectionViewMatrix;
		}

		void Camera::OnFrustumChange()
		{
			// map (x,y,z) into [-1,1]x[-1,1]x[-1,1].
			float dMin = m_frustum[static_cast<uint32_t>(FrustumIndex::DirectionMin)];
			float dMax = m_frustum[static_cast<uint32_t>(FrustumIndex::DirectionMax)];
			float uMin = m_frustum[static_cast<uint32_t>(FrustumIndex::UpMin)];
			float uMax = m_frustum[static_cast<uint32_t>(FrustumIndex::UpMax)];
			float rMin = m_frustum[static_cast<uint32_t>(FrustumIndex::RightMin)];
			float rMax = m_frustum[static_cast<uint32_t>(FrustumIndex::RightMax)];
			float invDDiff = 1.0f / (dMax - dMin);
			float invUDiff = 1.0f / (uMax - uMin);
			float invRDiff = 1.0f / (rMax - rMin);

			m_projectionMatrix(0, 0) = 2.0f * dMin * invRDiff;
			m_projectionMatrix(0, 1) = 0.0f;
			m_projectionMatrix(0, 2) = -(rMin + rMax) * invRDiff;
			m_projectionMatrix(0, 3) = 0.0f;
			m_projectionMatrix(1, 0) = 0.0f;
			m_projectionMatrix(1, 1) = 2.0f * dMin * invUDiff;
			m_projectionMatrix(1, 2) = -(uMin + uMax) * invUDiff;
			m_projectionMatrix(1, 3) = 0.0f;
			m_projectionMatrix(2, 0) = 0.0f;
			m_projectionMatrix(2, 1) = 0.0f;
			m_projectionMatrix(2, 2) = (dMin + dMax) * invDDiff;
			m_projectionMatrix(2, 3) = -2.0f * dMin * dMax * invDDiff;
			m_projectionMatrix(3, 0) = 0.0f;
			m_projectionMatrix(3, 1) = 0.0f;
			m_projectionMatrix(3, 2) = 1.0f;
			m_projectionMatrix(3, 3) = 0.0f;

			UpdateProjectionViewMatrix();
		}

		void Camera::OnFrameChange()
		{
			m_viewMatrix(0, 0) = m_rightDirection.X();
			m_viewMatrix(0, 1) = m_rightDirection.Y();
			m_viewMatrix(0, 2) = m_rightDirection.Z();
			m_viewMatrix(0, 3) = -m_position.DotProduct(m_rightDirection);
			m_viewMatrix(1, 0) = m_upDirection.X();
			m_viewMatrix(1, 1) = m_upDirection.Y();
			m_viewMatrix(1, 2) = m_upDirection.Z();
			m_viewMatrix(1, 3) = -m_position.DotProduct(m_upDirection);
			m_viewMatrix(2, 0) = m_lookDirection.X();
			m_viewMatrix(2, 1) = m_lookDirection.Y();
			m_viewMatrix(2, 2) = m_lookDirection.Z();
			m_viewMatrix(2, 3) = -m_position.DotProduct(m_lookDirection);
			m_viewMatrix(3, 0) = 0.0f;
			m_viewMatrix(3, 1) = 0.0f;
			m_viewMatrix(3, 2) = 0.0f;
			m_viewMatrix(3, 3) = 1.0f;

			//mInverseViewMatrix(0, 0) = mRVector[0];
			//mInverseViewMatrix(0, 1) = mUVector[0];
			//mInverseViewMatrix(0, 2) = mDVector[0];
			//mInverseViewMatrix(0, 3) = mPosition[0];
			//mInverseViewMatrix(1, 0) = mRVector[1];
			//mInverseViewMatrix(1, 1) = mUVector[1];
			//mInverseViewMatrix(1, 2) = mDVector[1];
			//mInverseViewMatrix(1, 3) = mPosition[1];
			//mInverseViewMatrix(2, 0) = mRVector[2];
			//mInverseViewMatrix(2, 1) = mUVector[2];
			//mInverseViewMatrix(2, 2) = mDVector[2];
			//mInverseViewMatrix(2, 3) = mPosition[2];
			//mInverseViewMatrix(3, 0) = 0.0f;
			//mInverseViewMatrix(3, 1) = 0.0f;
			//mInverseViewMatrix(3, 2) = 0.0f;
			//mInverseViewMatrix(3, 3) = 1.0f;

			UpdateProjectionViewMatrix();
		}

		void Camera::UpdateProjectionViewMatrix()
		{
			m_projectionViewMatrix = m_projectionMatrix * m_viewMatrix;
		}
	}
}