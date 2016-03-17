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

#include <Exception/CroissantException.hpp>
#include "Graphic/Window.hpp"
#include "Graphic/WindowEvent.hpp"
#include <iostream>
#include <chrono>
#include "Math/Matrix.hpp"
#include <cmath>

#define PI 3.14159265f

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
#include <Math/Vector4.hpp>
#include <Math/Quaternion.hpp>
#include <Math/Matrix4.hpp>

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

class ApplicationBase
{
protected:
	void OnInitializeBase() { std::cout << "Dans ApplicationBase::OnInitializeBase" << std::endl; }
};

namespace Imp
{
	template<typename T>
	struct HasMethodInitialize
	{
		template<typename U, void (U::*)()> struct SFINAE { };
		template<typename U> static int8_t Test(SFINAE<U, &U::OnInitialize>*);
		template<typename U> static int16_t Test(...);
		static const bool Has = sizeof(Test<T>(nullptr)) == sizeof(int8_t);
	};

	template<typename App, bool val = HasMethodInitialize<App>::Has>
	struct AppInitialization
	{
		static void InitializeInternal(App &);
	};

	template<typename App>
	struct AppInitialization<App, false>
	{
		static void InitializeInternal(App &)
		{
			std::cout << "specialisation false" << std::endl;
		}
	};

	template<typename App>
	struct AppInitialization<App, true>
	{
		static void InitializeInternal(App &ref)
		{
			std::cout << "specialisation true" << std::endl;
			ref.OnInitialize();
		}
	};
}

template<typename App>
class Application : public App, public ApplicationBase
{
	//void Initialize() { InitializeInternal<App>(*this); }
public:
	void Initialize() { Imp::AppInitialization<App>::InitializeInternal(*this); }
};

class DBlockApplication
{
public:
	void OnInitialize() { std::cout << "Dans DBlockApplication::OnInitialize" << std::endl; }

};


int main(int, char**)
{
	Application<DBlockApplication> appTest;

	appTest.Initialize();

	return 0;


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
		Time prevFrameTime{ firstFrameTime };

		// --------------------------------------------------------------------------- begin initialisation
		auto vertexShaderContent = std::string(R"(
#version 140

/********************Entrant********************/
in vec3 VertexPosition;
in vec3 VertexColor;

/********************Uniformes********************/
uniform mat4 WorldViewProjMatrix;

/********************Fonctions********************/
out vec3 vcolor;

void main()
{
	gl_Position = WorldViewProjMatrix * vec4(VertexPosition, 1.0);
	/*gl_Position = matrix * vec4(VertexPosition, 1.0);*/
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
			throw Croissant::Exception::CroissantException( "Erreur lors de la compilation du vertex shader");
		}

		auto fragmentShaderId = opengl.CreateShader(GL_FRAGMENT_SHADER);
		opengl.ShaderSource(fragmentShaderId, fragmentShaderContent);
		opengl.CompileShader(fragmentShaderId);
		auto fragmentShaderResult = opengl.GetShaderInteger(fragmentShaderId, Croissant::Graphic::OpenGLShaderIntegerNameEnum::CompileStatus);

		if (fragmentShaderResult == 0)
		{
			throw Croissant::Exception::CroissantException("Erreur lors de la compilation du fragment shader");
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
				throw Croissant::Exception::CroissantException("Une erreur est survenue lors de l'�tape de linkage du programme.");
			}

			throw Croissant::Exception::CroissantException(log.c_str());
		}

		// TODO supprimer les shaders ?

		// TODO : configurer les sommets pour l'affichage
		//vertexProp vertices[] = {
		//	vertexProp{ { -0.5, 0.5, 0.5 },{ 255, 0, 0 } },
		//	vertexProp{ { 0.5, 0.5, 0.5 },{ 255, 255, 255 } },
		//	vertexProp{ { -0.5, 0.5, -0.5 },{ 0, 255, 0 } },
		//	vertexProp{ { 0.5, 0.5, -0.5 },{ 255, 255, 255 } },
		//	vertexProp{ { -0.5, -0.5, 0.5 },{ 0, 0, 255 } },
		//	vertexProp{ { 0.5, -0.5, 0.5 },{ 255, 255, 255 } },
		//	vertexProp{ { -0.5, -0.5, -0.5 },{ 255, 0, 255 } },
		//	vertexProp{ { 0.5, -0.5, -0.5 },{ 255, 255, 255 } }
		//};
		vertexProp vertices[] = {
			vertexProp{ { 0.0, 0.0, 0.0 },{ 0xFF, 0x00, 0x00 } }, // face avant Z, bas gauche
			vertexProp{ { 0.5, 0.0, 0.0 },{ 0xFF, 0x00, 0x00 } }, // face avant Z, bas droit
			vertexProp{ { 0.5, 0.5, 0.0 },{ 0xFF, 0x00, 0x00 } }, // face avant Z, haut droit
			vertexProp{ { 0.0, 0.5, 0.0 },{ 0xFF, 0x00, 0x00 } }, // face avant Z, haut gauche
			vertexProp{ { 0.0, 0.0, 0.5 },{ 0x00, 0xFF, 0x00 } }, // face arrière Z, bas gauche
			vertexProp{ { 0.5, 0.0, 0.5 },{ 0x00, 0xFF, 0x00 } }, // face arrière Z, bas droit
			vertexProp{ { 0.5, 0.5, 0.5 },{ 0x00, 0xFF, 0x00 } }, // face arrière Z, haut droit
			vertexProp{ { 0.0, 0.5, 0.5 },{ 0x00, 0xFF, 0x00 } }, // face arrière Z, haut gauche
			vertexProp{ { 0.0, 0.0, 0.5 },{ 0x00, 0x00, 0xFF } }, // face avant X, bas gauche
			vertexProp{ { 0.0, 0.0, 0.0 },{ 0x00, 0x00, 0xFF } }, // face avant X, bas droit
			vertexProp{ { 0.0, 0.5, 0.0 },{ 0x00, 0x00, 0xFF } }, // face avant X, haut droit
			vertexProp{ { 0.0, 0.5, 0.5 },{ 0x00, 0x00, 0xFF } }, // face avant X, haut gauche
			vertexProp{ { 0.5, 0.0, 0.5 },{ 0x66, 0x66, 0x66 } }, // face arrière X, bas gauche
			vertexProp{ { 0.5, 0.0, 0.0 },{ 0x66, 0x66, 0x66 } }, // face arrière X, bas droit
			vertexProp{ { 0.5, 0.5, 0.0 },{ 0x66, 0x66, 0x66 } }, // face arrière X, haut droit
			vertexProp{ { 0.5, 0.5, 0.5 },{ 0x66, 0x66, 0x66 } }, // face arrière X, haut gauche
		};
		uint32_t verticesBufferId;
		uint32_t indexesBufferId;

		opengl.GenBuffers(1, &verticesBufferId);
		opengl.BindBuffer(GL_ARRAY_BUFFER, verticesBufferId);
		opengl.BufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// cube indexes

		//uint32_t indexes[] = {
		//	0, 2, 4, // demi face gauche (-x)
		//	4, 2, 6, // demi face gauche (-x)
		//	3, 1, 7, // demi face droit (x)
		//	7, 1, 5, // demi face droit (x)
		//	0, 1, 2, // demi face dessus (y)
		//	2, 1, 3, // demi face dessus (y)
		//	4, 1, 0, // demi face fond (z)
		//	5, 1, 4, // demi face fond (z)
		//	6, 2, 3, // demi face devant (-z)
		//	6, 3, 7, // demi face devant (-z)
		//	6, 5, 4, // demi face dessous (-y)
		//	7, 5, 6 // demi face dessous (-y)
		//};
		uint32_t indexes[] = {
			0, 1, 2, // demi face avant Z, bas droite
			0, 2, 3, // demi face avant Z, haut gauche
			7, 5, 4, // demi face arrière Z, bas droite
			7, 6, 5, // demi face arrière Z, haut gauche
			8, 9, 10, // demi face avant X, bas droite
			8, 10, 11, // demi face avant X, haut gauche
			15, 13, 12, // demi face arrière X, bas droite
			15, 14, 13, // demi face arrière X, haut gauche
		};

		opengl.GenBuffers(1, &indexesBufferId);
		opengl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexesBufferId);
		opengl.BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

		opengl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		opengl.BindBuffer(GL_ARRAY_BUFFER, 0);
		auto indexesSize = sizeof(indexes);

		// TODO : 
		auto uniformWorldViewProjMatrix = opengl.GetUniformLocation(programId, "WorldViewProjMatrix");
		//auto projection = Croissant::Math::Matrix4f::Identity();
		//auto angleX = 30.0f * PI / 180.0f;
		//auto angleY = 30.0f * PI / 180.0f;
		//auto angleZ = 30.0f * PI / 180.0f;

		// toutes les rotations sont dans le sens inverse des aiguilles d'une montre
		//// rotation sur Z
		//auto yaw = Croissant::Math::Matrix4f({
		//	std::cosf(angleZ),	-std::sinf(angleZ),	0.0f,	0.0f,
		//	std::sinf(angleZ),	std::cosf(angleZ),	0.0f,	0.0f,
		//	0.0f,				0.0f,				1.0f,	0.0f,
		//	0.0f,				0.0f,				0.0f,	1.0f
		//});

		//// rotation sur Y
		//auto pitch = Croissant::Math::Matrix4f({
		//	std::cosf(angleY),	0.0f,	std::sinf(angleY),	0.0f,
		//	0.0f,				1.0f,	0.0f,				0.0f,
		//	-std::sinf(angleY),	0.0f,	std::cosf(angleY),	0.0f,
		//	0.0f,				0.0f,	0.0f,				1.0f
		//});

		//// rotation sur X
		//auto roll = Croissant::Math::Matrix4f({
		//	1.0f,	0.0f,				0.0f,				0.0f,
		//	0.0f,	std::cosf(angleX),	-std::sinf(angleX),	0.0f,
		//	0.0f,	std::sinf(angleX),	std::cosf(angleX),	0.0f,
		//	0.0f,	0.0f,				0.0f,				1.0f
		//});

		// --------------------------------------------------------------------------- end   initialisation
		auto baseAngle = 0.0f;
		auto step = 45.0f;
		while (1)
		{

			++fps;
			Time currentFrameTime { Clock::now() };
			durationSecond secondSincePrevFrame{ currentFrameTime - prevFrameTime };
			durationSecond secondSinceFirstFrame { currentFrameTime - firstFrameTime };
			if (secondSinceFirstFrame.count() >= 1.0f) {
				durationNanoSecond nanoS = std::chrono::duration_cast<durationNanoSecond>(secondSinceFirstFrame);
				uint32_t tempsMoyenParFrame = nanoS.count() / fps;

				win.SetTitle(baseTitle + std::to_string(fps) + ", temps moyen par frame : " + std::to_string(tempsMoyenParFrame) + "us");
				fps = 0;
				firstFrameTime = Clock::now();
				lastFrameTime = firstFrameTime;
			}

			baseAngle += (step * secondSincePrevFrame.count());

			if (baseAngle > 360.0f)
			{
				baseAngle -= 360.0f;
			}

			auto angleX = 30.0f * PI / 180.0f;
			auto angleY = 30.0f * PI / 180.0f;
			auto angleZ = baseAngle * PI / 180.0f;

			//Croissant::Math::Vector4 tmp(0.0, 0.0, 1.0);
			Croissant::Math::Vector4 tmp(1.0, 0.0, 0.0);
			//tmp.MakeUnit();
			Croissant::Math::Quaternion quat(tmp, angleZ);
			auto rotation = quat.ToMatrix();

			// rotation complète
			//auto sinX = std::sin(angleX);
			//auto cosX = std::cos(angleX);
			//auto sinY = std::sin(angleY);
			//auto cosY = std::cos(angleY);
			//auto sinZ = std::sin(angleZ);
			//auto cosZ = std::cos(angleZ);
			//auto rotation = Croissant::Math::Matrix4f({
			//	cosZ * cosY,	cosZ * sinY * sinX - sinZ * cosX,	cosZ * sinY * cosX + sinZ * sinX,	0.0f,
			//	sinZ * cosY,	sinZ * sinY * sinX + cosZ * cosX,	sinZ * sinY * cosX - cosZ * sinX,	0.0f,
			//	-sinY,			cosY * sinX,						cosY * cosX,						0.0f,
			//	0.0f,			0.0f,								0.0f,								1.0f
			//});

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

			opengl.SetUniformMatrix4f(uniformWorldViewProjMatrix, 1, true, rotation);

			//opengl.VertexPointer(3, GL_FLOAT, sizeof(vertexProp), 0);
			//opengl.ColorPointer(3, GL_UNSIGNED_BYTE, sizeof(vertexProp), BUFFER_OFFSET(sizeof(vertexProp::m_coord)));
			opengl.VertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(vertexProp), 0);
			opengl.VertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, true, sizeof(vertexProp), BUFFER_OFFSET(sizeof(vertices[0].m_coord)));

//			opengl.PolygonMode(GL_FRONT, GL_LINE);
			opengl.DrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexesSize), GL_UNSIGNED_INT, nullptr);
//			opengl.PolygonMode(GL_FRONT, GL_FILL);

			opengl.DisableVertexAttribArray(0);
			opengl.DisableVertexAttribArray(1);

			opengl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			opengl.BindBuffer(GL_ARRAY_BUFFER, 0);


			renderer.Render();
			prevFrameTime = currentFrameTime;
		}
	}
	catch (std::exception& e)
	{
		std::cout << "Exception : " << e.what() << std::endl;
		std::cout << "Appuyez sur une touche" << std::endl;
		std::getchar();
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


