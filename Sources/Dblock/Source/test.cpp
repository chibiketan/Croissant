//#include "Core/DebugMemoryManager.hpp"
//#include "Core/Application.hpp"
//#include "Event/IEventListener.hpp"
//#include "Event/EventManager.hpp"
//#include "Core/Logger.hpp"
//#include "Core/LoggerOutputToFile.hpp"
//#include "FileSystem/Directory.hpp"
//#include "FileSystem/File.hpp"

#include "Debug/MemoryManager.hpp"
#include "Core/Application.hpp"

#include "Graphic/Window.hpp"
#include "Graphic/WindowEvent.hpp"
#include <iostream>
#include <chrono>

//#include <iostream>

//class TraceEventListener : public Croissant::Event::IEventListener
//{
//public:
//	TraceEventListener() : m_logger()
//	{
//		Croissant::FileSystem::Directory appDir(Croissant::FileSystem::Directory::PROGRAM_DIRECTORY);
//		auto logDir = appDir.Child("Logs");
//		Croissant::FileSystem::File file { "Events.log", logDir };
//
//		logDir.Create();
//		m_logger.AddLoggerOutput(std::unique_ptr<Croissant::Core::ILoggerOutput>(CROISSANT_NEW Croissant::Core::LoggerOutputToFile(file.FullPath())));
//	}
//
//	virtual ~TraceEventListener()
//	{
//
//	}
//
//	virtual void ProcessEvent( Croissant::Event::Event* evt )
//	{
//		m_logger.WriteDebugLine("TraceEventListener", evt->ToString() + "\r\n");
//	}
//
//	Croissant::Core::Logger m_logger;
//};

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------

#include "Graphic/OpenGLRenderer.hpp"
#include "Graphic/OpenGLWrapper.hpp"

using Clock = std::chrono::high_resolution_clock;
using Time = Clock::time_point;
using durationSecond = std::chrono::duration<float>;
using durationNanoSecond = std::chrono::duration<uint32_t, std::micro>;

int main(int, char**)
{
	Croissant::Core::Application app;
	std::string const baseTitle("Ma fenetre - fps : ");

	if (!app.Init())
	{
		return EXIT_FAILURE;
	}


	try
	{
		Croissant::Graphic::Window win(640, 480, "test titre");
		Croissant::Graphic::OpenGLRenderer renderer(win, app.GetLogManager());
		auto opengl = renderer.GetOpenGLWrapper();

		win.Open();
		int fps = 0;
		Time firstFrameTime { Clock::now() };
		Time lastFrameTime { firstFrameTime };

		// --------------------------------------------------------------------------- begin initialisation
		auto vertexShaderContent = R"(

)";

		auto programId = opengl.CreateProgram();
		auto vertexShaderId = opengl.CreateShader(GL_VERTEX_SHADER);


		// --------------------------------------------------------------------------- end   initialisation

		while (1)
		{
			++fps;
			Time currentFrameTime { Clock::now() };
			durationSecond secondSinceFirstFrame { currentFrameTime - firstFrameTime };
			if (secondSinceFirstFrame.count() >= 1.0f) {
				durationNanoSecond nanoS = std::chrono::duration_cast<durationNanoSecond>(secondSinceFirstFrame);
				uint32_t tempsMoyenParFrame = nanoS.count() / fps;

				win.SetTitle(baseTitle + std::to_string(fps) + ", temps moyen par frame : " + std::to_string(tempsMoyenParFrame) + "us");
				fps = 0;
				firstFrameTime = Clock::now();
				lastFrameTime = firstFrameTime;
			}



			auto evt = win.PeekEvent();

			if (evt->GetType() == Croissant::Graphic::WindowEventType::CLOSE)
			{
				win.Close();
				break;
			}

			renderer.Render();
		}
	}
	catch (std::exception& e)
	{
		std::cout << "Exception : " << e.what() << std::endl;
	}

//	TraceEventListener listener;
//	auto em = std::unique_ptr<Croissant::Event::EventManager>(CROISSANT_NEW Croissant::Event::EventManager());
//
//	em->RegisterListener([&listener](Croissant::Event::Event* evt) -> void {
//		listener.ProcessEvent(evt);
//	});
//	app.SetEventManager(std::move(em));

	auto result = app.Run();

    app.Shutdown();
//	system("PAUSE");
	return result;
}


