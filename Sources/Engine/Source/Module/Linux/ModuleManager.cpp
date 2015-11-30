#include "stdafx.hpp"
#include "Core/DebugMemoryManager.hpp"
#include "Module/ModuleManager.hpp"
#include "Event/Event.hpp"
#include "FileSystem/Directory.hpp"
#include "FileSystem/File.hpp"
#include <algorithm>
#include "Module/IModule.hpp"
#include "Event/EventManager.hpp"
#include "Event/IEventListener.hpp"
#include "Module/ModuleLoadingEvent.hpp"
#include "Module/ModuleLoadedEvent.hpp"
#include "Event/IEventManagerImplementor.hpp"
#include "Module/ServiceProvider.hpp"

#include <functional>
#include <dlfcn.h>

#define QU(x) #x
#define STRINGIFY(x) QU(x)
#define CROISSANT_MODULE_ENTRY_STR STRINGIFY(CROISSANT_MODULE_ENTRY)


namespace Croissant
{
	namespace Module
	{
		class LoadedModule : private Croissant::Core::NonCopyable
		{
		public:
			typedef std::list<std::unique_ptr<Croissant::Event::IEventListener>> EventListenerList;

			LoadedModule() : m_moduleHandle{nullptr}, m_moduleList{nullptr}, m_initListenerList{}
			{
			};
			LoadedModule(LoadedModule&& source)
				: m_moduleHandle(source.m_moduleHandle), m_moduleList(std::move(source.m_moduleList))
				  , m_initListenerList(std::move(source.m_initListenerList))
			{
			};


			void* m_moduleHandle;
			std::unique_ptr<ModuleList> m_moduleList;
			EventListenerList m_initListenerList;
		};

		template<typename Func>
		class LoadModuleListener : public Croissant::Event::IEventListener
		{
		public:
			LoadModuleListener(Func function)
			{
				m_function = function;
			}

			virtual ~LoadModuleListener()
			{
			}

			virtual void ProcessEvent(Croissant::Event::Event* evt)
			{
				m_function(evt);
			}

		private:
			Func m_function;
		};

		class ModuleManager::Pimpl : Croissant::Core::NonCopyable
		{
		public:
			typedef std::list<Croissant::FileSystem::Directory> DirectoryList;
			typedef std::list<LoadedModule> LoadedModuleList;

			Pimpl();
			~Pimpl();
			void AddDirectory(Croissant::FileSystem::Directory&& directory);
			bool Init(Croissant::Event::EventManager* eventManager, Croissant::Module::ServiceProvider& serviceProvider);

		private:
			bool m_init;
			DirectoryList m_directoryList;
			LoadedModuleList m_loadedModuleList;

		};

		// -------------------------------- ModuleManager::Pimpl impl�mentation

		ModuleManager::Pimpl::Pimpl()
			: m_init{false}, m_directoryList{}
		{
			AddDirectory(Croissant::FileSystem::Directory(Croissant::FileSystem::Directory::PROGRAM_DIRECTORY).Child("Modules"));
		}

		ModuleManager::Pimpl::~Pimpl()
		{
			for (auto &module : m_loadedModuleList)
			{
				module.m_initListenerList.clear();
				for (auto &loadedModule : (*module.m_moduleList))
				{
					loadedModule->Cleanup();
				}

				module.m_moduleList->clear();
				dlclose(module.m_moduleHandle);
			}

			m_loadedModuleList.clear();
		}

		void ModuleManager::Pimpl::AddDirectory(Croissant::FileSystem::Directory&& directory)
		{
			auto end = m_directoryList.end();
			auto it = std::find(m_directoryList.begin(), end, directory);

			if (it == end)
			{
				m_directoryList.push_back(std::move(directory));
			}
		}

		//bool ModuleManager::Pimpl::Init(Croissant::Event::EventManager* serviceProvider)
		bool ModuleManager::Pimpl::Init(Croissant::Event::EventManager* eventManager, Croissant::Module::ServiceProvider& serviceProvider)
		{
			// parcourir la liste des dossiers d�fini � la recherche de fichier .dll
			// pour chaque dll, la charger et v�rifier si elle poss�de un point d'entr�e std::list<std::unique_ptr<IModule>> LoadModule
			// pour chaque module, ajouter un Event Module:Load
			for (auto &dir : m_directoryList)
			{
				if (!dir.Exist())
				{
					continue;
				}

				auto files = dir.Search("*.so");
				for (auto &file : files)
				{
					LoadedModule loadedModule;

					loadedModule.m_moduleHandle = dlopen(file.FullPath().Str(), RTLD_NOW);
					if (nullptr == loadedModule.m_moduleHandle)
					{
						// TODO logger la raison de l'erreur
						continue;
					}

					CroissantModuleEntry entry = reinterpret_cast<CroissantModuleEntry>(dlsym(loadedModule.m_moduleHandle, CROISSANT_MODULE_ENTRY_STR));

					if (nullptr == entry)
					{
						// no entry point known, skip
						// TODO : log
						dlclose(loadedModule.m_moduleHandle);
						continue;
					}

					loadedModule.m_moduleList = std::unique_ptr<ModuleList>(entry());

					if (nullptr == loadedModule.m_moduleList.get() || 0 == loadedModule.m_moduleList->size())
					{
						// no module loaded, skip
						// TODO : log
						dlclose(loadedModule.m_moduleHandle);
						continue;
					}

					for (auto &module : (*loadedModule.m_moduleList))
					{
						auto evt = std::unique_ptr<ModuleLoadingEvent>(CROISSANT_NEW ModuleLoadingEvent(module->Name()));

						eventManager->RegisterListener(evt->Name(), [&module, &serviceProvider](Croissant::Event::Event* evt) -> void {
							if (((ModuleLoadingEvent*)evt)->ModuleName() == module->Name())
							{
								auto result = module->Init(serviceProvider);

								std::unique_ptr<Croissant::Event::IEventManagerImplementor> em { nullptr };
								serviceProvider.Resolve(em);
								//eventManager->AddEvent(std::unique_ptr<ModuleLoadedEvent>(CROISSANT_NEW ModuleLoadedEvent(module->Name(), result)), Croissant::Event::EventPriority::INTERACTION);
								em->AddEventAtInteractionLevel(std::unique_ptr<ModuleLoadedEvent>(CROISSANT_NEW ModuleLoadedEvent(module->Name(), result)));
							}
						});
						eventManager->AddEvent(std::move(evt), Croissant::Event::EventPriority::INTERACTION);
					}

					m_loadedModuleList.push_back(std::move(loadedModule));
				}
			}

			return true;
		}

		// -------------------------------- ModuleManager impl�mentation

		ModuleManager::ModuleManager()
			: m_pimpl(nullptr)
		{
			m_pimpl = CROISSANT_NEW ModuleManager::Pimpl();
		}

		ModuleManager::ModuleManager(ModuleManager&& source)
			: m_pimpl(source.m_pimpl)
		{
			source.m_pimpl = nullptr;
		}

		ModuleManager::~ModuleManager()
		{
			if (nullptr != m_pimpl)
			{
				CROISSANT_DELETE(m_pimpl);
				m_pimpl = nullptr;
			}
		}

		bool ModuleManager::Init(Croissant::Event::EventManager* eventManager, Croissant::Module::ServiceProvider& serviceProvider)
		{
			return m_pimpl->Init(eventManager, serviceProvider);
		}

	}
}
