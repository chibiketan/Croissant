#ifndef CROISSANT_ENGINE_CORE_BUFFERACCESSOR_INL_INC
#  define CROISSANT_ENGINE_CORE_BUFFERACCESSOR_INL_INC
#  pragma once

#  include "Core/BufferAccessor.hpp"
#include <assert.h>

namespace Croissant
{
	namespace Core
	{
		template <typename TBuffer, typename TData>
		BufferAccessor<TBuffer, TData>::BufferAccessor(TBuffer& buffer, BufferAccessEnum access)
			: m_buffer { buffer }, m_data { nullptr }
		{
			m_data = static_cast<TData*>(m_buffer.Map(access));
		}

		template <typename TBuffer, typename TData>
		BufferAccessor<TBuffer, TData>::~BufferAccessor()
		{
			if (nullptr != m_data)
			{
				m_buffer.Unmap();
				m_data = nullptr;
			}
		}

		template <typename TBuffer, typename TData>
		size_t BufferAccessor<TBuffer, TData>::GetSize() const
		{
			//return m_buffer.GetSize() / sizeof(TData);
			return m_buffer.GetNumElement();
		}

		template <typename TBuffer, typename TData>
		TData& BufferAccessor<TBuffer, TData>::operator[](size_t index)
		{
			assert((sizeof(TData) * index) < m_buffer.GetSize() && "Index en dehors des limites");
			return m_data[index];
		}
	}
}

#endif // !CROISSANT_ENGINE_CORE_BUFFERACCESSOR_INL_INC