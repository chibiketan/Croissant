#pragma once
#ifndef EVENT_H_
#define EVENT_H_

#include "Engine.hpp"
#include "Core/String.hpp"

using String = Croissant::Core::String;

namespace Croissant
{
	namespace Event
	{
		class ENGINE_API Event
		{
		public:
			virtual ~Event();
			const String& Name() const;
			virtual String ToString() const;

			Event(const Event&) = default;
			Event(Event&&) = default;
			Event& operator=(const Event&) = default;
			Event& operator=(Event&&) = default;

		protected:
			Event(const String&& type);
			Event(const String& type);

		private:
			class Pimpl;

			Pimpl* m_pimpl;
		};
	}
}

#endif
