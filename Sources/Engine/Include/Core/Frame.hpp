#pragma once

#ifndef FRAME_H_
#define FRAME_H_

#include "Engine.hpp"
#include <chrono>

namespace Croissant
{
	namespace Core
	{

		class ENGINE_API Frame
		{
		public:
			typedef std::chrono::high_resolution_clock clock;
			typedef clock::time_point time;
			typedef std::chrono::duration<float> durationSecond;

			Frame();
			explicit Frame(const Frame& right);
			Frame(Frame&& right);
			Frame& operator=(const Frame& right) = delete;
			Frame& operator=(Frame&& right);
			const time& Time() const;
			const time& PreviousFrameTime() const;
			const durationSecond& Duration() const;
			bool IsFirstFrame() const;
			const time& FirstFrameTime() const;

		private:
			time m_time;
			time m_lastFrameTime;
			durationSecond m_elapsedTime;
			bool m_firstFrame;
			time m_firstFrameTime;
		};
	}  // namespace Core
}  // namespace Croissant

#endif /* FRAME_H_ */
