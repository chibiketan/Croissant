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
			Node(Node const&) = delete;
			Node(Node&&) = delete;
			~Node();
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


			class Pimpl;

			Pimpl*	m_pimpl;
		};
	}
}

namespace Croissant
{
	namespace Core
	{
	}
}


#endif // !CROISSANT_ENGINE_CORE_NODE_HPP_INC