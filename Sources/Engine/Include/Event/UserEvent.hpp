#pragma once
#ifndef USEREVENT_H_
#define USEREVENT_H_

#include "Event/Event.hpp"

namespace Croissant
{
	namespace Event
	{
		class ENGINE_API UserEvent : public Croissant::Event::Event
		{
		public:
			enum UserEventKind
			{
				UserQuit
			};

			UserEvent(UserEventKind kind);
			~UserEvent();
			UserEventKind Kind() const;

		private:
			class Pimpl;

			Pimpl* m_pimpl;
		};
	}
}

#endif
