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
			explicit VertexBuffer(VertexBufferDescriptor descriptor, uint32_t numElement, std::shared_ptr<AbstractBuffer> internalBuffer);
			void*		Map(BufferAccessEnum access) const;
			void		Unmap() const;
			uint32_t	GetSize() const;
			uint32_t	GetBufferId() const;
			uint32_t	GetNumElement() const;
			VertexBufferDescriptor const&	GetDescriptor() const;

		private:
			VertexBufferDescriptor			m_descriptor;
			std::uint32_t					m_numElement;
			std::shared_ptr<AbstractBuffer>	m_internalBuffer;
		};
	}
}

namespace Croissant
{
	namespace Core
	{
		inline VertexBufferDescriptor const& VertexBuffer::GetDescriptor() const
		{
			return m_descriptor;
		}

	}
}
#endif