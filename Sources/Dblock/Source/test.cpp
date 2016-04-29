// TODO
// 4) Implémenter une caméra
// 6) Implémenter une classe Monde avec une caméra et un ensemble de node
// 7) implémenter un node d'un monde qui contient 
// 9) Implémenter les fenêtres dans un module
// 10) Implémenter le rendered dans un module

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
#include "Math/Point2.hpp"
#include "Math/EulerAngle.hpp"
#include "Core/Node.hpp"
#include "Graphic/WindowEventNone.hpp"
#include "Math/Matrix.hpp"

#define PI 3.14159265f

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

auto vertexShaderContent = std::string(R"(
#version 140

/********************Entrant********************/
in vec3 VertexPosition;
in vec3 VertexColor;

/********************Uniformes********************/
uniform mat4 WorldViewProjMatrix;
uniform mat4 ModelWorldMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

/********************Fonctions********************/
out vec3 vcolor;

void main()
{
	gl_Position = ProjectionMatrix * ViewMatrix * ModelWorldMatrix * vec4(VertexPosition, 1.0);
	/*gl_Position = WorldViewProjMatrix * vec4(VertexPosition, 1.0);*/
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

auto indexesSize = sizeof(indexes);
auto planIndexesSize = sizeof(planIndexes);
auto pointIndexesSize = sizeof(pointIndexes);


namespace Croissant
{
	namespace DBlock
	{
		struct PressedKeys
		{
			bool Up = false;
			bool Down = false;
			bool Left = false;
			bool Right = false;
			bool PageUp = false;
			bool PageDown = false;
		};

		class DBlockApplication
		{
		public:
			DBlockApplication()
				: m_log{ CROISSANT_GET_LOG_WITH_FILE(DBlockApplication) }, m_keys{}, m_win{ 1024, 768, "test titre" },
				m_renderer{ m_win }
			{
			}

			bool OnInitialize(Core::Application<DBlockApplication>& app)
			{
				Core::WriteTraceDebug("Début de OnInitialize");
				auto vpWidth = 1920;
				auto vpHeight = 1080;

				m_win.SetPosition(Math::Point2{ (vpWidth - m_win.Width()) / 2.0f, (vpHeight - m_win.Height()) / 2.0f });
				m_win.CenterCursor();
				m_win.Open();
				// traite tous les évènements en attente
				while (m_win.PeekEvent()->GetType() != Croissant::Graphic::WindowEventType::NONE)
				{

				}

				auto opengl = m_renderer.GetOpenGLWrapper();
				// initialisation des shaders
				// see exemple code in http://docs.gl/gl3/glCompileShader
				m_programId = opengl.CreateProgram();
				auto vertexShaderId = opengl.CreateShader(GL_VERTEX_SHADER);
				opengl.ShaderSource(vertexShaderId, vertexShaderContent);
				opengl.CompileShader(vertexShaderId);
				auto vertexShaderResult = opengl.GetShaderInteger(vertexShaderId, Croissant::Graphic::OpenGLShaderIntegerNameEnum::CompileStatus);

				if (vertexShaderResult == 0)
				{
					m_log.Write("Erreur lors de la compilation du vertex shader");
					return false;
				}

				auto fragmentShaderId = opengl.CreateShader(GL_FRAGMENT_SHADER);
				opengl.ShaderSource(fragmentShaderId, fragmentShaderContent);
				opengl.CompileShader(fragmentShaderId);
				auto fragmentShaderResult = opengl.GetShaderInteger(fragmentShaderId, Croissant::Graphic::OpenGLShaderIntegerNameEnum::CompileStatus);

				if (fragmentShaderResult == 0)
				{
					m_log.Write("Erreur lors de la compilation du fragment shader");
					return false;
				}

				// on associe à chaque nom de variable des shader un index pour y faire référence plus tard à partir du vertexbuffer 
				opengl.BindAttribLocation(m_programId, 0, "VertexPosition");
				opengl.BindAttribLocation(m_programId, 1, "VertexColor");

				// on marque les shader comme à attacher au programme
				opengl.AttachShader(m_programId, fragmentShaderId);
				opengl.AttachShader(m_programId, vertexShaderId);

				// on lie le programme et les shaders (création du programme dans la CG)
				opengl.LinkProgram(m_programId);
				auto programLinked = opengl.GetProgramInteger(m_programId, Croissant::Graphic::OpenGLProgramIntegerNameEnum::LinkStatus);

				if (programLinked == 0)
				{
					auto log = opengl.GetProgramInfoLog(m_programId);

					if (log.empty())
					{
						m_log.Write("Une erreur est survenue lors de l'�tape de linkage du programme.");
					}
					else
					{
						m_log.Write(log.c_str());
					}

					return false;
				}

				// TODO supprimer les shaders ?


				opengl.GenBuffers(1, &m_verticesBufferId);
				opengl.BindBuffer(GL_ARRAY_BUFFER, m_verticesBufferId);
				opengl.BufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

				opengl.GenBuffers(1, &m_planVerticesBufferId);
				opengl.BindBuffer(GL_ARRAY_BUFFER, m_planVerticesBufferId);
				opengl.BufferData(GL_ARRAY_BUFFER, sizeof(planVertices), planVertices, GL_STATIC_DRAW);

				opengl.GenBuffers(1, &m_pointVerticesBufferId);
				opengl.BindBuffer(GL_ARRAY_BUFFER, m_pointVerticesBufferId);
				opengl.BufferData(GL_ARRAY_BUFFER, sizeof(pointVertices), pointVertices, GL_STATIC_DRAW);

				opengl.GenBuffers(1, &m_indexesBufferId);
				opengl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexesBufferId);
				opengl.BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);
				
				opengl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				opengl.BindBuffer(GL_ARRAY_BUFFER, 0);

				// plan index buffer
				opengl.GenBuffers(1, &m_planIndexesBufferId);
				opengl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_planIndexesBufferId);
				opengl.BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(planIndexes), planIndexes, GL_STATIC_DRAW);
				opengl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				opengl.BindBuffer(GL_ARRAY_BUFFER, 0);

				// point index buffer
				opengl.GenBuffers(1, &m_pointIndexesBufferId);
				opengl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pointIndexesBufferId);
				opengl.BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pointIndexes), pointIndexes, GL_STATIC_DRAW);
				opengl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				opengl.BindBuffer(GL_ARRAY_BUFFER, 0);

				// on récupère les variables
				m_uniformWorldViewProjMatrix = opengl.GetUniformLocation(m_programId, "WorldViewProjMatrix");
				m_uniformModelWorldMatrix	= opengl.GetUniformLocation(m_programId, "ModelWorldMatrix");
				m_uniformViewMatrix			= opengl.GetUniformLocation(m_programId, "ViewMatrix");
				m_uniformProjectionMatrix	= opengl.GetUniformLocation(m_programId, "ProjectionMatrix");

				// initialisation de la caméra
				m_camNode = std::make_shared<Core::Node>();
				m_cam.SetNode(m_camNode);

				m_cam.SetFieldOfViewDegree(90.0f);
				m_cam.SetAspectRatio(static_cast<float>(m_win.Width()) / static_cast<float>(m_win.Height()));
				m_cam.SetNearDistance(1.0f);
				m_cam.SetFarDistance(1000.0f);
				m_camNode->Move(Croissant::Math::Vector4::UnitZ * -5);

				Core::WriteTraceDebug("Fin de OnInitialize");
				return true;
			}
			
			bool OnShutdown(Core::Application<DBlockApplication>& app)
			{
				std::cout << "Dans DBlockApplication::OnShutdown" << std::endl;
				return true;
			}

			float m_elapsedTime = 0;
			bool OnFrame(int32_t& returnValue, Core::FrameTime const& lastFrameTime, Core::FrameTime const& currentFrameTime)
			{
				std::string const baseTitle("Ma fenetre - fps : ");
				static auto fps = 0;

				returnValue = 0;
				++fps;

				durationSecond secondSincePrevFrame{ currentFrameTime - lastFrameTime };
				m_elapsedTime += secondSincePrevFrame.count();
				if (m_elapsedTime >= 1.0f) {
					std::cout << "coucou" << std::endl;
					auto nanoS = m_elapsedTime * 1e+6;
					uint32_t tempsMoyenParFrame = nanoS / fps;

					m_win.SetTitle(baseTitle + std::to_string(fps) + ", temps moyen par frame : " + std::to_string(tempsMoyenParFrame) + "us, " + std::to_string(m_elapsedTime));
					fps = 0;
					//m_elapsedTime = 0.0f;
					m_elapsedTime -= 1.0f;

					// rotation chaque seconde
					//Croissant::Math::EulerAngle rotAngle{
					//	10.0f,
					//	10.0f,
					//	0.0f
					//};

					//camNode->Rotate(Croissant::Math::ToQuaternion(rotAngle));
					//camNode->Move(Croissant::Math::Vector4::UnitX);
					//std::cout << "cam View matrix : " << cam.GetViewMatrix() << std::endl;
					//std::cout << "cam ViewProjection matrix : " << cam.GetProjectionMatrix() << std::endl;
					//std::cout << "camNode rotation : " << camNode->GetRotation() << std::endl;
					//std::cout << "cam position : " << cam.GetPosition() << std::endl;
					//std::cout << "cam right vector : " << cam.GetRightDirection() << std::endl;
				}


				// Gestion des évènements
				auto evt = m_win.PeekEvent();

				if (evt->GetType() == Croissant::Graphic::WindowEventType::CLOSE)
				{
					m_win.Close();
					return false;
				}

				if (evt->GetType() == Croissant::Graphic::WindowEventType::KEYDOWN
					|| evt->GetType() == Croissant::Graphic::WindowEventType::KEYUP)
				{
					auto& keyEvt = static_cast<Croissant::Graphic::WindowEventKey const&>(*evt);
					auto keyState = keyEvt.Type() == Croissant::Graphic::WindowEventKeyType::Press;

					switch (keyEvt.Key())
					{
					case Croissant::Graphic::WindowKey::Up:
						m_keys.Up = keyState;
						break;
					case Croissant::Graphic::WindowKey::Down:
						m_keys.Down = keyState;
						break;
					case Croissant::Graphic::WindowKey::Left:
						m_keys.Left = keyState;
						break;
					case Croissant::Graphic::WindowKey::Right:
						m_keys.Right = keyState;
						break;
					case Croissant::Graphic::WindowKey::PageUp:
						m_keys.PageUp = keyState;
						break;
					case Croissant::Graphic::WindowKey::PageDown:
						m_keys.PageDown = keyState;
						break;
					default: break;
					}
				}

				if (evt->GetType() == Croissant::Graphic::WindowEventType::MOUSEMOVE)
				{
					auto& mouseMoveEvt = static_cast<Croissant::Graphic::WindowMouseMoveEvent const&>(*evt);

					// TODO : Utiliser des angles d'Euler plutôt qu'essayer de créer un quaternion
					Croissant::Math::EulerAngle rotAngle{
						-mouseMoveEvt.DeltaX() * 0.3f,
						mouseMoveEvt.DeltaY() * 0.3f,
						0
					};

					//cam.Rotate(rotAngle);
					m_camNode->Rotate(Croissant::Math::ToQuaternion(rotAngle));
					//std::cout << "camNode rotation : " << camNode->GetRotation() << std::endl;

					//Croissant::Math::Quaternion tmpQuat{ Croissant::Math::Vector4::Zero, 1.0f };

					//if (mouseMoveEvt.DeltaX() != 0)
					//{
					//	tmpQuat *= Croissant::Math::Quaternion{Croissant::Math::Vector4::UnitY, (mouseMoveEvt.DeltaX() / 100.0f) * (PI / 180.0f) };
					//}

					//if (mouseMoveEvt.DeltaY() != 0)
					//{
					//	tmpQuat *= Croissant::Math::Quaternion{Croissant::Math::Vector4::UnitX, (mouseMoveEvt.DeltaY() / 100.0f) * (PI / 180.0f) };
					//}

					//cam.Rotate(tmpQuat);
					//std::cout << "delta [" << mouseMoveEvt.DeltaX() << ", " << mouseMoveEvt.DeltaY() << "], " << rotAngle << std::endl;
					m_win.CenterCursor();
					//std::cout << "MouseMove : deltaX = " << mouseMoveEvt.DeltaX() << std::endl;
					//std::cout << "MouseMove : deltaY = " << mouseMoveEvt.DeltaY() << std::endl;
					//Croissant::Math::Vector4 rotAxe{ 0.0f, 0.0f, 0.0f };
				}

				// move camera
				if (m_keys.Up)
				{
					// up first if up and down simultaneously set
					m_camNode->Move(m_cam.GetLookDirection() * secondSincePrevFrame.count());
				}
				else if (m_keys.Down)
				{
					m_camNode->Move(-m_cam.GetLookDirection() * secondSincePrevFrame.count());
				}

				if (m_keys.Right)
				{
					// up first if up and down simultaneously set
					m_camNode->Move(m_cam.GetRightDirection() * secondSincePrevFrame.count());
				}
				else if (m_keys.Left)
				{
					m_camNode->Move(-m_cam.GetRightDirection() * secondSincePrevFrame.count());
				}

				if (m_keys.PageUp)
				{
					// up first if up and down simultaneously set
					m_camNode->Move(-m_cam.GetUpDirection() * secondSincePrevFrame.count());
					///cam.Move(cam.UpVector() *  secondSincePrevFrame.count());
				}
				else if (m_keys.PageDown)
				{
					m_camNode->Move(m_cam.GetUpDirection() * secondSincePrevFrame.count());
				}

				// force matrix update
				m_camNode->Update();


				// RENDER

				// clear
				auto opengl = m_renderer.GetOpenGLWrapper();
				opengl.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				opengl.UseProgram(m_programId);
				// render

				// render plan
				opengl.BindBuffer(GL_ARRAY_BUFFER, m_planVerticesBufferId);
				opengl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_planIndexesBufferId);
				opengl.EnableVertexAttribArray(0);
				opengl.EnableVertexAttribArray(1);
				// définition des constantes
				//opengl.SetUniformMatrix4f(uniformWorldViewProjMatrix, 1, true, cam.GetProjectionViewMatrix());
				opengl.SetUniformMatrix4f(m_uniformViewMatrix, 1, true, m_cam.GetViewMatrix());
				opengl.SetUniformMatrix4f(m_uniformProjectionMatrix, 1, true, m_cam.GetProjectionMatrix());
				// définition des attributs des vertex
				opengl.VertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(vertexProp), nullptr);
				opengl.VertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, true, sizeof(vertexProp), BUFFER_OFFSET(sizeof(planVertices[0].m_coord)));
				// dessin effectif
				opengl.SetUniformMatrix4f(m_uniformModelWorldMatrix, 1, true, Croissant::Math::Matrix4f::Identity());
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


				m_renderer.Render();
				return true;
			}

		private:
			Core::LogManager::Log m_log;
			PressedKeys m_keys;
			Croissant::Graphic::Window m_win;
			Croissant::Graphic::OpenGLRenderer m_renderer;
			uint32_t	m_programId;


			uint32_t m_verticesBufferId;
			uint32_t m_indexesBufferId;
			uint32_t m_planVerticesBufferId;
			uint32_t m_planIndexesBufferId;
			uint32_t m_pointVerticesBufferId;
			uint32_t m_pointIndexesBufferId;
			int32_t m_uniformWorldViewProjMatrix;
			int32_t m_uniformModelWorldMatrix;
			int32_t m_uniformViewMatrix;
			int32_t m_uniformProjectionMatrix;
			std::shared_ptr<Core::Node> m_camNode;
			Croissant::Graphic::Camera m_cam;
		};
	}
}




int main(int, char**)
{
	Croissant::Core::LogManager::Init();
	Croissant::Core::Application<Croissant::DBlock::DBlockApplication> app("DBlock");

	if (!app.Initialize())
	{
		return EXIT_FAILURE;
	}

	auto result = app.Run();

	app.Shutdown();
	std::cout << "Fin de l'application" << std::endl;
	return result;
}


