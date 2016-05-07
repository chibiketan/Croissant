#ifndef CROISSANT_ENGINE_CORE_VERTEXBUFFER_HPP_INC
#  define CROISSANT_ENGINE_CORE_VERTEXBUFFER_HPP_INC
#  pragma once

#  include "Engine.hpp"
#  include "Core/VertexBufferDescriptor.hpp"

namespace Croissant
{
	namespace Core
	{
		class AbstractBuffer;

		class ENGINE_API VertexBuffer final
		{
		public:
			explicit VertexBuffer(VertexBufferDescriptor descriptor, uint32_t size, std::shared_ptr<AbstractBuffer> internalBuffer);

		private:
			VertexBufferDescriptor			m_descriptor;
			uint32_t						m_size;
			std::shared_ptr<AbstractBuffer>	m_internalBuffer;
		};
	}
}

#endif