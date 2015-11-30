#pragma once
#ifndef WINDOWSINPUTMODULE_H_
#define WINDOWSINPUTMODULE_H_

#include "Module/IModule.hpp"
#include "Core/Delegate.hpp"
#include <memory>

namespace Croissant
{
	namespace Event
	{
		class Event;
		class IEventManagerImplementor;
	}

	namespace Module
	{
		namespace OpenGL
		{
			class Window;

			class WindowsInputModule : public Croissant::Module::IModule
			{
			public:
				WindowsInputModule(std::shared_ptr<Window> window);
				virtual ~WindowsInputModule();
				virtual bool Init( Croissant::Module::ServiceProvider& serviceProvider );
				virtual void Cleanup();
				virtual const String& Name() const;
				virtual void ProcessEvent( Croissant::Event::Event* evt );

			private:
				std::shared_ptr<Window> m_window;
				static const String s_name;
				std::unique_ptr<Croissant::Event::IEventManagerImplementor> m_eventManager;
				Croissant::Core::Delegate<std::function<void (Croissant::Event::Event*)>> m_processUserInputDelegate;
			};
		}
	}
}

#endif
