#ifndef CROISSANT_ENGINE_CORE_BUFFERACCESSOR_HPP_INC
#  define CROISSANT_ENGINE_CORE_BUFFERACCESSOR_HPP_INC
#  pragma once

#  include "Core/Enums.hpp"

namespace Croissant
{
	namespace Core
	{
		template<typename TBuffer, typename TData>
		class BufferAccessor final
		{
		public:
			BufferAccessor(TBuffer& buffer, BufferAccessEnum access);
			~BufferAccessor();
			size_t	GetSize() const;
			TData&	operator[](size_t index);

		private:
			TBuffer&	m_buffer;
			TData*		m_data;

		};
	}
}

#  include "Core/BufferAccessor.inl"

#endif // !CROISSANT_ENGINE_CORE_BUFFERACCESSOR_HPP_INC