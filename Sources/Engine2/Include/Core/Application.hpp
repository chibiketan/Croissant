#ifndef APPLICATION_HPP_INC
#define APPLICATION_HPP_INC

#include "Engine.hpp"
#include <memory>

namespace Croissant
{
//	namespace Event
//	{
//		class EventManager;
//	}

//	namespace Module
//	{
//		//class ModuleManager;
//	class ServiceProviderInternal;
//	class ServiceProvider;
//	}

//	namespace Scene
//	{
//		class SceneManager;
//	}

	namespace Core
	{
		class LogManager;

		class ENGINE_API Application final
		{
		public:
			Application(Application const&) = delete;
			Application(Application&&) = delete;
			Application& operator=(Application const&) = delete;
			Application& operator=(Application&&) = delete;

			Application(void);
			~Application(void);
			LogManager& GetLogManager();
			//Croissant::Event::EventManager* EventManager() const;
			//void SetEventManager(std::unique_ptr<Croissant::Event::EventManager> eventManager);
			//Croissant::Module::ModuleManager* ModuleManager() const;
			//Croissant::Scene::SceneManager* SceneManager() const;
			//void SetSceneManager(std::unique_ptr<Croissant::Scene::SceneManager> sceneManager);
			bool Init();
			void Shutdown();
			int Run();
			void Quit();

		private:
			class Impl;

			std::unique_ptr<Impl> m_impl;
		};

	}
}

#endif

