#pragma once
#ifndef FRAMESTARTEVENT_H_
#define FRAMESTARTEVENT_H_

#include "Event/Event.hpp"
#include "Core/String.hpp"

using String = Croissant::Core::String;

namespace Croissant
{
	namespace Event
	{
		class FrameStartEvent : public Croissant::Event::Event
		{
		public:
			FrameStartEvent();
			~FrameStartEvent();

		private:
			static const String s_eventName;
		};
	}
}

#endif
