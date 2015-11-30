#include "stdafx.hpp"
#include "Core/StopWatchTime.hpp"

namespace Croissant
{
	namespace Core
	{
		StopwatchTime::StopwatchTime()
		{
			Reset();
		}

		StopwatchTime::StopwatchTime( bool start )
		{
			Reset();
			if (start)
				Start();
		}

		size_t StopwatchTime::GetFrequency() /*in milliseconds */
		{
			//static_assert(CLOCKS_PER_SEC >= 1000);
			return CLOCKS_PER_SEC / 1000;
		}

		void StopwatchTime::Reset()
		{
			m_StopCount = 0;
			m_StartCount = 0;

			assert(!IsRunning());
			assert(GetElapsedTicks() == 0);
		}

		void StopwatchTime::Start()
		{
			if (IsRunning())
				return;

			const bool resume = (m_StartCount != 0);
			if (resume)
				m_StopCount = 0;
			else
			{
				m_StartCount = clock();
			}

			assert(IsRunning());
		}

		void StopwatchTime::Stop()
		{
			if (!IsRunning())
				return;

			m_StopCount = clock();

			assert(!IsRunning());
		}

		clock_t StopwatchTime::GetElapsedTicks() const
		{
			if (IsRunning())
				return (clock() - m_StartCount);

			return (m_StopCount - m_StartCount);
		}

		std::size_t StopwatchTime::GetElapsedMilliseconds() const
		{
			return GetElapsedTicks() / GetFrequency();
		}

		bool StopwatchTime::IsRunning() const
		{
			return m_StartCount != 0 && m_StopCount == 0;
		}

	}
}


