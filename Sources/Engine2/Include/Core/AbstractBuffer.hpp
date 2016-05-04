#ifndef CROISSANT_ENGINE_CORE_ABSTRACTBUFFER_HPP_INC
#  define CROISSANT_ENGINE_CORE_ABSTRACTBUFFER_HPP_INC

#  include "Engine.hpp"

namespace Croissant
{
	namespace Core
	{
		class ENGINE_API AbstractBuffer
		{
		public:
			virtual ~AbstractBuffer(){};
			virtual int32_t	GetBufferId() = 0;
			void*	Map();
			void	Unmap();

		protected:
			virtual void*	MapInternal() = 0;
			virtual void	UnmapInternal() = 0;

		private:
		};
	}
}


#endif // !CROISSANT_ENGINE_CORE_ABSTRACTBUFFER_HPP_INC