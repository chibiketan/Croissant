#if defined(CROISSANT_THREAD_WIN)

#include "Debug/MemoryManager.hpp"
#include "Threading/Thread.hpp"
#include <windows.h>

namespace Croissant
{
	namespace Threading
	{
		using threadEmptyPoint = unsigned long int (void*);

		class ThreadInternal final
		{
		public:
			ThreadInternal() noexcept;
			ThreadInternal(threadEmptyPoint* entryPoint, Thread::CallbackType* callback);
			ThreadInternal(const ThreadInternal&) = delete;
			ThreadInternal(ThreadInternal&&) = delete;
			~ThreadInternal() noexcept;
			ThreadInternal& operator=(const ThreadInternal&) = delete;
			ThreadInternal& operator=(ThreadInternal&&) = delete;

			void Join() noexcept;
			bool Join(uint32_t waitTime) noexcept;
			void Terminate() noexcept;

		private:
			DWORD m_id;
			HANDLE m_handle;
		};

		ThreadInternal::ThreadInternal() noexcept
			: m_id { 0 }, m_handle { nullptr }
		{
		}

		ThreadInternal::ThreadInternal(threadEmptyPoint* entryPoint, Thread::CallbackType* callback)
			: m_id { 0 }, m_handle { nullptr }
		{
			m_handle = CreateThread(nullptr, 0, entryPoint, callback, 0, &m_id);
			if (nullptr == m_handle)
			{
				// TODO throw proper Exception
				throw "Impossible de créer un thread";
			}
				//CreateThread()
		}

		ThreadInternal::~ThreadInternal() noexcept
		{
			if (nullptr != m_handle)
			{
				Terminate();
			}
		}

		void ThreadInternal::Join() noexcept
		{
			if (m_handle)
			{
				WaitForSingleObject(m_handle, INFINITE);
				CloseHandle(m_handle);
				m_handle = nullptr;
				m_id = 0;
			}
		}

		bool ThreadInternal::Join(uint32_t waitTime) noexcept
		{
			if (m_handle)
			{
				auto ret = WaitForSingleObject(m_handle, waitTime);

				if (ret == WAIT_TIMEOUT)
				{
					return false;
				}

				CloseHandle(m_handle);
				m_handle = nullptr;
				m_id = 0;
			}

			return true;
		}

		void ThreadInternal::Terminate() noexcept
		{
			if (nullptr != m_handle)
			{
				TerminateThread(m_handle, 0);
				CloseHandle(m_handle);
				m_handle = nullptr;
				m_id = 0;
			}
		}
	}
}



#endif /* defined(CROISSANT_THREAD_WIN) */
