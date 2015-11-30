#pragma once
#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "Engine.hpp"
#include "Core/NonCopyable.hpp"
#include "Core/Pimpl.hpp"
#include <memory>

namespace Croissant
{
	namespace Event
	{
		class EventManager;
	}

	namespace Module
	{
		class ModuleManager;
	}

	namespace Scene
	{
		class SceneManager;
	}

	namespace Core
	{

		class ENGINE_API Application final : private NonCopyable, public Pimpl<Application>::Detail
		{
		public:
			Application(void);
			virtual ~Application(void);
			Croissant::Event::EventManager* EventManager() const;
			void SetEventManager(std::unique_ptr<Croissant::Event::EventManager> eventManager);
			Croissant::Module::ModuleManager* ModuleManager() const;
			Croissant::Scene::SceneManager* SceneManager() const;
			void SetSceneManager(std::unique_ptr<Croissant::Scene::SceneManager> sceneManager);
			bool Init();
			void Shutdown();
			int Run();
			void Quit();
		};

	}
}

#endif

