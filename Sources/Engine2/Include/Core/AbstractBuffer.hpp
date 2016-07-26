#ifndef CROISSANT_ENGINE_CORE_ABSTRACTBUFFER_HPP_INC
#  define CROISSANT_ENGINE_CORE_ABSTRACTBUFFER_HPP_INC
#  pragma once

#  include "Engine.hpp"
#  include "Core/Enums.hpp"

namespace Croissant
{
	namespace Core
	{
		class ENGINE_API AbstractBuffer
		{
		public:
			explicit AbstractBuffer(uint32_t size);
			virtual ~AbstractBuffer(){};
			virtual int32_t	GetBufferId() const = 0;
			virtual void*	Map(BufferAccessEnum access) const = 0;
			virtual void	Unmap() const = 0;
			uint32_t		GetSize() const;

		private:
			uint32_t	m_size;

		};
	}
}

namespace Croissant
{
	namespace Core
	{
		inline AbstractBuffer::AbstractBuffer(uint32_t size)
			: m_size(size)
		{
		}

		inline uint32_t AbstractBuffer::GetSize() const
		{
			return m_size;
		}
	}
}
#endif // !CROISSANT_ENGINE_CORE_ABSTRACTBUFFER_HPP_INC