#include "stdafx.hpp"
#include "Event/FrameStartEvent.hpp"

namespace Croissant
{
	namespace Event
	{
		const String FrameStartEvent::s_eventName = "Frame::Start";


		FrameStartEvent::FrameStartEvent()
			: Croissant::Event::Event(s_eventName)
		{

		}

		FrameStartEvent::~FrameStartEvent()
		{

		}


	}
}
