#ifndef AUTORESETEVENT_HPP_
#define AUTORESETEVENT_HPP_

#if defined(CROISSANT_HAS_THREAD)

#include "Engine.hpp"
#include <cstdint>

namespace Croissant
{
	namespace Threading
	{
		class ENGINE_API AutoResetEvent
		{
		public:
			AutoResetEvent();
			~AutoResetEvent();
			void Wait();
			void Wait(uint32_t milliseconds);
			void Signal();

			AutoResetEvent(const AutoResetEvent&) = delete;
			AutoResetEvent(AutoResetEvent&&) = delete;
			AutoResetEvent& operator=(const AutoResetEvent&) = delete;
			AutoResetEvent& operator=(AutoResetEvent&&) = delete;
		private:
			class Pimpl;

			Pimpl* m_pimpl;
		};
	}
}


#endif // CROISSANT_HAS_THREAD

#endif /* AUTORESETEVENT_HPP_ */
