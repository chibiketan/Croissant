#include "stdafx.hpp"
#include "Event/FrameEndEvent.hpp"

namespace Croissant
{
	namespace Event
	{


		const String FrameEndEvent::s_eventName = "Frame::End";


		FrameEndEvent::FrameEndEvent()
			: Croissant::Event::Event(s_eventName)
		{

		}

		FrameEndEvent::~FrameEndEvent()
		{

		}

	}
}
