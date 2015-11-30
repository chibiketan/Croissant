#pragma once
#ifndef EVENTMANAGER_H_
#define EVENTMANAGER_H_

#if defined(_WIN32) || defined(_WIN64)
#  include <Windows.h>
#endif

#include <memory>
#include <cstdint>

#include "Engine.hpp"
#include "Event/Event.hpp"
#include "Event/IEventListener.hpp"
#include "Core/NonCopyable.hpp"
#include "Core/Delegate.hpp"
#include "Core/String.hpp"

using String = Croissant::Core::String;

#define GENERIC_EVENT_TYPE ""

namespace Croissant
{
	namespace Event
	{
		enum class EventPriority
		{
			START_FRAME = 0,
			USER_ACTION = 1,
			INTERACTION = 2,
			END_FRAME = 3,
			RENDER = 4
		};

		typedef Croissant::Core::Delegate<std::function<void (Event*)>> ListenerDelegate;

		class ENGINE_API EventManager : private Croissant::Core::NonCopyable
		{
		public:

			EventManager();
			~EventManager();
			std::uint32_t GetEventCount() const;
			void AddEvent(std::unique_ptr<Event> event, EventPriority priority);
			void RegisterListener(ListenerDelegate listener);
			void RegisterListener(const String& category, ListenerDelegate listener);
			void UnregisterListener(ListenerDelegate listener);
			void UnregisterListener(const String& category, ListenerDelegate listener);
			void ProcessEvents();

		private:
			class EventManagerImpl;

			EventManagerImpl* m_impl;
		};
	}
}

#endif
