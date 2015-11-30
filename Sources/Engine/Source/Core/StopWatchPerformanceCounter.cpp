#include "stdafx.hpp"
#include "Core/StopWatchPerformanceCounter.hpp"

#include <stdexcept>

#if defined(__WIN32) || defined(__WIN64)
#include <windows.h>
#endif
#include <cassert>
#include <ostream>

namespace Croissant
{
	namespace Core
	{
		StopwatchPerformanceCounter::StopwatchPerformanceCounter()
			: StopwatchPerformanceCounter(false)
		{
		}

		StopwatchPerformanceCounter::StopwatchPerformanceCounter(bool start)
		{
			Reset();
			if (start)
			{
				Start();
			}
		}

		void StopwatchPerformanceCounter::Reset()
		{
			m_stopCount = Clock::now();
			m_startCount = m_stopCount;
			m_isRunning = false;

			assert(!IsRunning());
			assert(GetElapsedMilliseconds().count() == 0);
		}

		void StopwatchPerformanceCounter::Start()
		{
			if (IsRunning())
			{
				return;
			}

			m_isRunning = true;

			const bool resume = m_startCount == m_stopCount;
			if (resume)
			{
			}
			else
			{
				m_startCount = Clock::now();
			}

			assert(IsRunning());
		}

		void StopwatchPerformanceCounter::Stop()
		{
			if (!IsRunning())
			{
				return;
			}

			m_isRunning = false;
			m_stopCount = Clock::now();
			assert(!IsRunning());
		}

		StopwatchPerformanceCounter::DurationMillisecond StopwatchPerformanceCounter::GetElapsedMilliseconds() const
		{
			if (IsRunning())
			{
				m_stopCount = Clock::now();
			}

			return std::chrono::duration_cast<DurationMillisecond>(m_stopCount - m_startCount);
		}

		bool StopwatchPerformanceCounter::IsRunning() const
		{
			return m_isRunning;
		}

	}
}
