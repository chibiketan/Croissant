//
// Copyright (C) 2007, Thiago Adams
//
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
//
// Source : http://www.thradams.com/codeblog/Stopwatch.htm
//

#ifndef STOPWATCHPERFORMANCECOUNTER_H_
#define STOPWATCHPERFORMANCECOUNTER_H_
#pragma once

#include <chrono>

namespace Croissant
{
	namespace Core
	{
		class StopwatchPerformanceCounter
		{
		public:
			typedef std::chrono::high_resolution_clock Clock;
			typedef Clock::time_point Time;
			typedef std::chrono::milliseconds DurationMillisecond;

			StopwatchPerformanceCounter();
			StopwatchPerformanceCounter(bool start);
			void Reset();
			void Start();
			void Stop();
			DurationMillisecond GetElapsedMilliseconds() const;
			bool IsRunning() const;

		private:

			bool m_isRunning;
			Time m_startCount;
			mutable Time m_stopCount;
		};
	}
}

#endif
