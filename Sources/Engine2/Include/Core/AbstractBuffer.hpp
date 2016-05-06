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
			virtual ~AbstractBuffer(){};
			virtual int32_t	GetBufferId() const = 0;
			virtual void*	Map(BufferAccessEnum access) const = 0;
			virtual void	Unmap() const = 0;

		};
	}
}


#endif // !CROISSANT_ENGINE_CORE_ABSTRACTBUFFER_HPP_INC