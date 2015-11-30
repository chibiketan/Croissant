#include "Core/DebugMemoryManager.hpp"
#include "OpenGL/WindowsOpenGLModule.hpp"
#include "OpenGL/Window.hpp"
#include "Event/IEventManagerImplementor.hpp"
#include "Module/ServiceProvider.hpp"
#include "Core/IFrameProvider.hpp"
#include "Math/Vector.hpp"
#include "Core/Delegate.hpp"


#include <chrono>

#include <iostream>

#include <GL/glu.h>
#include <GL/wglew.h>

#include <iostream>
#include <cstdint>

#define BUFFER_OFFSET(val) reinterpret_cast<void*>(val)

namespace Croissant
{
	namespace Module
	{
		namespace OpenGL
		{

			struct vertexProp
			{
				float m_coord[3];
				uint8_t m_color[3];
			};

			uint32_t verticesBufferId = 0;
			uint32_t indexesBufferId = 0;
			size_t verticesSize;
			size_t indexesSize;

			struct SimpleVertex
			{
				//XMFLOAT3 Pos;
				std::tuple<float, float, float> Pos;
			};

			// -------------------------------------- WindowsDX11Module implï¿½mentation
			const String WindowsOpenGLModule::s_name = "OpenGL::WindowsOpenGL";

			WindowsOpenGLModule::WindowsOpenGLModule(std::shared_ptr<Croissant::Module::OpenGL::Window> windows)
				: m_windows(windows)
					, m_eventManager(nullptr)
					, m_renderDelegate([this](Croissant::Event::Event* evt) -> void {
						this->ProcessEvent(evt);
					})
					//, g_driverType(D3D_DRIVER_TYPE_NULL)
					//, g_featureLevel(D3D_FEATURE_LEVEL_11_0)
					//, g_pd3dDevice(nullptr)
					//, g_pImmediateContext(nullptr)
					//, g_pSwapChain(nullptr)
					//, g_pRenderTargetView(nullptr)
					//, g_pVertexShader(nullptr)
					//, g_pPixelShader(nullptr)
					//, g_pVertexLayout(nullptr)
					//, g_pVertexBuffer(nullptr)
					//, m_constantBuffer(nullptr)
			{
			}

			WindowsOpenGLModule::~WindowsOpenGLModule()
			{
			}


			void polarView(double radius, double twist, double latitude,
			           double longitude)
			{
			    glTranslated(0.0, 0.0, -radius);
			    glRotated(-twist, 0.0, 0.0, 1.0);
			    glRotated(-latitude, 1.0, 0.0, 0.0);
			    glRotated(longitude, 0.0, 0.0, 1.0);

			}


			void WindowsOpenGLModule::ProcessEvent( Croissant::Event::Event*)
			{
				auto &frame = m_frameProvider->CurrentFrame();

				// Clear the back buffer
				//float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha
				//g_pImmediateContext->ClearRenderTargetView( g_pRenderTargetView, ClearColor );

				static float pos = 0.0f;
				static UINT64 frameCount = 0.0;

				// set constant buffer
				Croissant::Math::Vector vec(0.0f, pos, 0.0f);
				//g_pImmediateContext->UpdateSubresource(m_constantBuffer, 0, nullptr, &vec, 0, 0);

				//std::cout << "frame time : " << frame.Duration().count() << std::endl;
				pos += 0.01f;
				if (frame.IsFirstFrame())
				{
					std::cout << "frame per second : " << frameCount << std::endl;
					frameCount = 0.0;
				}

				++frameCount;

				// Render a triangle
				//g_pImmediateContext->VSSetShader( g_pVertexShader, NULL, 0 );
				//g_pImmediateContext->VSSetConstantBuffers(0, 1, &m_constantBuffer);
				//g_pImmediateContext->PSSetShader( g_pPixelShader, NULL, 0 );
				//g_pImmediateContext->Draw( 3, 0 );

				// Present the information rendered to the back buffer to the front buffer (the screen)
				//g_pSwapChain->Present( 0, 0 );

			    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

			    glPushMatrix();




			    //if (!first_frame)
			    //{
		        //m_latitude += (m_latinc * (float)frame.Duration().count() / 1000.0f);
		        //m_longitude += (m_longinc * (float)frame.Duration().count() / 1000.0f);
		        m_latitude += (m_latinc * frame.Duration().count());
		        m_longitude += (m_longinc * frame.Duration().count());
			    //}
			        polarView( m_radius, 0, m_latitude, m_longitude );

			        glIndexi(RED_INDEX);
			        glCallList(CONE);

			        glIndexi(BLUE_INDEX);
			        glCallList(GLOBE);

			        glBindBuffer(GL_ARRAY_BUFFER, verticesBufferId);
			        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexesBufferId);

			        glEnableClientState(GL_VERTEX_ARRAY);
			        glEnableClientState(GL_COLOR_ARRAY);

			        glVertexPointer(3, GL_FLOAT, sizeof(vertexProp), 0);
			        glColorPointer(3, GL_UNSIGNED_BYTE, sizeof(vertexProp), BUFFER_OFFSET(sizeof(vertexProp::m_coord)));

			        //glDrawElements(GL_TRIANGLES, indexesSize, GL_UNSIGNED_INT, 0);
			        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
			        glDrawElements(GL_TRIANGLES, indexesSize, GL_UNSIGNED_INT, 0);
			        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

			        //glDrawElements(GL_POINTS, verticesSize, GL_UNSIGNED_BYTE, 0);
			        auto err = glGetError();

			        if (GL_NO_ERROR != err)
			        {
			        	switch (err)
			        	{
			        	case GL_INVALID_ENUM:
			        		std::cout << "La valeur de mode est invalide" << std::endl;
			        		break;
			        	case GL_INVALID_VALUE:
			        		std::cout << "La valeur de count est négative" << std::endl;
			        		break;
			        	case GL_INVALID_OPERATION :
			        		std::cout << "mode incompatible avec le géométrie shader ou trop d'objets sont mappé pour le rendu" << std::endl;
			        		break;
			        	}
			        }

			        glDisableClientState(GL_COLOR_ARRAY);
			        glDisableClientState(GL_VERTEX_ARRAY);


			        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			        glBindBuffer(GL_ARRAY_BUFFER, 0);


			    glIndexi(GREEN_INDEX);
			        glPushMatrix();
			            glTranslatef(0.8F, -0.65F, 0.0F);
			            glRotatef(30.0F, 1.0F, 0.5F, 1.0F);
			            glCallList(CYLINDER);
			        glPopMatrix();

			    glPopMatrix();

			    SWAPBUFFERS;
			    //first_frame = false;
			}

			const String& WindowsOpenGLModule::Name() const
			{
				return s_name;
			}

			bool WindowsOpenGLModule::SetupPixelFormat(HDC hdc)
			{
			    PIXELFORMATDESCRIPTOR pfd, *ppfd;
			    int pixelformat;

			    ppfd = &pfd;

			    ppfd->nSize = sizeof(PIXELFORMATDESCRIPTOR);
			    ppfd->nVersion = 1;
			    ppfd->dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
			                        PFD_DOUBLEBUFFER;
			    ppfd->dwLayerMask = PFD_MAIN_PLANE;
			    ppfd->iPixelType = PFD_TYPE_COLORINDEX;
			    ppfd->cColorBits = 8;
			    ppfd->cDepthBits = 16;
			    ppfd->cAccumBits = 0;
			    ppfd->cStencilBits = 0;

			    pixelformat = ChoosePixelFormat(hdc, ppfd);

			    if ( (pixelformat = ChoosePixelFormat(hdc, ppfd)) == 0 )
			    {
			        MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK);
			        return FALSE;
			    }

			    if (SetPixelFormat(hdc, pixelformat, ppfd) == FALSE)
			    {
			        MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK);
			        return false;
			    }

			    return true;
			}

			void WindowsOpenGLModule::InitializeGL(GLsizei width, GLsizei height)
			{
			    float     maxObjectSize, aspect;
			    double    near_plane;
			    //double far_plane;

			    glClearIndex( (GLfloat)BLACK_INDEX);
			    glClearDepth( 1.0 );

			    glEnable(GL_DEPTH_TEST);
			    glEnable(GL_CULL_FACE);

			    glMatrixMode( GL_PROJECTION );
			    aspect = (GLfloat) width / height;
			    gluPerspective( 45.0, aspect, 3.0, 7.0 );
			    glMatrixMode( GL_MODELVIEW );

			    near_plane = 3.0;
			    //far_plane = 7.0;
			    maxObjectSize = 3.0F;
			    m_radius = near_plane + maxObjectSize/2.0;

			    m_latitude = 90.0F;
			    m_longitude = 0.0F;
			    m_latinc = 0.0F;
			    m_longinc = 180.0F;

			    CreateObjects();
			}

			void WindowsOpenGLModule::CreateObjects()
			{
				vertexProp vertices[] = {
						vertexProp { -0.5, 0.5, 0.5, 255, 0, 0 },
						vertexProp { 0.5, 0.5, 0.5, 255, 255, 255 },
						vertexProp { -0.5, 0.5, -0.5, 0, 255, 0 },
						vertexProp { 0.5, 0.5, -0.5, 255, 255, 255 },
						vertexProp { -0.5, -0.5, 0.5, 0, 0, 255 },
						vertexProp { 0.5, -0.5, 0.5, 255, 255, 255 },
						vertexProp { -0.5, -0.5, -0.5, 255, 0, 255 },
						vertexProp { 0.5, -0.5, -0.5, 255, 255, 255 }
				};

				glGenBuffers(1, &verticesBufferId);
				glBindBuffer(GL_ARRAY_BUFFER, verticesBufferId);
				glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

				verticesSize = sizeof(vertices);
				std::cout << "sizeof(vertices) : " << sizeof(vertices) << std::endl;

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

				glGenBuffers(1, &indexesBufferId);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexesBufferId);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				indexesSize = sizeof(indexes);
				std::cout << "sizeof(indexes) : " << sizeof(indexes) << std::endl;

			    GLUquadricObj *quadObj;

			    glNewList(GLOBE, GL_COMPILE);
			        quadObj = gluNewQuadric ();
			        gluQuadricDrawStyle (quadObj, GLU_LINE);
			        gluSphere (quadObj, 1.5, 16, 16);
			    glEndList();

			    glNewList(CONE, GL_COMPILE);
			        quadObj = gluNewQuadric ();
			        gluQuadricDrawStyle (quadObj, GLU_FILL);
			        gluQuadricNormals (quadObj, GLU_SMOOTH);
			        gluCylinder(quadObj, 0.3, 0.0, 0.6, 15, 10);
			    glEndList();

			    glNewList(CYLINDER, GL_COMPILE);
			        glPushMatrix ();
			        glRotatef ((GLfloat)90.0, (GLfloat)1.0, (GLfloat)0.0, (GLfloat)0.0);
			        glTranslatef ((GLfloat)0.0, (GLfloat)0.0, (GLfloat)-1.0);
			        quadObj = gluNewQuadric ();
			        gluQuadricDrawStyle (quadObj, GLU_FILL);
			        gluQuadricNormals (quadObj, GLU_SMOOTH);
			        gluCylinder (quadObj, 0.3, 0.3, 0.6, 12, 2);
			        glPopMatrix ();
			    glEndList();
			}

			bool WindowsOpenGLModule::Init( Croissant::Module::ServiceProvider& serviceProvider )
			{
				if (!m_windows->Init())
				{
					return false;
				}

				serviceProvider.Resolve(m_frameProvider);

				//HRESULT hr = S_OK;
				auto hwnd = m_windows->GetHandle();
		        m_ghDC = GetDC(hwnd);

		        if (!SetupPixelFormat(m_ghDC))
		        {
		        	Cleanup();
		        	return false;
		        }

		        auto ghRC = wglCreateContext(m_ghDC);
		        wglMakeCurrent(m_ghDC, ghRC);

		        if (GLEW_OK !=  glewInit())
		        {
		        	Cleanup();
		        	return false;
		        }

		        // desactive VSYNC
		        wglSwapIntervalEXT(0);

		        RECT rect;

		        GetClientRect(hwnd, &rect);
		        InitializeGL(rect.right, rect.bottom);

				serviceProvider.Resolve(m_eventManager);
				m_eventManager->RegisterListener("Frame::Render", m_renderDelegate);

				return true;

			}

			void WindowsOpenGLModule::Cleanup()
			{
				glDeleteBuffers(1, &verticesBufferId);
				glDeleteBuffers(1, &indexesBufferId);
				m_eventManager->UnregisterListener("Frame::Render", m_renderDelegate);
			}

		}
	}
}
