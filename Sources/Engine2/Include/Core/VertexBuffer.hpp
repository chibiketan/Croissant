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
			VertexBuffer(VertexBuffer const&) = delete;
			~VertexBuffer();
			void*		Map(BufferAccessEnum access) const;
			void		Unmap() const;
			uint32_t	GetSize() const;
			VertexBufferDescriptor const&	GetDescriptor() const;

		private:
			class Pimpl;

			Pimpl*	m_pimpl;
		};
	}
}

#endif