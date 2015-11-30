//
// Copyright (C) 2010, Thiago Adams
//
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
//
// source : http://www.thradams.com/codeblog/stopwatch2.htm
//

#ifndef STOPWATCHTIME_H_
#define STOPWATCHTIME_H_
#pragma once


namespace Croissant
{
	namespace Core
	{
		class StopwatchTime
		{

		public:
			StopwatchTime();
			StopwatchTime(bool start);
			static size_t GetFrequency(); //in milliseconds
			void Reset();
			void Start();
			void Stop();
			clock_t GetElapsedTicks() const;
			std::size_t GetElapsedMilliseconds() const;
			bool IsRunning() const;

		private:
			clock_t m_StartCount;
			clock_t m_StopCount;
		};
	}
}


#endif
