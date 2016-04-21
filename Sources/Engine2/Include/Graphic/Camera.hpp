#ifndef CROISSANT_ENGINE_GRAPHIC_HPP_INC
#  define CROISSANT_ENGINE_GRAPHIC_HPP_INC
#  pragma once

#include "Engine.hpp"
#include "Math/Matrix.hpp"
#include "Math/Vector4.hpp"
#include "Math/Point4.hpp"
#include <memory>
#include <Core/Node.hpp>

namespace Croissant{
	namespace Math{
		class EulerAngle;
	}
}

namespace Croissant
{
	namespace Graphic
	{
		class ENGINE_API Camera final
		{
		public:
			using node_ptr = std::shared_ptr<Core::Node>;

			Camera();
			inline Math::Vector4	GetLookDirection() const;
			inline Math::Vector4	GetRightDirection() const;
			inline Math::Vector4	GetUpDirection() const;
			inline Math::Matrix4f	GetProjectionViewMatrix() const;
			inline Math::Point4		GetPosition() const;
			inline void				SetFieldOfViewDegree(float val);
			inline void				SetAspectRatio(float val);
			inline void				SetNearDistance(float val);
			inline void				SetFarDistance(float val);
			void					SetNode(node_ptr node);
			inline Math::Matrix4f const& GetViewMatrix() const;
			inline Math::Matrix4f const& GetProjectionMatrix() const;
		private:
			void OnFrustumChange() const;
			void OnFrameChange() const;
			void OnNodeUpdate() const;

			float							m_fieldOfViewDegree;
			float							m_aspectRatio;
			float							m_nearDistance;
			float							m_farDistance;
			mutable Math::Matrix4f			m_projectionMatrix;
			mutable Math::Matrix4f			m_viewMatrix;
			node_ptr						m_node;
			mutable bool					m_projectionNeedUpdate;
			mutable bool					m_viewNeedUpdate;
			Core::Node::OnUpdateCallback	m_nodeUpdateCallback;
		};
	}
}

namespace Croissant
{
	namespace Graphic
	{
		inline Math::Point4 Camera::GetPosition() const
		{
			Math::Point4 result{ 0, 0, 0 };

			if (nullptr != m_node)
			{
				result = result * m_node->GetModelToWorldMatrix();
			}

			return result;
		}

		inline Math::Vector4 Camera::GetLookDirection() const
		{
			auto result{ Math::Vector4::UnitZ };

			if (nullptr != m_node)
			{
				result = result * m_node->GetModelToWorldMatrix();
			}

			return result;
		}

		inline Math::Vector4 Camera::GetRightDirection() const
		{
			auto result{ Math::Vector4::UnitX };

			if (nullptr != m_node)
			{
				result = result * m_node->GetModelToWorldMatrix();
			}

			return result;
		}

		inline Math::Vector4 Camera::GetUpDirection() const
		{
			auto result{ Math::Vector4::UnitY };

			if (nullptr != m_node)
			{
				result = result * m_node->GetModelToWorldMatrix();
			}

			return result;
		}
	
		inline void Camera::SetFieldOfViewDegree(float val)
		{
			m_fieldOfViewDegree = val;
			m_projectionNeedUpdate = true;
		}

		inline void Camera::SetAspectRatio(float val)
		{
			m_aspectRatio = val;
			m_projectionNeedUpdate = true;
		}

		inline void Camera::SetNearDistance(float val)
		{
			m_nearDistance = val;
			m_projectionNeedUpdate = true;
		}

		inline void Camera::SetFarDistance(float val)
		{
			m_farDistance = val;
			m_projectionNeedUpdate = true;
		}

		inline Math::Matrix4f Camera::GetProjectionViewMatrix() const
		{
			OnFrameChange();
			OnFrustumChange();
			return m_projectionMatrix * m_viewMatrix;
		}

		inline Math::Matrix4f const& Camera::GetViewMatrix() const
		{
			OnFrameChange();
			return m_viewMatrix;
		}

		inline Math::Matrix4f const& Camera::GetProjectionMatrix() const
		{
			OnFrustumChange();
			return m_projectionMatrix;
			
		}
	}
}


#endif // !CROISSANT_ENGINE_GRAPHIC_HPP_INC