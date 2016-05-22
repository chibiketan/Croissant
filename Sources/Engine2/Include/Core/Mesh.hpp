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
			std::shared_ptr<VertexBuffer const>	GetVertexBuffer() const;
			std::shared_ptr<IndexBuffer const>	GetIndexBuffer() const;

		private:
			std::shared_ptr<VertexBuffer>	m_vertexBuffer;
			std::shared_ptr<IndexBuffer>		m_indexBuffer;
		};
	}
}

#endif // !CROISSANT_ENGINE_CORE_MESH_HPP_INC