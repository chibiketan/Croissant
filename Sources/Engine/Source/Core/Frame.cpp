#include "Core/Frame.hpp"
#include <memory>

namespace Croissant
{
	namespace Core
	{
		Frame::Frame()
			: m_time { clock::now() }, m_lastFrameTime { m_time }, m_elapsedTime { m_lastFrameTime - m_time }
			, m_firstFrame { true }, m_firstFrameTime { m_time }
		{
		}

		Frame::Frame(const Frame& right)
			: m_time { clock::now() }, m_lastFrameTime { right.m_time }, m_elapsedTime { m_time - m_lastFrameTime }
			, m_firstFrame { (Frame::durationSecond { m_time - right.m_firstFrameTime }.count() >= 1.0f ? true : false) }
			, m_firstFrameTime { (Frame::durationSecond { m_time - right.m_firstFrameTime }.count() >= 1.0f ? m_time : right.m_firstFrameTime) }
		{
		}

		Frame::Frame(Frame&& right)
			: m_time { std::move(right.m_time) }, m_lastFrameTime { std::move(right.m_lastFrameTime) }, m_elapsedTime { std::move(right.m_elapsedTime) }
			, m_firstFrame { std::move(right.m_firstFrame) }, m_firstFrameTime { std::move(right.m_firstFrameTime) }
		{
		}

		Frame& Frame::operator=(Frame&& right)
		{
			m_time = std::move(right.m_time);
			m_elapsedTime = std::move(right.m_elapsedTime);
			m_lastFrameTime = std::move(right.m_lastFrameTime);
			m_firstFrame = std::move(right.m_firstFrame);
			m_firstFrameTime = std::move(right.m_firstFrameTime);

			return *this;
		}

		const Frame::time& Frame::Time() const
		{
			return m_time;
		}

		const Frame::time& Frame::PreviousFrameTime() const
		{
			return m_lastFrameTime;
		}

		const Frame::durationSecond& Frame::Duration() const
		{
			return m_elapsedTime;
		}

		bool Frame::IsFirstFrame() const
		{
			return m_firstFrame;
		}

		const Frame::time& Frame::FirstFrameTime() const
		{
			return m_firstFrameTime;
		}

	}  // namespace Core
}  // namespace Croissant

