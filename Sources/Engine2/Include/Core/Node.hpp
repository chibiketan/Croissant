#ifndef CROISSANT_ENGINE_CORE_NODE_HPP_INC
#  define CROISSANT_ENGINE_CORE_NODE_HPP_INC
#  pragma once

#  include "Engine.hpp"
#  include "Math/Matrix.hpp"
#  include "Math/Quaternion.hpp"
#  include "Math/Vector4.hpp"
#  include <vector>
#  include <memory>
#  include <functional>
#  include <list>

namespace Croissant
{

	namespace Core
	{
		class Mesh;
		class ENGINE_API Node
		{
		public:
			using OnUpdateCallback = std::function<void (Node const&, Math::Matrix4f const&)>;

			Node();
			~Node();
			Node(Node const&) = delete;
			Node(Node&&) = default;
			void						AddOnUpdate(OnUpdateCallback& callback) const;
			void						RemoveOnUpdate(OnUpdateCallback& callback) const;
			void						Move(Math::Vector4 const& move);
			void						Rotate(Math::Quaternion const& rotation);
			Math::Matrix4f const&		GetModelToWorldMatrix() const;
			inline Math::Vector4 const&	GetTranslation() const;
			void						Update() const;
			inline Math::Quaternion const&				GetRotation() const;
			void										AddMesh(std::shared_ptr<Mesh> mesh);
			std::vector<std::shared_ptr<Mesh>> const&	GetMeshes() const;
		
		private:
			static std::string			GenerateName();
			void						PreUpdate() const;
			void						Update(bool goUp) const;
			void						PostUpdate() const;
			void						NotifyUpdate() const;


			std::string								m_name;
			Node*									m_parent;
			std::vector<std::unique_ptr<Node>>		m_children;
			Math::Quaternion						m_rotation;
			Math::Vector4							m_translation;
			std::vector<std::shared_ptr<Mesh>>		m_meshes;
			mutable bool							m_needUpdate;
			mutable std::list<OnUpdateCallback*>	m_onUpdateListeners;
			mutable Math::Matrix4f					m_modelToWorldMatrix;
		};

		using NodePtr = std::shared_ptr<Node>;
		using NodeCPtr = std::shared_ptr<Node const>;
	}
}

namespace Croissant
{
	namespace Core
	{
		inline Math::Vector4 const&	Node::GetTranslation() const
		{
			return m_translation;
		}

		inline Math::Quaternion const&	Node::GetRotation() const
		{
			return m_rotation;
		}
	}
}


#endif // !CROISSANT_ENGINE_CORE_NODE_HPP_INC