#include "Core/IndexBuffer.hpp"
#include "Core/AbstractBuffer.hpp"

namespace Croissant
{
	namespace Core
	{
		IndexBuffer::IndexBuffer(std::shared_ptr<AbstractBuffer> internalBuffer)
			: m_internalBuffer { internalBuffer }
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
	}
}