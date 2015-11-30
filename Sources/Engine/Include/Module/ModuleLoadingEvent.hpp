#pragma once
#ifndef MODULELOADINGEVENT_H_
#define MODULELOADINGEVENT_H_

#include "Event/Event.hpp"
#include "Core/String.hpp"

using String = Croissant::Core::String;

namespace Croissant
{
	namespace Module
	{
		class ModuleLoadingEvent : public Croissant::Event::Event
		{
		public:
			ModuleLoadingEvent(const String& moduleName);
			virtual ~ModuleLoadingEvent();
			virtual String ToString() const;
			const String ModuleName() const;

		private:
			String m_moduleName;
		};
	}
}

#endif
