#pragma once
#ifndef PROCESSUSERINPUTEVENT_H_
#define PROCESSUSERINPUTEVENT_H_

#include "Event/Event.hpp"

namespace Croissant
{
	namespace Event
	{
		class ENGINE_API ProcessUserInputEvent : public Croissant::Event::Event
		{
		public:
			ProcessUserInputEvent();
			virtual ~ProcessUserInputEvent();

		private:
			class Pimpl;

			Pimpl* m_pimpl;
		};
	}
}

#endif
