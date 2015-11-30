#pragma once
#ifndef IEVENTLISTENER_H_
#define IEVENTLISTENER_H_

#include "Engine.hpp"
#include "Event/Event.hpp"

namespace Croissant
{
	namespace Event
	{
		class ENGINE_API IEventListener
		{
		public:
			virtual void ProcessEvent(Event* evt) = 0;
			virtual ~IEventListener() {}
		};
	}
}

#endif
