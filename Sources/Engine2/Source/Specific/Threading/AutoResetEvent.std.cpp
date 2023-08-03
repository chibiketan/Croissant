#if defined(CROISSANT_THREAD_STD)

#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
namespace Croissant
{
	namespace Threading
	{
		namespace
		{
			class AutoResetEventImpl
			{
			public:
				AutoResetEventImpl();
				~AutoResetEventImpl();

				void Wait();
				void Wait(uint32_t milliseconds);
				void Signal();

				AutoResetEventImpl(const AutoResetEventImpl&) = delete;
				AutoResetEventImpl(AutoResetEventImpl&&) = delete;
				AutoResetEventImpl& operator=(const AutoResetEventImpl&) = delete;
				AutoResetEventImpl& operator=(AutoResetEventImpl&&) = delete;

			private:
				std::mutex	m_mutex;
				std::condition_variable_any m_condition;
				//std::atomic_int m_lockCount;
				int m_lockCount;
			};

			AutoResetEventImpl::AutoResetEventImpl()
				: m_mutex {}, m_condition {}, m_lockCount { 0 }
			{
				//TRACE("AutoResetEventImpl: constructor");
			}

			AutoResetEventImpl::~AutoResetEventImpl()
			{
				//TRACE("AutoResetEventImpl: destructor : begin");
				//while (m_lockCount.load() > 0)
				while (m_lockCount > 0)
				{
					Signal();
				}
				//TRACE("AutoResetEventImpl: destructor : end");
			}

			void AutoResetEventImpl::Wait()
			{
				//TRACE("AutoResetEventImpl: Wait : begin");
				//++m_lockCount;
				//TRACE("&m_lockCount: " << (void*)&m_lockCount);
				//std::atomic_fetch_add(&m_lockCount, 1);
				std::unique_lock<std::mutex> l(m_mutex);
				++m_lockCount;

				m_condition.wait(l);
				//--m_lockCount;
				//std::atomic_fetch_add(&m_lockCount, -1);
				--m_lockCount;
				//TRACE("AutoResetEventImpl: Wait : end");
			}

			void AutoResetEventImpl::Wait(uint32_t milliseconds)
			{
				//TRACE("AutoResetEventImpl: Wait(milliseconds) : begin");
				//++m_lockCount;
				//std::atomic_fetch_add(&m_lockCount, 1);
				std::unique_lock<std::mutex> l(m_mutex);
				++m_lockCount;

				m_condition.wait_for(l, std::chrono::milliseconds(milliseconds));
				//--m_lockCount;
				//std::atomic_fetch_add(&m_lockCount, -1);
				--m_lockCount;
				//TRACE("AutoResetEventImpl: Wait(milliseconds) : end");
			}

			void AutoResetEventImpl::Signal()
			{
				//TRACE("AutoResetEventImpl: Signal : begin");
				m_condition.notify_all();
				//TRACE("AutoResetEventImpl: Signal : end");
			}
		}
	}
}


#endif // CROISSANT_THREAD_STD
