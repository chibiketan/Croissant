#include "stdafx.hpp"
#include "Event/RenderEvent.hpp"

namespace Croissant
{
	namespace Event
	{
		const String RenderEvent::s_eventName = "Frame::Render";

		RenderEvent::RenderEvent()
			: Croissant::Event::Event(s_eventName)
		{
		}

		RenderEvent::~RenderEvent()
		{
		}
	}
}
