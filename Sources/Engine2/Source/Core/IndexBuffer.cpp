#include "Core/IndexBuffer.hpp"
#include "Core/AbstractBuffer.hpp"

namespace Croissant
{
	namespace Core
	{
		IndexBuffer::IndexBuffer(uint32_t numElement, std::shared_ptr<AbstractBuffer> internalBuffer)
			: m_internalBuffer { internalBuffer }, m_numElement(numElement)
		{
		}

		void* IndexBuffer::Map(BufferAccessEnum access) const
		{
			return m_internalBuffer->Map(access);
		}

		void IndexBuffer::Unmap() const
		{
			m_internalBuffer->Unmap();
		}

		uint32_t IndexBuffer::GetBufferId() const
		{
			return m_internalBuffer->GetBufferId();
		}

		uint32_t IndexBuffer::GetSize() const
		{
			return m_internalBuffer->GetSize();
		}

		uint32_t IndexBuffer::GetNumElement() const
		{
			return m_numElement;
		}
	}
}