#include "Core/IndexBuffer.hpp"
#include "Core/AbstractBuffer.hpp"
#include "Debug/MemoryManager.hpp"

namespace Croissant
{
	namespace Core
	{
		class IndexBuffer::Pimpl
		{
		public:
			Pimpl(std::shared_ptr<AbstractBuffer> internalBuffer)
				: m_internalBuffer{ internalBuffer }
			{}

			std::shared_ptr<AbstractBuffer> m_internalBuffer;
		};

		IndexBuffer::IndexBuffer(std::shared_ptr<AbstractBuffer> internalBuffer)
			: m_pimpl{ CROISSANT_NEW Pimpl{ internalBuffer } }
		{
		}

		IndexBuffer::~IndexBuffer()
		{
			CROISSANT_DELETE(m_pimpl);
			m_pimpl = nullptr;
		}

		void* IndexBuffer::Map(BufferAccessEnum access) const
		{
			return m_pimpl->m_internalBuffer->Map(access);
		}

		void IndexBuffer::Unmap() const
		{
			m_pimpl->m_internalBuffer->Unmap();
		}

		uint32_t IndexBuffer::GetBufferId() const
		{
			return m_pimpl->m_internalBuffer->GetBufferId();
		}
	}
}