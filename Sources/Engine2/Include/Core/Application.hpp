#ifndef CROISSANT_ENGINE_CORE_APPLICATION_HPP_INC
#  define CROISSANT_ENGINE_CORE_APPLICATION_HPP_INC

#  include "Engine.hpp"
#  include "Core/LogManager.hpp"
#  include <string>
#  include <memory>


#  define CROISSANT_DEFINE_HAS_METHOD(methodName, methodReturn, ...) template<typename T>\
struct HasMethod##methodName\
{\
	template<typename U, methodReturn (U::*)(__VA_ARGS__)> struct SFINAE { };\
	template<typename U> static int8_t Test(SFINAE<U, &U::methodName>*);\
	template<typename U> static int16_t Test(...);\
	static const bool Has = sizeof(Test<T>(nullptr)) == sizeof(int8_t);\
};\
\
template<typename TType, bool val = HasMethod##methodName<TType>::Has>\
struct Function##methodName\
{\
			template<typename... Args> static methodReturn Execute(TType&, Args&&... args);\
};\
\
template<typename TType>\
struct Function##methodName<TType, false>\
{\
	template<typename... Args> static methodReturn Execute(TType&, Args&&... args)\
	{\
		static_assert(HasMethod##methodName<TType>::Has, "La classe de base doit implementer une methode " #methodReturn " " #methodName "(" #__VA_ARGS__ ");");\
		return methodReturn{};\
	}\
};\
\
template<typename TType>\
struct Function##methodName<TType, true>\
{\
	template<typename... Args> static methodReturn Execute(TType& ref, Args&&... args)\
	{\
		return ref.methodName(std::forward<Args>(args)...);\
	}\
};



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

		//class ENGINE_API Application final
		//{
		//public:
		//	Application(Application const&) = delete;
		//	Application(Application&&) = delete;
		//	Application& operator=(Application const&) = delete;
		//	Application& operator=(Application&&) = delete;

		//	Application(void);
		//	~Application(void);
		//	LogManager& GetLogManager();
		//	//Croissant::Event::EventManager* EventManager() const;
		//	//void SetEventManager(std::unique_ptr<Croissant::Event::EventManager> eventManager);
		//	//Croissant::Module::ModuleManager* ModuleManager() const;
		//	//Croissant::Scene::SceneManager* SceneManager() const;
		//	//void SetSceneManager(std::unique_ptr<Croissant::Scene::SceneManager> sceneManager);
		//	bool Init();
		//	void Shutdown();
		//	int Run();
		//	void Quit();

		//private:
		//	class Impl;

		//	std::unique_ptr<Impl> m_impl;
		//};

		using FrameTime = std::chrono::high_resolution_clock::time_point;

		template<typename App>
		class Application : public App
		{
		public:
			using Clock = std::chrono::high_resolution_clock;

			explicit Application(std::string const& name);
			Application(Application const&) = delete;
			bool	Initialize();
			void	Shutdown();
			int32_t	Run();

		private:
			CROISSANT_DEFINE_HAS_METHOD(OnShutdown, bool, Application<App>&);
			CROISSANT_DEFINE_HAS_METHOD(OnInitialize, bool, Application<App>&);
			CROISSANT_DEFINE_HAS_METHOD(OnFrame, bool, int32_t&, FrameTime const&, FrameTime const&);

			std::string	m_name;
			LogManager::Log	m_logManager;
			FrameTime								m_lastFrameTime;

		};


	}
}

#include "Core/Application.inl"

#endif // !CROISSANT_ENGINE_CORE_APPLICATION_HPP_INC

