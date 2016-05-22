#include "Debug/MemoryManager.hpp"
#include "Threading/AutoResetEvent.hpp"

#if defined(CROISSANT_THREAD_STD)
#  include "../Source/Specific/Threading/AutoResetEvent.std.cpp"
#elif defined(CROISSANT_THREAD_WIN)
#  include "../Source/Specific/Threading/AutoResetEvent.win.cpp"
#endif

namespace Croissant
{
	namespace Threading
	{
		class AutoResetEvent::Pimpl
		{
		public:
			Pimpl();
			void Wait();
			void Wait(uint32_t milliseconds);
			void Signal();

			Pimpl(const Pimpl&) = delete;
			Pimpl(Pimpl&&) = delete;
			Pimpl& operator=(const Pimpl&) = delete;
			Pimpl& operator=(Pimpl&&) = delete;

		private:
			AutoResetEventImpl m_impl;
		};

		// ---------------------------------------- AutoResetEvent:Pimpl
		AutoResetEvent::Pimpl::Pimpl()
			: m_impl {  }
		{
		}

		void AutoResetEvent::Pimpl::Wait()
		{
			m_impl.Wait();
		}

		void AutoResetEvent::Pimpl::Wait(uint32_t milliseconds)
		{
			m_impl.Wait(milliseconds);
		}

		void AutoResetEvent::Pimpl::Signal()
		{
			m_impl.Signal();
		}


		// ---------------------------------------- AutoResetEvent

		AutoResetEvent::AutoResetEvent()
			: m_pimpl { new Pimpl{} }
		{
		}

		AutoResetEvent::~AutoResetEvent()
		{
		}

		void AutoResetEvent::Wait()
		{
			m_pimpl->Wait();
		}

		void AutoResetEvent::Wait(uint32_t milliseconds)
		{
			m_pimpl->Wait(milliseconds);
		}

		void AutoResetEvent::Signal()
		{
			m_pimpl->Signal();
		}
	}
}

