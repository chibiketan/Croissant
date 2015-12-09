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

struct vertexProp
{
	float m_coord[3];
	uint8_t m_color[3];
};

#define BUFFER_OFFSET(val) reinterpret_cast<void*>(val)


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
		auto vertexShaderContent = std::string(R"(
#version 140

/********************Entrant********************/
in vec3 VertexPosition;
in vec3 VertexColor;

/********************Uniformes********************/
/*uniform mat4 WorldViewProjMatrix;*/

/********************Fonctions********************/
out vec3 vcolor;
void main()
{
	/*gl_Position = WorldViewProjMatrix * vec4(VertexPosition, 1.0);*/
	gl_Position = vec4(VertexPosition, 1.0);
	vcolor = VertexColor;
}
)");
		auto fragmentShaderContent = std::string(R"(
#version 140

/********************Entrant********************/
in vec3 vcolor;
/********************Sortant********************/
/*out vec4 RenderTarget0;*/
/********************Uniformes********************/
uniform vec4 Color;

/********************Fonctions********************/
void main()
{
	gl_FragColor = vec4(vcolor, 1.0);
/*	RenderTarget0 = Color;*/
}
)");

		// see exemple code in http://docs.gl/gl3/glCompileShader
		auto programId = opengl.CreateProgram();
		auto vertexShaderId = opengl.CreateShader(GL_VERTEX_SHADER);
		opengl.ShaderSource(vertexShaderId,vertexShaderContent);
		opengl.CompileShader(vertexShaderId);
		auto vertexShaderResult = opengl.GetShaderInteger(vertexShaderId, Croissant::Graphic::OpenGLShaderIntegerNameEnum::CompileStatus);

		if (vertexShaderResult == 0)
		{
			throw std::exception( "Erreur lors de la compilation du vertex shader");
		}

		auto fragmentShaderId = opengl.CreateShader(GL_FRAGMENT_SHADER);
		opengl.ShaderSource(fragmentShaderId, fragmentShaderContent);
		opengl.CompileShader(fragmentShaderId);
		auto fragmentShaderResult = opengl.GetShaderInteger(fragmentShaderId, Croissant::Graphic::OpenGLShaderIntegerNameEnum::CompileStatus);

		if (fragmentShaderResult == 0)
		{
			throw std::exception("Erreur lors de la compilation du fragment shader");
		}

		// on associe � chaque nom de variable des shader un index pour y faire r�f�rence plus tard � partir du vertexbuffer 
		opengl.BindAttribLocation(programId, 0, "VertexPosition");
		opengl.BindAttribLocation(programId, 1, "VertexColor");
		
		// on marque les shader comme � attacher au programme
		opengl.AttachShader(programId, fragmentShaderId);
		opengl.AttachShader(programId, vertexShaderId);

		// on lie le programme et les shaders (cr�ation du programme dans la CG)
		opengl.LinkProgram(programId);
		auto programLinked = opengl.GetProgramInteger(programId, Croissant::Graphic::OpenGLProgramIntegerNameEnum::LinkStatus);

		if (programLinked == 0)
		{
			auto log = opengl.GetProgramInfoLog(programId);

			if (log.empty())
			{
				throw std::exception("Une erreur est survenue lors de l'�tape de linkage du programme.");
			}

			throw std::exception(log.c_str());
		}

		// TODO supprimer les shaders ?

		// TODO : configurer les sommets pour l'affichage
		vertexProp vertices[] = {
			vertexProp{ { -0.5, 0.5, 0.5 },{ 255, 0, 0 } },
			vertexProp{ { 0.5, 0.5, 0.5 },{ 255, 255, 255 } },
			vertexProp{ { -0.5, 0.5, -0.5 },{ 0, 255, 0 } },
			vertexProp{ { 0.5, 0.5, -0.5 },{ 255, 255, 255 } },
			vertexProp{ { -0.5, -0.5, 0.5 },{ 0, 0, 255 } },
			vertexProp{ { 0.5, -0.5, 0.5 },{ 255, 255, 255 } },
			vertexProp{ { -0.5, -0.5, -0.5 },{ 255, 0, 255 } },
			vertexProp{ { 0.5, -0.5, -0.5 },{ 255, 255, 255 } }
		};
		uint32_t verticesBufferId;
		uint32_t indexesBufferId;

		opengl.GenBuffers(1, &verticesBufferId);
		opengl.BindBuffer(GL_ARRAY_BUFFER, verticesBufferId);
		opengl.BufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		auto verticesSize = sizeof(vertices);
		//std::cout << "sizeof(vertices) : " << sizeof(vertices) << std::endl;

		// cube indexes

		uint32_t indexes[] = {
			0, 2, 4, // demi face gauche (-x)
			4, 2, 6, // demi face gauche (-x)
			3, 1, 7, // demi face droit (x)
			7, 1, 5, // demi face droit (x)
			0, 1, 2, // demi face dessus (y)
			2, 1, 3, // demi face dessus (y)
			4, 1, 0, // demi face fond (z)
			5, 1, 4, // demi face fond (z)
			6, 2, 3, // demi face devant (-z)
			6, 3, 7, // demi face devant (-z)
			6, 5, 4, // demi face dessous (-y)
			7, 5, 6 // demi face dessous (-y)
		};

		opengl.GenBuffers(1, &indexesBufferId);
		opengl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexesBufferId);
		opengl.BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

		opengl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		opengl.BindBuffer(GL_ARRAY_BUFFER, 0);
		auto indexesSize = sizeof(indexes);

		// TODO : 

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

			// clear
			opengl.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			opengl.UseProgram(programId);
			// render
			opengl.BindBuffer(GL_ARRAY_BUFFER, verticesBufferId);
			opengl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexesBufferId);

			opengl.EnableVertexAttribArray(0);
			opengl.EnableVertexAttribArray(1);

			//opengl.VertexPointer(3, GL_FLOAT, sizeof(vertexProp), 0);
			//opengl.ColorPointer(3, GL_UNSIGNED_BYTE, sizeof(vertexProp), BUFFER_OFFSET(sizeof(vertexProp::m_coord)));
			opengl.VertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(vertexProp), 0);
			opengl.VertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, false, sizeof(vertexProp), BUFFER_OFFSET(sizeof(vertexProp::m_coord)));

			opengl.PolygonMode(GL_FRONT, GL_LINE);
			opengl.DrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexesSize), GL_UNSIGNED_INT, 0);
			opengl.PolygonMode(GL_FRONT, GL_FILL);

			opengl.DisableVertexAttribArray(0);
			opengl.DisableVertexAttribArray(1);

			opengl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			opengl.BindBuffer(GL_ARRAY_BUFFER, 0);


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
	std::cout << "Appuyez sur une touche" << std::endl;
	std::getchar();
	return result;
}


