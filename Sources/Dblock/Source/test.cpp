// TODO
// 1) Afficher le repère du plan
// 2) Tester le rendu d'un point qui se déplace à l'aide d'un quaternion
// 3) Implémenter la translation via un vecteur
// 4) Implémenter une caméra
// 5) implémenter la matrice de transformation plan ==> vue
// 6) Implémenter une classe Monde avec une caméra et un ensemble de node
// 7) implémenter un node d'un monde qui contient 
// 8) Implémenter une matrice de transformation model => plan
// 9) Implémenter les fenêtres dans un module
// 10) Implémenter le rendered dans un module

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
#include "Graphic/WindowEventKey.hpp"
#include "Graphic/WindowMouseMoveEvent.hpp"

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
#include "Math/Point4.hpp"
#include <Graphic/Camera.hpp>

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

struct PressedKeys
{
	bool Up = false;
	bool Down = false;
	bool Left = false;
	bool Right = false;
	bool PageUp = false;
	bool PageDown = false;
};


#include "Math/Point2.hpp"
#include "Math/EulerAngle.hpp"
#include "Core/Node.hpp"
#include "Graphic/WindowEventNone.hpp"

class A {};
class B : public A {};

#include "Math/Matrix.hpp"

int main(int, char**)
{
	Croissant::Math::Matrix4f testMat {std::array<float, 4*4>{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 1,
			0, 0, 0, 1
	}};

	auto cofactor = testMat.CofactorMatrix();
	auto inverse = testMat.GetInverse();

	std::cout << "matrice : " << testMat << "\nDeteminant : " << testMat.Determinant() << std::endl;
	std::cout << "cofactor : " << cofactor << std::endl;
	std::cout << "inverse : " << inverse << std::endl;

	return 0;

	PressedKeys keys;
	//Application<DBlockApplication> appTest;

	//appTest.Initialize();

	//return 0;
	Croissant::Math::Point4 p(1.0f, 0.0f, 0.0f);
	Croissant::Math::Quaternion Q { Croissant::Math::Vector4 { 0.0f, 0.0f, 1.0f }, 270.0f * PI / 180.0f };
	auto P2 = p * Q.ToMatrix();

	std::cout << "P = " << p << std::endl;
	std::cout << "P' = " << P2 << std::endl;
	std::cout << "M" << std::endl << Q.ToMatrix() << std::endl;

	Croissant::Core::Application app;
	std::string const baseTitle("Ma fenetre - fps : ");

	if (!app.Init())
	{
		return EXIT_FAILURE;
	}


	try
	{
		// TODO use encapsulation !
		auto vpWidth = 1920;
		auto vpHeight = 1080;
		Croissant::Graphic::Window win(1024, 768, "test titre");
		Croissant::Graphic::OpenGLRenderer renderer(win, app.GetLogManager());
		auto opengl = renderer.GetOpenGLWrapper();

		win.SetPosition(Croissant::Math::Point2{ (vpWidth - win.Width()) / 2.0f, (vpHeight - win.Height()) / 2.0f });
		win.CenterCursor();
		win.Open();

		// traite tous les évènements en attente
		while (win.PeekEvent()->GetType() != Croissant::Graphic::WindowEventType::NONE)
		{
			
		}

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
		vertexProp planVertices[] = {
			vertexProp{ { 0.0f, 0.0f, 0.0f },{ 0xFF, 0x00, 0x00 } }, // X
			vertexProp{ { 1.0f, 0.0f, 0.0f },{ 0xFF, 0x00, 0x00 } }, // X
			vertexProp{ { 0.0f, 0.0f, 0.0f },{ 0x00, 0xFF, 0x00 } }, // Y
			vertexProp{ { 0.0f, 1.0f, 0.0f },{ 0x00, 0xFF, 0x00 } }, // Y
			vertexProp{ { 0.0f, 0.0f, 0.0f },{ 0x00, 0x00, 0xFF } }, // Z
			vertexProp{ { 0.0f, 0.0f, 1.0f },{ 0x00, 0x00, 0xFF } }, // Z
		};

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

		vertexProp pointVertices[] = {
			vertexProp{ { 0.75f, 0.0f, 0.0f },{ 0xFF, 0xFF, 0xFF } }
		};

		uint32_t verticesBufferId;
		uint32_t indexesBufferId;
		uint32_t planVerticesBufferId;
		uint32_t planIndexesBufferId;
		uint32_t pointVerticesBufferId;
		uint32_t pointIndexesBufferId;

		opengl.GenBuffers(1, &verticesBufferId);
		opengl.BindBuffer(GL_ARRAY_BUFFER, verticesBufferId);
		opengl.BufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		opengl.GenBuffers(1, &planVerticesBufferId);
		opengl.BindBuffer(GL_ARRAY_BUFFER, planVerticesBufferId);
		opengl.BufferData(GL_ARRAY_BUFFER, sizeof(planVertices), planVertices, GL_STATIC_DRAW);

		opengl.GenBuffers(1, &pointVerticesBufferId);
		opengl.BindBuffer(GL_ARRAY_BUFFER, pointVerticesBufferId);
		opengl.BufferData(GL_ARRAY_BUFFER, sizeof(pointVertices), pointVertices, GL_STATIC_DRAW);

		// reinitialise les matrices de projection et de transformation Model => Vue
		//glMatrixMode(GL_PROJECTION);
		//glLoadIdentity();
		//glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();

		// cube indexes
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

		uint32_t planIndexes[] = {
			0, 1,
			2, 3,
			4, 5
		};

		uint32_t pointIndexes[] = {
			0
		};

		opengl.GenBuffers(1, &indexesBufferId);
		opengl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexesBufferId);
		opengl.BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

		opengl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		opengl.BindBuffer(GL_ARRAY_BUFFER, 0);
		auto indexesSize = sizeof(indexes);


		// plan index buffer
		opengl.GenBuffers(1, &planIndexesBufferId);
		opengl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, planIndexesBufferId);
		opengl.BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(planIndexes), planIndexes, GL_STATIC_DRAW);
		opengl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		opengl.BindBuffer(GL_ARRAY_BUFFER, 0);
		auto planIndexesSize = sizeof(planIndexes);

		// point index buffer
		opengl.GenBuffers(1, &pointIndexesBufferId);
		opengl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, pointIndexesBufferId);
		opengl.BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pointIndexes), pointIndexes, GL_STATIC_DRAW);
		opengl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		opengl.BindBuffer(GL_ARRAY_BUFFER, 0);
		auto pointIndexesSize = sizeof(pointIndexes);


		// TODO : 
		auto uniformWorldViewProjMatrix = opengl.GetUniformLocation(programId, "WorldViewProjMatrix");
		Croissant::Math::Matrix4 translation{ Croissant::Math::Matrix4::ArrayType{
			1.0f, 0.0f, 0.0f, 0.5f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		} };

		// --------------------------------------------------------------------------- end   initialisation
		auto camNode = std::make_shared<Croissant::Core::Node>();
		Croissant::Graphic::Camera cam{};
		
		cam.SetNode(camNode);
		auto baseAngle = 0.0f;
		auto step = 45.0f;
		Croissant::Math::Matrix4 identity;

		cam.SetPosition(Croissant::Math::Point4{ 0.0f, 0.0f, -5.0f });
		Croissant::Math::Vector4 look{ 0.0f, 0.0f, 1.0f };
		Croissant::Math::Vector4 up = Croissant::Math::Vector4::UnitY;
		Croissant::Math::Vector4 right = Croissant::Math::Vector4::UnitX;
		//Croissant::Math::Quaternion camRot{ Croissant::Math::Vector4::UnitX, 20 };
		//auto camRotMat = camRot.ToMatrix();
		cam.SetAxes(Croissant::Math::Vector4::UnitZ, Croissant::Math::Vector4::UnitY, Croissant::Math::Vector4::UnitX);
		//cam.SetAxes(look * camRotMat, up * camRotMat, right * camRotMat);
		cam.SetFrustum(90.0f, static_cast<float>(win.Width()) / static_cast<float>(win.Height()), 1.0f, 1000.0f);
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

			//auto angleX = 30.0f * PI / 180.0f;
			//auto angleY = 30.0f * PI / 180.0f;
			auto angleZ = baseAngle * PI / 180.0f;

			//Croissant::Math::Quaternion camRot{ Croissant::Math::Vector4::UnitX, angleZ };
			//auto camRotMat = camRot.ToMatrix();
			//cam.SetAxes(Croissant::Math::Vector4{ 0.0f, 0.0f, 1.0f }, Croissant::Math::Vector4::UnitY, Croissant::Math::Vector4::UnitX);
			//cam.SetAxes(look * camRotMat, up * camRotMat, right * camRotMat);


			//Croissant::Math::Vector4 tmp(0.0, 0.0, 1.0);
			Croissant::Math::Vector4 tmp(0.0, 1.0, 0.0);
			//tmp.MakeUnit();
			Croissant::Math::Quaternion quat(tmp, angleZ);
			auto rotation = quat.ToMatrix();

			auto evt = win.PeekEvent();

			if (evt->GetType() == Croissant::Graphic::WindowEventType::CLOSE)
			{
				win.Close();
				break;
			}

			if (evt->GetType() == Croissant::Graphic::WindowEventType::KEYDOWN
				|| evt->GetType() == Croissant::Graphic::WindowEventType::KEYUP)
			{
				auto& keyEvt = static_cast<Croissant::Graphic::WindowEventKey const&>(*evt);
				auto keyState = keyEvt.Type() == Croissant::Graphic::WindowEventKeyType::Press;

				switch (keyEvt.Key())
				{
				case Croissant::Graphic::WindowKey::Up:
					keys.Up = keyState;
					break;
				case Croissant::Graphic::WindowKey::Down: 
					keys.Down = keyState;
					break;
				case Croissant::Graphic::WindowKey::Left: 
					keys.Left = keyState;
					break;
				case Croissant::Graphic::WindowKey::Right: 
					keys.Right = keyState;
					break;
				case Croissant::Graphic::WindowKey::PageUp: 
					keys.PageUp = keyState;
					break;
				case Croissant::Graphic::WindowKey::PageDown: 
					keys.PageDown = keyState;
					break;
				default: break;
				}
			}

			//if (evt->GetType() == Croissant::Graphic::WindowEventType::MOUSEMOVE)
			//{
			//	auto& mouseMoveEvt = static_cast<Croissant::Graphic::WindowMouseMoveEvent const&>(*evt);

			//	// TODO : Utiliser des angles d'Euler plutôt qu'essayer de créer un quaternion
			//	Croissant::Math::EulerAngle rotAngle{
			//		mouseMoveEvt.DeltaX() * 0.3f,
			//		mouseMoveEvt.DeltaY() * 0.3f,
			//		0
			//	};

			//	cam.Rotate(rotAngle);

			//	//Croissant::Math::Quaternion tmpQuat{ Croissant::Math::Vector4::Zero, 1.0f };

			//	//if (mouseMoveEvt.DeltaX() != 0)
			//	//{
			//	//	tmpQuat *= Croissant::Math::Quaternion{Croissant::Math::Vector4::UnitY, (mouseMoveEvt.DeltaX() / 100.0f) * (PI / 180.0f) };
			//	//}

			//	//if (mouseMoveEvt.DeltaY() != 0)
			//	//{
			//	//	tmpQuat *= Croissant::Math::Quaternion{Croissant::Math::Vector4::UnitX, (mouseMoveEvt.DeltaY() / 100.0f) * (PI / 180.0f) };
			//	//}

			//	//cam.Rotate(tmpQuat);
			//	std::cout << "delta [" << mouseMoveEvt.DeltaX() << ", " << mouseMoveEvt.DeltaY() << "], " << rotAngle << std::endl;
			//	win.CenterCursor();
			//	//std::cout << "MouseMove : deltaX = " << mouseMoveEvt.DeltaX() << std::endl;
			//	//std::cout << "MouseMove : deltaY = " << mouseMoveEvt.DeltaY() << std::endl;
			//	//Croissant::Math::Vector4 rotAxe{ 0.0f, 0.0f, 0.0f };
			//}

			// move camera
			if (keys.Up)
			{
				// up first if up and down simultaneously set
				camNode->Move(cam.GetRealLookDirection() * secondSincePrevFrame.count());
				auto tp = cam.Position();

				auto tpp = tp * camNode->GetModelToWorldMatrix();

				//cam.Move(cam.LookVector() *  secondSincePrevFrame.count());
				//std::cout << "camNode translation : " << camNode->GetTranslation() << std::endl;
				std::cout << "Model to world matrix : " << camNode->GetModelToWorldMatrix() << std::endl;
				std::cout << "P" << tp << ", P'" << tpp << std::endl;
			}
			else if (keys.Down)
			{
				camNode->Move(-cam.GetRealLookDirection() * secondSincePrevFrame.count());
				auto tp = cam.Position();

				auto tpp = tp * camNode->GetModelToWorldMatrix();

				//cam.Move(-cam.LookVector() *  secondSincePrevFrame.count());
				//std::cout << "camNode translation : " << camNode->GetTranslation() << std::endl;
				std::cout << "Model to world matrix : " << camNode->GetModelToWorldMatrix() << std::endl;
				std::cout << "P" << tp << ", P'" << tpp << std::endl;
			}

			if (keys.Right)
			{
				// up first if up and down simultaneously set
				camNode->Move(cam.GetRealRightDirection() * secondSincePrevFrame.count());
				//cam.Move(cam.RightVector() *  secondSincePrevFrame.count());
			}
			else if (keys.Left)
			{
				camNode->Move(-cam.GetRealRightDirection() * secondSincePrevFrame.count());
				//cam.Move(-cam.RightVector() *  secondSincePrevFrame.count());
			}

			if (keys.PageUp)
			{
				// up first if up and down simultaneously set
				camNode->Move(-cam.GetRealUpDirection() * secondSincePrevFrame.count());
				///cam.Move(cam.UpVector() *  secondSincePrevFrame.count());
			}
			else if (keys.PageDown)
			{
				camNode->Move(cam.GetRealUpDirection() * secondSincePrevFrame.count());
				//cam.Move(-cam.UpVector() *  secondSincePrevFrame.count());
			}

			// force matrix update
			camNode->GetModelToWorldMatrix();

			// clear
			opengl.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			opengl.UseProgram(programId);
			// render

			// render plan
			opengl.BindBuffer(GL_ARRAY_BUFFER, planVerticesBufferId);
			opengl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, planIndexesBufferId);
			opengl.EnableVertexAttribArray(0);
			opengl.EnableVertexAttribArray(1);
			// définition des constantes
			opengl.SetUniformMatrix4f(uniformWorldViewProjMatrix, 1, true, cam.GetProjectionViewMatrix());
			// définition des attributs des vertex
			opengl.VertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(vertexProp), nullptr);
			opengl.VertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, true, sizeof(vertexProp), BUFFER_OFFSET(sizeof(planVertices[0].m_coord)));
			// dessin effectif
			opengl.DrawElements(GL_LINES, static_cast<GLsizei>(planIndexesSize), GL_UNSIGNED_INT, nullptr);
			// desactivation des attributs de vertex
			opengl.DisableVertexAttribArray(0);
			opengl.DisableVertexAttribArray(1);
			// suppression du binding sur les buffers
			opengl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			opengl.BindBuffer(GL_ARRAY_BUFFER, 0);


//			// render cube
//			opengl.BindBuffer(GL_ARRAY_BUFFER, verticesBufferId);
//			opengl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexesBufferId);
//
//			opengl.EnableVertexAttribArray(0);
//			opengl.EnableVertexAttribArray(1);
//
//			//opengl.SetUniformMatrix4f(uniformWorldViewProjMatrix, 1, true, rotation);
//			opengl.SetUniformMatrix4f(uniformWorldViewProjMatrix, 1, true, translation * rotation);
//
//			//opengl.VertexPointer(3, GL_FLOAT, sizeof(vertexProp), 0);
//			//opengl.ColorPointer(3, GL_UNSIGNED_BYTE, sizeof(vertexProp), BUFFER_OFFSET(sizeof(vertexProp::m_coord)));
//			opengl.VertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(vertexProp), 0);
//			opengl.VertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, true, sizeof(vertexProp), BUFFER_OFFSET(sizeof(vertices[0].m_coord)));
//
////			opengl.PolygonMode(GL_FRONT, GL_LINE);
//			opengl.DrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexesSize), GL_UNSIGNED_INT, nullptr);
////			opengl.PolygonMode(GL_FRONT, GL_FILL);
//
//			opengl.DisableVertexAttribArray(0);
//			opengl.DisableVertexAttribArray(1);
//
//			opengl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//			opengl.BindBuffer(GL_ARRAY_BUFFER, 0);

			////render point
			//Croissant::Math::Quaternion quatPoint(Croissant::Math::Vector4{ 0.0f, 0.0f, 1.0f }, angleZ);
			//rotation = quatPoint.ToMatrix();

			//opengl.BindBuffer(GL_ARRAY_BUFFER, pointVerticesBufferId);
			//opengl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, pointIndexesBufferId);
			//opengl.EnableVertexAttribArray(0);
			//opengl.EnableVertexAttribArray(1);
			//// définition des constantes
			//opengl.SetUniformMatrix4f(uniformWorldViewProjMatrix, 1, true, rotation);
			//// définition des attributs des vertex
			//opengl.VertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(vertexProp), nullptr);
			//opengl.VertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, true, sizeof(vertexProp), BUFFER_OFFSET(sizeof(pointVertices[0].m_coord)));
			//// dessin effectif
			//opengl.DrawElements(GL_POINTS, static_cast<GLsizei>(pointIndexesSize), GL_UNSIGNED_INT, nullptr);
			//// desactivation des attributs de vertex
			//opengl.DisableVertexAttribArray(0);
			//opengl.DisableVertexAttribArray(1);
			//// suppression du binding sur les buffers
			//opengl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			//opengl.BindBuffer(GL_ARRAY_BUFFER, 0);


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


