#include "Core/VertexBuffer.hpp"
#include "Core/AbstractBuffer.hpp"

namespace Croissant
{
	namespace Core
	{
		VertexBuffer::VertexBuffer(VertexBufferDescriptor descriptor, uint32_t size, std::shared_ptr<AbstractBuffer> internalBuffer)
			: m_descriptor { descriptor }, m_size { size }, m_internalBuffer { internalBuffer }
		{
		}

	}
}