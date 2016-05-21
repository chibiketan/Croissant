#include "Graphic/Camera.hpp"
#include "Math/Math.hpp"
#include "Debug/MemoryManager.hpp"

namespace Croissant
{
	namespace Graphic
	{
		class Camera::Pimpl
		{
		public:
			Pimpl(Core::Node::OnUpdateCallback nodeUpdateCallback)
				: m_node{ nullptr }, m_nodeUpdateCallback{ nodeUpdateCallback },
				m_projectionMatrix{}, m_viewMatrix{}
			{}

			node_ptr						m_node;
			Core::Node::OnUpdateCallback	m_nodeUpdateCallback;
			mutable Math::Matrix4f			m_projectionMatrix;
			mutable Math::Matrix4f			m_viewMatrix;
		};

		Camera::Camera()
			: m_pimpl{ CROISSANT_NEW Pimpl{ [this](Core::Node const&, Math::Matrix4f const&) { this->OnNodeUpdate();  } } },
			m_fieldOfViewDegree{ 0 }, m_aspectRatio{ 0 }, m_nearDistance{ 0 }, m_farDistance{ 0 },
			m_projectionNeedUpdate { false }, m_viewNeedUpdate { false }
		{
			SetFieldOfViewDegree(90.0f);
			SetAspectRatio(4.0f / 3.0f);
			SetNearDistance(1.0f);
			SetFarDistance(1000.0f);
		}

		Camera::~Camera()
		{
			CROISSANT_DELETE(m_pimpl);
			m_pimpl = nullptr;
		}

		void Camera::SetNode(node_ptr node)
		{
			if (nullptr != m_pimpl->m_node)
			{
				m_pimpl->m_node->RemoveOnUpdate(m_pimpl->m_nodeUpdateCallback);
			}

			m_pimpl->m_node = node;
			if (nullptr != node)
			{
				node->AddOnUpdate(m_pimpl->m_nodeUpdateCallback);
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
			m_pimpl->m_projectionMatrix = std::move(matrix);
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
			m_pimpl->m_viewMatrix(0, 0) = right.X();
			m_pimpl->m_viewMatrix(0, 1) = right.Y();
			m_pimpl->m_viewMatrix(0, 2) = right.Z();
			m_pimpl->m_viewMatrix(0, 3) = -position.DotProduct(right);
			m_pimpl->m_viewMatrix(1, 0) = up.X();
			m_pimpl->m_viewMatrix(1, 1) = up.Y();
			m_pimpl->m_viewMatrix(1, 2) = up.Z();
			m_pimpl->m_viewMatrix(1, 3) = -position.DotProduct(up);
			m_pimpl->m_viewMatrix(2, 0) = look.X();
			m_pimpl->m_viewMatrix(2, 1) = look.Y();
			m_pimpl->m_viewMatrix(2, 2) = look.Z();
			m_pimpl->m_viewMatrix(2, 3) = -position.DotProduct(look);
			m_pimpl->m_viewMatrix(3, 0) = 0.0f;
			m_pimpl->m_viewMatrix(3, 1) = 0.0f;
			m_pimpl->m_viewMatrix(3, 2) = 0.0f;
			m_pimpl->m_viewMatrix(3, 3) = 1.0f;
		}

		void Camera::OnNodeUpdate() const
		{
			m_viewNeedUpdate = true;
		}

		Math::Point4 Camera::GetPosition() const
		{
			Math::Point4 result{ 0, 0, 0 };

			if (nullptr != m_pimpl->m_node)
			{
				result = result * m_pimpl->m_node->GetModelToWorldMatrix();
			}

			return result;
		}

		Math::Vector4 Camera::GetLookDirection() const
		{
			auto result{ Math::Vector4::UnitZ };

			if (nullptr != m_pimpl->m_node)
			{
				result = result * m_pimpl->m_node->GetModelToWorldMatrix();
			}

			return result;
		}

		Math::Vector4 Camera::GetRightDirection() const
		{
			auto result{ Math::Vector4::UnitX };

			if (nullptr != m_pimpl->m_node)
			{
				result = result * m_pimpl->m_node->GetModelToWorldMatrix();
			}

			return result;
		}

		Math::Vector4 Camera::GetUpDirection() const
		{
			auto result{ Math::Vector4::UnitY };

			if (nullptr != m_pimpl->m_node)
			{
				result = result * m_pimpl->m_node->GetModelToWorldMatrix();
			}

			return result;
		}

		Math::Matrix4f Camera::GetProjectionViewMatrix() const
		{
			OnFrameChange();
			OnFrustumChange();
			return m_pimpl->m_projectionMatrix * m_pimpl->m_viewMatrix;
		}

		Math::Matrix4f const& Camera::GetViewMatrix() const
		{
			OnFrameChange();
			return m_pimpl->m_viewMatrix;
		}

		Math::Matrix4f const& Camera::GetProjectionMatrix() const
		{
			OnFrustumChange();
			return m_pimpl->m_projectionMatrix;

		}
	}
}