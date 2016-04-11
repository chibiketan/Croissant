#ifndef CROISSANT_ENGINE_CORE_NODE_HPP_INC
#  define CROISSANT_ENGINE_CORE_NODE_HPP_INC
#  pragma once

#  include "Engine.hpp"
#  include "Math/Quaternion.hpp"
#  include "Math/Vector4.hpp"
#  include "Math/Matrix4.hpp"
#  include <vector>
#  include <memory>
#  include <functional>
#  include <list>

namespace Croissant
{

	namespace Core
	{
		class ENGINE_API Node
		{
		public:
			using OnUpdateCallback = std::function<void (Node const&, Math::Matrix4 const&)>;

			Node();
			void AddOnUpdate(OnUpdateCallback& callback) const;
			void RemoveOnUpdate(OnUpdateCallback& callback) const;
			void Move(Math::Vector4 const& move);
			void Rotate(Math::Quaternion const& rotation);
			Math::Matrix4 const&	GetModelToWorldMatrix() const;

		private:
			static std::string	GenerateName();
			void				Update() const;

			std::string								m_name;
			Node*									m_parent;
			std::vector<std::unique_ptr<Node>>		m_children;
			Math::Quaternion						m_rotation;
			Math::Vector4							m_translation;
			mutable bool							m_needUpdate;
			mutable std::list<OnUpdateCallback*>	m_onUpdateListeners;
			mutable Math::Matrix4					m_modelToWorldMatrix;
		};
	}
}


#endif // !CROISSANT_ENGINE_CORE_NODE_HPP_INC