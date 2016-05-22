#include "Graphic/Camera.hpp"
#include "Math/Math.hpp"

namespace Croissant
{
	namespace Graphic
	{
		Camera::Camera()
			: m_fieldOfViewDegree{ 0 }, m_aspectRatio{ 0 }, m_nearDistance{ 0 }, m_farDistance{ 0 },
			m_projectionMatrix{}, m_viewMatrix{},
			m_node{ nullptr }, m_projectionNeedUpdate { false }, m_viewNeedUpdate { false },
			m_nodeUpdateCallback{ [this](Core::Node const&, Math::Matrix4f const&) { this->OnNodeUpdate();  } }
		{
			SetFieldOfViewDegree(90.0f);
			SetAspectRatio(4.0f / 3.0f);
			SetNearDistance(1.0f);
			SetFarDistance(1000.0f);
		}

		void Camera::SetNode(node_ptr node)
		{
			if (nullptr != m_node)
			{
				m_node->RemoveOnUpdate(m_nodeUpdateCallback);
			}

			m_node = node;
			if (nullptr != node)
			{
				node->AddOnUpdate(m_nodeUpdateCallback);
			}
			
			m_viewNeedUpdate = true;
		}

		void Camera::OnFrustumChange() const
		{
			if (!m_projectionNeedUpdate)
			{
				return;
			}

			m_projectionNeedUpdate = false;
			auto matrix{ Math::Matrix4f::Identity() };
			auto angle = Math::ToRadian(m_fieldOfViewDegree ) * 0.5f;

			auto yScale = Math::Tan(CROISSANT_PI_2 - angle);

			// https://msdn.microsoft.com/en-us/library/windows/desktop/bb204944(v=vs.85).aspx
			matrix(0, 0) = yScale / m_aspectRatio;
			matrix(1, 1) = yScale;
			matrix(2, 2) = m_farDistance / (m_farDistance - m_nearDistance);
			matrix(2, 3) = -m_nearDistance * m_farDistance / (m_farDistance - m_nearDistance);
			matrix(3, 2) = 1;
			matrix(3, 3) = 0;
			m_projectionMatrix = std::move(matrix);
		}

		void Camera::OnFrameChange() const
		{
			if (!m_viewNeedUpdate)
			{
				return;
			}

			m_viewNeedUpdate = false;
			//if (nullptr == m_node)
			//{
			//	m_viewMatrix = Math::Matrix4f::Identity();
			//}
			//else
			//{
			//	m_viewMatrix = m_node->GetModelToWorldMatrix().GetInverse();
			//}

			auto look = GetLookDirection();
			auto right = GetRightDirection();
			auto up = GetUpDirection();
			auto position = GetPosition();
			//auto look = Math::Vector4::UnitZ;
			//auto right = Math::Vector4::UnitX;
			//auto up = Math::Vector4::UnitY;
			//Math::Point4 position{ 0.0f, 0.0f, 0.0f };
			m_viewMatrix(0, 0) = right.X();
			m_viewMatrix(0, 1) = right.Y();
			m_viewMatrix(0, 2) = right.Z();
			m_viewMatrix(0, 3) = -position.DotProduct(right);
			m_viewMatrix(1, 0) = up.X();
			m_viewMatrix(1, 1) = up.Y();
			m_viewMatrix(1, 2) = up.Z();
			m_viewMatrix(1, 3) = -position.DotProduct(up);
			m_viewMatrix(2, 0) = look.X();
			m_viewMatrix(2, 1) = look.Y();
			m_viewMatrix(2, 2) = look.Z();
			m_viewMatrix(2, 3) = -position.DotProduct(look);
			m_viewMatrix(3, 0) = 0.0f;
			m_viewMatrix(3, 1) = 0.0f;
			m_viewMatrix(3, 2) = 0.0f;
			m_viewMatrix(3, 3) = 1.0f;
		}

		void Camera::OnNodeUpdate() const
		{
			m_viewNeedUpdate = true;
		}

	}
}