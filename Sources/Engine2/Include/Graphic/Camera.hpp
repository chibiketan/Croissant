#ifndef CROISSANT_ENGINE_GRAPHIC_HPP_INC
#  define CROISSANT_ENGINE_GRAPHIC_HPP_INC
#  pragma once

#include "Engine.hpp"
#include "Math/Point4.hpp"
#include "Math/Vector4.hpp"
#include "Math/Matrix4.hpp"
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
			void					Move(Math::Vector4 const& translation);
			Math::Matrix4 const&	GetProjectionViewMatrix() const;
			Math::Point4 const&		Position() const;
			void Rotate(Math::Quaternion const& quaternion);
			void Rotate(Math::EulerAngle const& angle);
			void SetNode(node_ptr node);
		private:
			void OnFrustumChange();
			void OnFrameChange();
			void UpdateProjectionViewMatrix();

			Math::Point4					m_position;
			Math::Vector4					m_lookDirection;
			Math::Vector4					m_upDirection;
			Math::Vector4					m_rightDirection;
			std::array<float, 6>			m_frustum;
			Math::Matrix4					m_projectionMatrix;
			Math::Matrix4					m_viewMatrix;
			Math::Matrix4					m_projectionViewMatrix;
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


#endif // !CROISSANT_ENGINE_GRAPHIC_HPP_INC