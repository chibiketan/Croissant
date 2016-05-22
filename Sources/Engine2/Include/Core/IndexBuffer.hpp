#ifndef CROISSANT_ENGINE_CORE_INDEXBUFFER_HPP_INC
#  define CROISSANT_ENGINE_CORE_INDEXBUFFER_HPP_INC
#  pragma once

#  include "Engine.hpp"
#  include "Core/Enums.hpp"
#  include <memory>

namespace Croissant
{
	namespace Core
	{
		class AbstractBuffer;

		class ENGINE_API IndexBuffer final
		{
		public:
			IndexBuffer(std::shared_ptr<AbstractBuffer> internalBuffer);
			void*	Map(BufferAccessEnum access) const;
			void	Unmap() const;
			uint32_t	GetBufferId() const;

		private:
			std::shared_ptr<AbstractBuffer> m_internalBuffer;
		};


	}
}

#endif // !CROISSANT_ENGINE_CORE_INDEXBUFFER_HPP_INC