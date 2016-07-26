#  include "Core/Mesh.hpp"

namespace Croissant
{
	namespace Core
	{
		Mesh::Mesh(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer)
			: m_vertexBuffer { vertexBuffer }, m_indexBuffer { indexBuffer }
		{	
		}

		std::shared_ptr<VertexBuffer const> Mesh::GetVertexBuffer() const
		{
			return m_vertexBuffer;
		}

		IndexBufferCPtr Mesh::GetIndexBuffer() const
		{
			return m_indexBuffer;
		}

		void Mesh::SetIndexBuffer(IndexBufferPtr const& indexBuffer)
		{
			m_indexBuffer = indexBuffer;
		}
	}
}