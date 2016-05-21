#include "Core/Mesh.hpp"
#include "Debug/MemoryManager.hpp"

namespace Croissant
{
	namespace Core
	{
		class Mesh::Pimpl
		{
		public:
			Pimpl(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer)
				: m_vertexBuffer{ vertexBuffer }, m_indexBuffer{ indexBuffer }
			{}
			std::shared_ptr<VertexBuffer>	m_vertexBuffer;
			std::shared_ptr<IndexBuffer>		m_indexBuffer;
		};

		Mesh::Mesh(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer)
			: m_pimpl{ CROISSANT_NEW Pimpl{ vertexBuffer, indexBuffer }}
		{	
		}

		Mesh::~Mesh()
		{
			CROISSANT_DELETE(m_pimpl);
			m_pimpl = nullptr;
		}

		std::shared_ptr<VertexBuffer const> Mesh::GetVertexBuffer() const
		{
			return m_pimpl->m_vertexBuffer;
		}

		std::shared_ptr<IndexBuffer const> Mesh::GetIndexBuffer() const
		{
			return m_pimpl->m_indexBuffer;
		}
	}
}