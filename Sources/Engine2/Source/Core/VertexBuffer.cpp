#include "Core/VertexBuffer.hpp"
#include "Core/AbstractBuffer.hpp"

namespace Croissant
{
	namespace Core
	{
		VertexBuffer::VertexBuffer(VertexBufferDescriptor descriptor, uint32_t numElement, std::shared_ptr<AbstractBuffer> internalBuffer)
			: m_descriptor { descriptor }, m_numElement { numElement }, m_internalBuffer { internalBuffer }
		{
		}

		void* VertexBuffer::Map(BufferAccessEnum access) const
		{
			return m_internalBuffer->Map(access);
		}

		void VertexBuffer::Unmap() const
		{
			m_internalBuffer->Unmap();
		}

		uint32_t VertexBuffer::GetBufferId() const
		{
			return m_internalBuffer->GetBufferId();
		}

		uint32_t VertexBuffer::GetSize() const
		{
			return m_internalBuffer->GetSize();;
		}


		uint32_t VertexBuffer::GetNumElement() const
		{
			return m_numElement;
		}
	}
}