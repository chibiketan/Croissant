#if defined(CROISSANT_THREAD_STD)

#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

namespace Croissant
{
	namespace Threading
	{
		using threadEmptyPoint = unsigned long int (void*);

		class ManualResetEvent
		{
		public:
			ManualResetEvent();
			ManualResetEvent(bool signaled);

			void Set();
			void Reset();
			void Wait();
			std::cv_status Wait(uint32_t milliseconds);

			ManualResetEvent(const ManualResetEvent&) = delete;
			ManualResetEvent(ManualResetEvent&&) = delete;
			ManualResetEvent& operator=(const ManualResetEvent&) = delete;
			ManualResetEvent& operator=(ManualResetEvent&&) = delete;
		private:
			std::mutex	m_mutex;
			std::condition_variable_any m_condition;
			std::atomic_bool m_flag;
		};

		struct ThreadItem
		{
			ManualResetEvent		m_event;
			threadEmptyPoint*		m_entry;
			Thread::CallbackType*	m_callback;
		};

		namespace
		{
			unsigned long intStdThreadEntryPoint(ThreadItem* item)
			{
				auto ret = (*item->m_entry)(item->m_callback);

				item->m_event.Set();
				return ret;
			}
		}

		class ThreadInternal final
		{
		public:
			ThreadInternal();
			ThreadInternal(threadEmptyPoint* entryPoint, Thread::CallbackType* callback);
			~ThreadInternal();

			void Join();
			bool Join(uint32_t waitTime);
			void Terminate();

			ThreadInternal(const ThreadInternal&) = delete;
			ThreadInternal(ThreadInternal&&) = delete;
			ThreadInternal& operator=(const ThreadInternal&) = delete;
			ThreadInternal& operator=(ThreadInternal&&) = delete;

		private:
			ThreadItem m_item;
			std::thread m_thread;
		};

		// -------------------------------------------------------- ManualResetEvent
		ManualResetEvent::ManualResetEvent()
			: ManualResetEvent(false)
		{
		}

		ManualResetEvent::ManualResetEvent(bool signaled)
			: m_mutex {}, m_condition {}, m_flag { signaled }
		{
		}

		void ManualResetEvent::Set()
		{
//			if (m_flag.exchange(true) == false)
//			{
//				m_condition.notify_all();
//			}
			m_flag.store(true);
			m_condition.notify_all();
		}

		void ManualResetEvent::Reset()
		{
			m_flag.store(false);
		}

		void ManualResetEvent::Wait()
		{
			if (m_flag.load() == false)
			{
				std::unique_lock<std::mutex> l(m_mutex);

				m_condition.wait(l);
			}
		}

		std::cv_status ManualResetEvent::Wait(uint32_t milliseconds)
		{
			auto result = std::cv_status::no_timeout;

			if (m_flag.load() == false)
			{
				std::unique_lock<std::mutex> l(m_mutex);

				result = m_condition.wait_for(l, std::chrono::milliseconds(milliseconds));
			}

			return result;
		}

		// -------------------------------------------------------- ThreadInternal

		ThreadInternal::ThreadInternal()
			: m_item {{}, nullptr, nullptr}, m_thread {}
		{
			TRACE("ThreadInternal: constructor 1 : begin");
		}

		ThreadInternal::ThreadInternal(threadEmptyPoint* entryPoint, Thread::CallbackType* callback)
			: m_item {{}, entryPoint, callback}, m_thread { intStdThreadEntryPoint, &m_item }
		{
			TRACE("ThreadInternal: constructor 2 : begin");
		}

		ThreadInternal::~ThreadInternal()
		{
			TRACE("ThreadInternal: destructor : begin");
			if (m_thread.joinable())
			{
				m_thread.join();
			}
			TRACE("ThreadInternal: destructor : end");
		}

		void ThreadInternal::Join()
		{
			TRACE("ThreadInternal: Join : begin");
			if (m_thread.joinable())
			{
				m_thread.join();
			}
			TRACE("ThreadInternal: Join : end");
		}

		bool ThreadInternal::Join(uint32_t waitTime)
		{
			TRACE("ThreadInternal: Join(waitTime) : begin");
			if (m_thread.joinable())
			{
				if (m_item.m_event.Wait(waitTime) == std::cv_status::timeout)
				{
					TRACE("ThreadInternal: Join(waitTime) : end timeout");
					return false;
				}

				Join(); // call join to set the method joinable() to false
			}

			TRACE("ThreadInternal: Join(waitTime) : end");
			return true;
		}

		void ThreadInternal::Terminate()
		{
			TRACE("ThreadInternal: Terminate : begin");
			// nothing can be done
		}
	}
}

#endif /* defined(CROISSANT_THREAD_STD) */
