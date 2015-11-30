#pragma once
#ifndef MODULELOADEDEVENT_H_
#define MODULELOADEDEVENT_H_

#include "Event/Event.hpp"
#include "Core/String.hpp"

using String = Croissant::Core::String;

namespace Croissant
{
	namespace Module
	{
		class ModuleLoadedEvent : public Croissant::Event::Event
		{
		public:
			ModuleLoadedEvent(const String& moduleName, bool success);
			virtual ~ModuleLoadedEvent();
			virtual String ToString() const;
			const String ModuleName() const;
			bool Success() const;

		private:
			String m_moduleName;
			bool m_success;
		};
	}
}

#endif
