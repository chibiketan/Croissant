#ifndef THREAD_HPP
#define THREAD_HPP

#ifdef CROISSANT_HAS_THREAD

#include "Engine.hpp"
#include <functional>
#include <memory>

namespace Croissant
{
	namespace Threading
	{
		class ENGINE_API Thread
		{
		public:
			using CallbackType = std::function<int ()>;

			Thread(CallbackType callback);
			~Thread();
			void Join();
			bool Join(uint32_t waitTime);
			void Terminate();
		private:
			class Pimpl;

			std::unique_ptr<Pimpl> m_pimpl;
		};
	}
}

#endif /* CROISSANT_HAS_THREAD */

#endif /* !THREAD_HPP_ */
