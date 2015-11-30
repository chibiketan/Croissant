#pragma once
#ifndef FRAMEENDEVENT_H_
#define FRAMEENDEVENT_H_

#include "Event/Event.hpp"
#include "Core/String.hpp"

using String = Croissant::Core::String;

namespace Croissant
{
	namespace Event
	{
		class FrameEndEvent : public Croissant::Event::Event
		{
		public:
			FrameEndEvent();
			~FrameEndEvent();

		private:
			static const String s_eventName;
		};
	}
}

#endif
