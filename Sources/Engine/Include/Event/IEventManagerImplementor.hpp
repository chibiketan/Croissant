#pragma once
#ifndef IEVENTMANAGERIMPLEMENTOR
#define IEVENTMANAGERIMPLEMENTOR

#include "Core/NonCopyable.hpp"
#include "Core/Delegate.hpp"
#include <memory>
#include "Core/String.hpp"

using String = Croissant::Core::String;

namespace Croissant
{
	namespace Event
	{
		class IEventListener;
		class Event;

		typedef Croissant::Core::Delegate<std::function<void (Croissant::Event::Event*)>> ListenerDelegate;

		class ENGINE_API IEventManagerImplementor : public Croissant::Core::NonCopyable
		{
		public:
			virtual ~IEventManagerImplementor();
			static const String& GetTypeName();

			virtual void AddEventAtStartFrameLevel(std::unique_ptr<Croissant::Event::Event> event) = 0;
			virtual void AddEventAtUserInputLevel(std::unique_ptr<Croissant::Event::Event> event) = 0;
			virtual void AddEventAtInteractionLevel(std::unique_ptr<Croissant::Event::Event> event) = 0;
			virtual void AddEventAtEndFrameLevel(std::unique_ptr<Croissant::Event::Event> event) = 0;
			virtual void AddEventAtRenderLevel(std::unique_ptr<Croissant::Event::Event> event) = 0;
			virtual void RegisterListener(ListenerDelegate listener) = 0;
			virtual void RegisterListener(const String& category, ListenerDelegate listener) = 0;
			virtual void UnregisterListener(ListenerDelegate listener) = 0;
			virtual void UnregisterListener(const String& category, ListenerDelegate listener) = 0;

		private:
		};
	}
}

#endif
