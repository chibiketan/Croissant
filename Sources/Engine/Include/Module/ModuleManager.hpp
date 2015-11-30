#pragma once
#ifndef MODULEMANAGER_H_
#define MODULEMANAGER_H_

#include "Engine.hpp"
#include "Core/NonCopyable.hpp"


namespace Croissant
{
	namespace Event
	{
		class EventManager;
	}

	namespace Module
	{
		class ServiceProvider;

		class ENGINE_API ModuleManager : private Croissant::Core::NonCopyable
		{
		public:
			ModuleManager();
			ModuleManager(ModuleManager&& source);
			virtual ~ModuleManager();
			bool Init(Croissant::Event::EventManager* eventManager, Croissant::Module::ServiceProvider& serviceProvder);

		private:
			class Pimpl;

			Pimpl* m_pimpl;
		};
	}
}

#endif
