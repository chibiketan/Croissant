#ifndef CROISSANT_ENGINE_CORE_MESH_HPP_INC
#  define CROISSANT_ENGINE_CORE_MESH_HPP_INC

#  include "Engine.hpp"
#  include "Core/VertexBuffer.hpp"
#  include "Core/IndexBuffer.hpp"
#  include <memory>

namespace Croissant
{
	namespace Core
	{
		class ENGINE_API Mesh
		{
		public:
			Mesh(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer);
			~Mesh();
			std::shared_ptr<VertexBuffer const>	GetVertexBuffer() const;
			std::shared_ptr<IndexBuffer const>	GetIndexBuffer() const;

		private:
			class Pimpl;

			Pimpl*	m_pimpl;
		};
	}
}

#endif // !CROISSANT_ENGINE_CORE_MESH_HPP_INC