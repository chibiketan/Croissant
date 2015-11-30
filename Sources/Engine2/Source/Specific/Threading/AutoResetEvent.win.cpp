#if defined(CROISSANT_THREAD_WIN)
#include <windows.h>

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
				HANDLE m_event;
			};

			AutoResetEventImpl::AutoResetEventImpl()
				: m_event { nullptr }
			{
				m_event = CreateEvent(nullptr, FALSE, FALSE, nullptr);
				if (nullptr == m_event)
				{
					throw "Impossible de créer l'événement de synchronisation";
				}
			}

			AutoResetEventImpl::~AutoResetEventImpl()
			{
				if (nullptr != m_event)
				{
					CloseHandle(m_event);
					m_event = nullptr;
				}
			}

			void AutoResetEventImpl::Wait()
			{
				WaitForSingleObject(m_event, INFINITE);
			}

			void AutoResetEventImpl::Wait(uint32_t milliseconds)
			{
				WaitForSingleObject(m_event, milliseconds);
			}

			void AutoResetEventImpl::Signal()
			{
				SetEvent(m_event);
			}
		}
	}
}


#endif // CROISSANT_THREAD_WIN
