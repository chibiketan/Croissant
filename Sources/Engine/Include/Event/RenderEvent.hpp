#pragma once
#ifndef RENDEREVENT_H_
#define RENDEREVENT_H_

#include "Event/Event.hpp"
#include "Core/String.hpp"

using String = Croissant::Core::String;

namespace Croissant
{
	namespace Event
	{
		class RenderEvent : public Croissant::Event::Event
		{
		public:
			RenderEvent();
			virtual ~RenderEvent();

		private:
			static const String s_eventName;
		};
	}
}

#endif
