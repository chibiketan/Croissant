#ifndef CROISSANT_ENGINE_CORE_INDEXBUFFER_HPP_INC
#  define CROISSANT_ENGINE_CORE_INDEXBUFFER_HPP_INC

#  include "Engine.hpp"
#  include <memory>

namespace Croissant
{
	namespace Core
	{
		class AbstractBuffer;

		class ENGINE_API IndexBuffer final
		{
		public:
			IndexBuffer(std::unique_ptr<AbstractBuffer>&& internalBuffer);

		private:
			std::unique_ptr<AbstractBuffer> m_internalBuffer;
		};
	}
}

#endif // !CROISSANT_ENGINE_CORE_INDEXBUFFER_HPP_INC