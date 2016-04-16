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
			void SetFrustum(float fieldOfViewDegree, float ratio, float nearView, float distantView);
			void SetAxes(Math::Vector4 const& lookVector, Math::Vector4 const& upVector, Math::Vector4 const& rightVector);
			void SetPosition(Math::Point4 const& position);
			Math::Vector4 const&	LookVector() const;
			Math::Vector4 const&	RightVector() const;
			Math::Vector4 const&	UpVector() const;
			inline Math::Vector4 const&	GetRealLookDirection() const;
			inline Math::Vector4 const&	GetRealRightDirection() const;
			inline Math::Vector4 const&	GetRealUpDirection() const;
			void					Move(Math::Vector4 const& translation);
			Math::Matrix4f const&	GetProjectionViewMatrix() const;
			Math::Point4 const&		Position() const;
			void					Rotate(Math::Quaternion const& quaternion);
			void					Rotate(Math::EulerAngle const& angle);
			void					SetNode(node_ptr node);
		private:
			void OnFrustumChange();
			void OnFrameChange();
			void OnNodeUpdate();
			void UpdateRealDirections();
			void UpdateProjectionViewMatrix();

			Math::Point4					m_position;
			Math::Vector4					m_lookDirection;
			Math::Vector4					m_upDirection;
			Math::Vector4					m_rightDirection;
			Math::Vector4					m_realLookDirection;
			Math::Vector4					m_realUpDirection;
			Math::Vector4					m_realRightDirection;
			std::array<float, 6>			m_frustum;
			Math::Matrix4f					m_projectionMatrix;
			Math::Matrix4f					m_viewMatrix;
			Math::Matrix4f					m_projectionViewMatrix;
			node_ptr						m_node;
			Core::Node::OnUpdateCallback	m_nodeUpdateCallback;

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

namespace Croissant
{
	namespace Graphic
	{
		inline Math::Vector4 const&	Camera::GetRealLookDirection() const
		{
			return m_realLookDirection;
		}

		inline Math::Vector4 const&	Camera::GetRealRightDirection() const
		{
			return m_realRightDirection;
		}

		inline Math::Vector4 const&	Camera::GetRealUpDirection() const
		{
			return m_realUpDirection;
		}
	}
}


#endif // !CROISSANT_ENGINE_GRAPHIC_HPP_INC