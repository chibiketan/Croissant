#ifdef CROISSANT_HAS_THREAD

#include "Debug/MemoryManager.hpp"
#include "Threading/Thread.hpp"

#if defined(CROISSANT_THREAD_STD)
#  include "../Source/Specific/Threading/Thread.std.cpp"
#elif defined(CROISSANT_THREAD_WIN)
#  include "../Source/Specific/Threading/Thread.win.cpp"
#endif

namespace Croissant
{
	namespace Threading
	{
		namespace
		{
			unsigned long int threadMain(void* obj)
			{
				auto callback = reinterpret_cast<std::function<int ()>*>(obj);
				auto result = (*callback)();

				return result;
			}
		}

		class Thread::Pimpl
		{
		public:
			Pimpl();
			Pimpl(CallbackType callback);
			~Pimpl();

			void Join();
			bool Join(uint32_t waitTime);
			void Terminate();

			Pimpl(const Pimpl&) = delete;
			Pimpl(Pimpl&&) = delete;
			Pimpl& operator=(const Pimpl&) = delete;
			Pimpl& operator=(Pimpl&&) = delete;
		private:

			CallbackType m_callback;
			ThreadInternal m_thread;
		};

		// ------------------------------------------------------------- Thread::Pimpl
		Thread::Pimpl::Pimpl()
			:	m_thread {}
		{
			TRACE("Thread::Pimpl: Constructor 1 : call");
		}

		Thread::Pimpl::Pimpl(CallbackType callback)
			: m_callback { callback }, m_thread { &threadMain, &m_callback }
		{
			TRACE("Thread::Pimpl: Constructor 2 : call");
		}

		Thread::Pimpl::~Pimpl()
		{
			TRACE("Thread::Pimpl: Destructor : begin");
			// on laisse une minute au thread pour se terminer
			//if (!m_thread.Join(60000))
			if (!m_thread.Join(5000))
			{
				m_thread.Terminate();
			}
			TRACE("Thread::Pimpl: Destructor : end");
		}

		void Thread::Pimpl::Join()
		{
			TRACE("Thread::Pimpl: Join : begin");
			m_thread.Join();
			TRACE("Thread::Pimpl: Join : end");
		}

		bool Thread::Pimpl::Join(uint32_t waitTime)
		{
			TRACE("Thread::Pimpl: Join(waitTime) : begin");
			return m_thread.Join(waitTime);
			TRACE("Thread::Pimpl: Join(waitTime) : end");
		}

		void Thread::Pimpl::Terminate()
		{
			TRACE("Thread::Pimpl: Terminate : begin");
			m_thread.Terminate();
			TRACE("Thread::Pimpl: Terminate : end");
		}

		// ------------------------------------------------------------- Thread

		Thread::Thread(CallbackType callback)
			: m_pimpl { new Pimpl { callback } }
		{
		}

		Thread::~Thread()
		{
		}

		void Thread::Join()
		{
			m_pimpl->Join();
		}

		bool Thread::Join(uint32_t waitTime)
		{
			return m_pimpl->Join(waitTime);
		}

		void Thread::Terminate()
		{
			return m_pimpl->Terminate();
		}
	}
}



#endif /* CROISSANT_HAS_THREAD */
