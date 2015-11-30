#include "stdafx.hpp"
#include "Event/EventManagerImplementor.hpp"
#include "Event/EventManager.hpp"
#include "Event/Event.hpp"

namespace Croissant
{
	namespace Event
	{
		EventManagerImplementor::EventManagerImplementor(Croissant::Event::EventManager& eventManager)
			: m_eventManager(eventManager)
		{
		}

		void EventManagerImplementor::AddEventAtStartFrameLevel( std::unique_ptr<Croissant::Event::Event> event) 
		{
			m_eventManager.AddEvent(std::move(event), Croissant::Event::EventPriority::START_FRAME);
		}

		void EventManagerImplementor::AddEventAtUserInputLevel( std::unique_ptr<Croissant::Event::Event> event) 
		{
			m_eventManager.AddEvent(std::move(event), Croissant::Event::EventPriority::USER_ACTION);
		}

		void EventManagerImplementor::AddEventAtInteractionLevel( std::unique_ptr<Croissant::Event::Event> event) 
		{
			m_eventManager.AddEvent(std::move(event), Croissant::Event::EventPriority::INTERACTION);
		}

		void EventManagerImplementor::AddEventAtEndFrameLevel( std::unique_ptr<Croissant::Event::Event> event) 
		{
			m_eventManager.AddEvent(std::move(event), Croissant::Event::EventPriority::END_FRAME);
		}

		void EventManagerImplementor::AddEventAtRenderLevel( std::unique_ptr<Croissant::Event::Event> event) 
		{
			m_eventManager.AddEvent(std::move(event), Croissant::Event::EventPriority::RENDER);
		}

		void EventManagerImplementor::RegisterListener(ListenerDelegate listener)
		{
			m_eventManager.RegisterListener(listener);
		}

		void EventManagerImplementor::RegisterListener(const String& category, ListenerDelegate listener)
		{
			m_eventManager.RegisterListener(category, listener);
		}

		void EventManagerImplementor::UnregisterListener(ListenerDelegate listener)
		{
			m_eventManager.UnregisterListener(listener);
		}

		void EventManagerImplementor::UnregisterListener(const String& category, ListenerDelegate listener)
		{
			m_eventManager.UnregisterListener(category, listener);
		}

		EventManagerImplementor::~EventManagerImplementor()
		{

		}

	}
}
