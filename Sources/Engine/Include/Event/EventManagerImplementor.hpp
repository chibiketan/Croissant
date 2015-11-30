#pragma once
#ifndef EVENTMANAGERIMPLEMENTOR_H_
#define EVENTMANAGERIMPLEMENTOR_H_

#include "Event/IEventManagerImplementor.hpp"
#include "Core/String.hpp"

using String = Croissant::Core::String;

namespace Croissant
{
	namespace Event
	{
		class Event;
		class EventManager;

		class EventManagerImplementor : public IEventManagerImplementor
		{
		public:
			EventManagerImplementor(Croissant::Event::EventManager& eventManager);
			virtual ~EventManagerImplementor();

			virtual void AddEventAtStartFrameLevel( std::unique_ptr<Croissant::Event::Event> event);
			virtual void AddEventAtUserInputLevel( std::unique_ptr<Croissant::Event::Event> event);
			virtual void AddEventAtInteractionLevel( std::unique_ptr<Croissant::Event::Event> event);
			virtual void AddEventAtEndFrameLevel( std::unique_ptr<Croissant::Event::Event> event);
			virtual void AddEventAtRenderLevel( std::unique_ptr<Croissant::Event::Event> event);
			virtual void RegisterListener( ListenerDelegate listener );
			virtual void RegisterListener( const String& category, ListenerDelegate listener );
			virtual void UnregisterListener(ListenerDelegate listener);
			virtual void UnregisterListener(const String& category, ListenerDelegate listener);

		private:
			Croissant::Event::EventManager& m_eventManager;
		};
	}
}

#endif
