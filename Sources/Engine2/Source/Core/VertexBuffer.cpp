#include "Core/VertexBuffer.hpp"
#include "Core/AbstractBuffer.hpp"
#include "Debug/MemoryManager.hpp"

namespace Croissant
{
	namespace Core
	{
		class VertexBuffer::Pimpl
		{
		public:
			Pimpl(VertexBufferDescriptor descriptor, uint32_t size, std::shared_ptr<AbstractBuffer> internalBuffer)
				: m_descriptor{ descriptor }, m_size{ size }, m_internalBuffer{ internalBuffer }
			{}

			VertexBufferDescriptor			m_descriptor;
			uint32_t						m_size;
			std::shared_ptr<AbstractBuffer>	m_internalBuffer;
		};

		VertexBuffer::VertexBuffer(VertexBufferDescriptor descriptor, uint32_t size, std::shared_ptr<AbstractBuffer> internalBuffer)
			: m_pimpl{ CROISSANT_NEW Pimpl{ descriptor, size, internalBuffer }}
		{
		}

		VertexBuffer::~VertexBuffer()
		{
			CROISSANT_DELETE(m_pimpl);
			m_pimpl = nullptr;
		}

		void* VertexBuffer::Map(BufferAccessEnum access) const
		{
			return m_pimpl->m_internalBuffer->Map(access);
		}

		void VertexBuffer::Unmap() const
		{
			m_pimpl->m_internalBuffer->Unmap();
		}

		uint32_t VertexBuffer::GetSize() const
		{
			return m_pimpl->m_size;
		}

		VertexBufferDescriptor const& VertexBuffer::GetDescriptor() const
		{
			return m_pimpl->m_descriptor;
		}
	}
}