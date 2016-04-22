#include "..\..\Include\Graphic\OpenGLRenderer.hpp"
/*
 * OpenGLRenderer.cpp
 *
 *  Created on: 3 sept. 2014
 *      Author: Gregory_Compte
 */
// http://www.irit.fr/~Mathias.Paulin/M2IM/RenduTempsReel/VBO.html

#include "Debug/MemoryManager.hpp"
#include "Graphic/OpenGLRenderer.hpp"
#include "Graphic/Window.hpp"
#include "Core/LogManager.hpp"
#include "Exception/CroissantException.hpp"
#include <iostream>
#include <chrono>



#define BLACK_INDEX     0
#define RED_INDEX       13
#define GREEN_INDEX     14
#define BLUE_INDEX      16

#define GLOBE    1
#define CYLINDER 2
#define CONE     3
#define SWAPBUFFERS SwapBuffers(m_ghDC)
#define BUFFER_OFFSET(val) reinterpret_cast<void*>(val)

using OpenGLRendererException = Croissant::Exception::CroissantException;

#define LOGGER_NAME "OpenGL"

namespace Croissant
{
	namespace Graphic
	{
		using LogManager = Croissant::Core::LogManager;
		using string = std::string;

		template<>
		std::map<BufferType, std::string> EnumConverter<BufferType>::m_valMap {
			{ BufferType::INDEX, "INDEX" },
			{ BufferType::VERTEX, "VERTEX" }
		};


		void polarView(double radius, double twist, double latitude,
		           double longitude)
		{
		    glTranslated(0.0, 0.0, -radius);
		    glRotated(-twist, 0.0, 0.0, 1.0);
		    glRotated(-latitude, 1.0, 0.0, 0.0);
		    glRotated(longitude, 0.0, 0.0, 1.0);

		}

		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//         FOR TESTS ONLY
		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		struct vertexProp
		{
			float m_coord[3];
			uint8_t m_color[3];
		};

		void OpenGLRenderer::CreateObjects()
		{
			vertexProp vertices[] = {
					vertexProp { { -0.5, 0.5, 0.5 }, { 255, 0, 0 } },
					vertexProp { { 0.5, 0.5, 0.5 }, { 255, 255, 255 } },
					vertexProp { { -0.5, 0.5, -0.5 }, { 0, 255, 0 } },
					vertexProp { { 0.5, 0.5, -0.5 }, { 255, 255, 255 } },
					vertexProp { { -0.5, -0.5, 0.5 }, { 0, 0, 255 } },
					vertexProp { { 0.5, -0.5, 0.5 }, { 255, 255, 255 } },
					vertexProp { { -0.5, -0.5, -0.5 }, { 255, 0, 255 } },
					vertexProp { { 0.5, -0.5, -0.5 }, { 255, 255, 255 } }
			};

			m_wrapper.GenBuffers(1, &verticesBufferId);
	        m_wrapper.BindBuffer(GL_ARRAY_BUFFER, verticesBufferId);
	        m_wrapper.BufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			verticesSize = sizeof(vertices);
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

			m_wrapper.GenBuffers(1, &indexesBufferId);
			m_wrapper.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexesBufferId);
			m_wrapper.BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

			m_wrapper.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			m_wrapper.BindBuffer(GL_ARRAY_BUFFER, 0);
			indexesSize = sizeof(indexes);

		}


		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//         ENd OF TESTS ONLY
		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		// -------------------------------- OpenGLRenderer::Impl definition
		OpenGLRenderer::OpenGLRenderer(Window& window)
			: m_wrapper { }, m_systemInfo { m_wrapper }, m_window{ window }, m_logManager { CROISSANT_GET_LOG(OpenGLRenderer) }
		{
			m_logManager.Write("Initialisation du renderer OpenGL");
			//serviceProvider.Resolve(m_frameProvider);

			auto hwnd = m_window.GetSystemHandle();
	        m_ghDC = GetDC(hwnd);
	        if (NULL == m_ghDC)
	        {
	        	throw OpenGLRendererException("Erreur lors de la récupération du Draw Context");
	        }

	        SetupPixelFormat(m_ghDC);

	        m_contextGl = wglCreateContext(m_ghDC);
	        if (NULL == m_contextGl)
	        {
	        	throw OpenGLRendererException("Erreur lors de la création du contexte OpenGL.");
	        }

	        if (FALSE == wglMakeCurrent(m_ghDC, m_contextGl))
	        {
	        	throw OpenGLRendererException("Erreur lors de la définition du contexte OpenGL comme courant.");
	        }


	        RECT rect;

	        GetClientRect(hwnd, &rect);
	        InitializeGLExtentions();
	        InitializeGL(rect.right, rect.bottom);
	        CreateObjects();

			//serviceProvider.Resolve(m_eventManager);
			//m_eventManager->RegisterListener("Frame::Render", m_renderDelegate);
		    m_logManager.Write("Renderer OpenGL initialisé avec succès");
		}

		OpenGLRenderer::~OpenGLRenderer()
		{
			//////////////////////////////////////////////////////
			//						DEBUG ONLY					//
			//////////////////////////////////////////////////////

			m_wrapper.DeleteBuffers(1, &indexesBufferId);
			if (0 != pixelShaderId)
			{
				m_wrapper.DeleteShader(pixelShaderId);
			}

			if (0 != vertexShaderId)
			{
				m_wrapper.DeleteShader(vertexShaderId);
			}

			if (0 != programId)
			{
				m_wrapper.DeleteProgram(programId);
			}

			//////////////////////////////////////////////////////
			//						!DEBUG ONLY					//
			//////////////////////////////////////////////////////

			m_logManager.Write("Destruction du renderer OpenGL");
		}

		void OpenGLRenderer::SetupPixelFormat(HDC hdc)
		{
		    PIXELFORMATDESCRIPTOR pfd, *ppfd;
		    int pixelformat;

		    ppfd = &pfd;

		    ppfd->nSize = sizeof(PIXELFORMATDESCRIPTOR);
		    ppfd->nVersion = 1;
		    ppfd->dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
		                        PFD_DOUBLEBUFFER;
		    ppfd->dwLayerMask = PFD_MAIN_PLANE;
		    ppfd->iPixelType = PFD_TYPE_RGBA;
		    ppfd->cColorBits = 32;
		    ppfd->cDepthBits = 24;
		    ppfd->cAccumBits = 0;
		    ppfd->cStencilBits = 0;

		    pixelformat = ChoosePixelFormat(hdc, ppfd);

		    if (pixelformat == 0)
		    {
		    	throw OpenGLRendererException("Erreur lors de la création du renrerer OpenGL : ChoosePixelFormat failed");
		    }

		    if (SetPixelFormat(hdc, pixelformat, ppfd) == FALSE)
		    {
		    	throw OpenGLRendererException("Erreur lors de la création du renrerer OpenGL : SetPixelFormat failed");
		    }
		}

		void OpenGLRenderer::InitializeGL(GLsizei width, GLsizei height)
		{
			float     aspect;

			m_wrapper.ClearIndex((GLfloat)BLACK_INDEX);
			m_wrapper.ClearDepth(1.0);

			m_wrapper.Enable(GL_DEPTH_TEST);
			m_wrapper.Enable(GL_CULL_FACE);

			m_wrapper.MatrixMode(GL_PROJECTION);
		    aspect = static_cast<GLfloat>(width) / static_cast<GLfloat>(height);
		    // TODO replace with camera
		    gluPerspective( 45.0, aspect, 3.0, 7.0 );
			m_wrapper.MatrixMode(GL_MODELVIEW);
	        // désactive la synchronisation verticale
		    m_wrapper.EnableVSync(false);
		}

		void OpenGLRenderer::InitializeGLExtentions()
		{


		}

		using Clock = std::chrono::high_resolution_clock;
		using Time = Clock::time_point;
		using durationSecond = std::chrono::duration<float>;

		void OpenGLRenderer::Render()
		{
			static Time lastFrameTime { Clock::now() };
			Time currentFrameTime { Clock::now() };
			durationSecond secondSinceFirstFrame { currentFrameTime - lastFrameTime };
			lastFrameTime = currentFrameTime;


			//m_logManager.Write("call to render");
			static float pos = 0.0f;
			static UINT64 frameCount = 0ull;

			pos += 0.01f;
			++frameCount;

		    static float latitude = 0.0f;
		    static float longitude = 0.0f;
		    static float latitude2 = 0.0f;
		    static float longitude2 = 0.0f;
			float near_plane = 3.0f;
		    float maxObjectSize = 3.0f;
		    float radius = near_plane + maxObjectSize/2.0f;


		    latitude += (90 * secondSinceFirstFrame.count());
		    longitude += (90 * secondSinceFirstFrame.count());

		    latitude2 += (-120 * secondSinceFirstFrame.count());
		    longitude2 += (-120 * secondSinceFirstFrame.count());
		    //}
		    //m_wrapper.Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		    //m_wrapper.PushMatrix();

		    //    polarView( radius, 0, latitude, longitude );

		        //m_wrapper.BindBuffer(GL_ARRAY_BUFFER, verticesBufferId);
		        //m_wrapper.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexesBufferId);

		        //m_wrapper.EnableClientState(GL_VERTEX_ARRAY);
		        //m_wrapper.EnableClientState(GL_COLOR_ARRAY);

		        //m_wrapper.VertexPointer(3, GL_FLOAT, sizeof(vertexProp), 0);
		        //m_wrapper.ColorPointer(3, GL_UNSIGNED_BYTE, sizeof(vertexProp), BUFFER_OFFSET(sizeof(vertexProp::m_coord)));

		        //m_wrapper.PolygonMode(GL_FRONT, GL_LINE);
		        //m_wrapper.DrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexesSize), GL_UNSIGNED_INT, 0);
		        //m_wrapper.PolygonMode(GL_FRONT, GL_FILL);

		        //m_wrapper.DisableClientState(GL_COLOR_ARRAY);
		        //m_wrapper.DisableClientState(GL_VERTEX_ARRAY);

		        //m_wrapper.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		        //m_wrapper.BindBuffer(GL_ARRAY_BUFFER, 0);



		    //m_wrapper.PopMatrix();

		    //m_wrapper.PushMatrix();

		    //    polarView( radius + 2, 0, latitude2, longitude2 );

		    //    m_wrapper.BindBuffer(GL_ARRAY_BUFFER, verticesBufferId);
		    //    m_wrapper.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexesBufferId);

		    //    m_wrapper.EnableClientState(GL_VERTEX_ARRAY);
		    //    m_wrapper.EnableClientState(GL_COLOR_ARRAY);

		    //    m_wrapper.VertexPointer(3, GL_FLOAT, sizeof(vertexProp), 0);
		    //    m_wrapper.ColorPointer(3, GL_UNSIGNED_BYTE, sizeof(vertexProp), BUFFER_OFFSET(sizeof(vertexProp::m_coord)));

		    //    m_wrapper.PolygonMode(GL_FRONT, GL_FILL);
		    //    m_wrapper.DrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexesSize), GL_UNSIGNED_INT, 0);
		    //    m_wrapper.PolygonMode(GL_FRONT, GL_FILL);

		    //    m_wrapper.DisableClientState(GL_COLOR_ARRAY);
		    //    m_wrapper.DisableClientState(GL_VERTEX_ARRAY);

		    //    m_wrapper.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		    //    m_wrapper.BindBuffer(GL_ARRAY_BUFFER, 0);



		    //m_wrapper.PopMatrix();

		    SWAPBUFFERS;
		    //first_frame = false;
		}

		const OpenGLSystemInfo& OpenGLRenderer::GetSystemInfo() const {
			return m_systemInfo;
		}

		OpenGLWrapper const & OpenGLRenderer::GetOpenGLWrapper() const
		{
			return m_wrapper;
		}


		// ------------------------------------ OpenGLIndexBuffer
		OpenGLBuffer::OpenGLBuffer(OpenGLRenderer& renderer, BufferType type, int32_t size)
			: m_type { type }, m_index { 0 }, m_size { size }, m_renderer { renderer }
		{
			m_renderer.m_wrapper.GenBuffers(1, &m_index);
			if (0 == m_index)
			{
				throw Croissant::Exception::CroissantException("Une erreur s'est produite lors de la création d'un Vertex Buffer");
			}

			auto glType = GetGLBufferType();

			m_renderer.m_wrapper.BindBuffer(glType, m_index);
			m_renderer.m_wrapper.BufferData(glType, size, nullptr, GL_STATIC_DRAW);
			m_renderer.m_wrapper.BindBuffer(glType, 0);
		}

		OpenGLBuffer::OpenGLBuffer(OpenGLBuffer&& ref) noexcept
			: m_type { ref.m_type }, m_index { ref.m_index }, m_size { ref.m_size },
			  m_renderer { ref.m_renderer }
		{
			ref.m_index = 0;
			ref.m_size = 0;
		}

		OpenGLBuffer::~OpenGLBuffer()
		{
			if (0 != m_index)
			{
				// delete buffer unbind le buffer si besoin
				m_renderer.m_wrapper.DeleteBuffers(1, &m_index);
				m_index = 0;
			}
		}

		OpenGLBuffer& OpenGLBuffer::operator =(OpenGLBuffer&& ref) noexcept
		{
			this->m_type = ref.m_type;
			this->m_size = ref.m_size;
			this->m_index = ref.m_index;
			return *this;
		}

		uint32_t OpenGLBuffer::GetIndex() const
		{
			return m_index;
		}

		GLenum OpenGLBuffer::GetGLBufferType() const
		{
			GLenum glType = 0;

			if (m_type == BufferType::INDEX)
			{
				glType = GL_ELEMENT_ARRAY_BUFFER;
			}
			else if (m_type == BufferType::VERTEX)
			{
				glType = GL_ARRAY_BUFFER;
			}

			return glType;
		}

		BufferType OpenGLBuffer::GetType() const
		{
			return m_type;
		}

		// ------------------------------------ OpenGLVertexBufferBinding
		OpenGLBufferBinding::OpenGLBufferBinding(OpenGLBuffer& buffer, OpenGLRenderer& renderer)
			: m_buffer { buffer }, m_renderer { renderer }
		{
			CROISSANT_GET_LOG(OpenGLBufferBinding).Write(string("Binding du buffer n°" + std::to_string(m_buffer.GetIndex())));
			m_renderer.m_wrapper.BindBuffer(m_buffer.GetGLBufferType(), m_buffer.GetIndex());
		}

		OpenGLBufferBinding::~OpenGLBufferBinding()
		{
			CROISSANT_GET_LOG(OpenGLBufferBinding).Write(string("Binding du buffer n°" + std::to_string(m_buffer.GetIndex())));
			m_renderer.m_wrapper.BindBuffer(m_buffer.GetGLBufferType(), 0);
		}

		// ------------------------------------ OpenGLBufferMapping
		OpenGLBufferMapping::OpenGLBufferMapping(OpenGLBuffer& buffer, OpenGLRenderer& renderer)
			: m_buffer { buffer }, m_renderer { renderer }, m_data { nullptr }
		{
			CROISSANT_GET_LOG(OpenGLBufferMapping).Write(string("Mapping du buffer n°" + std::to_string(m_buffer.GetIndex())));
			m_data = m_renderer.m_wrapper.MapBuffer(m_buffer.GetGLBufferType(), GL_READ_WRITE);
		}

		OpenGLBufferMapping::~OpenGLBufferMapping()
		{
			CROISSANT_GET_LOG(OpenGLBufferMapping).Write(string("Unmapping du buffer n°" + std::to_string(m_buffer.GetIndex()) + " du type " + EnumConverter<BufferType>::ToString(m_buffer.GetType())));
			m_renderer.m_wrapper.UnmapBuffer(m_buffer.GetGLBufferType());
		}
	}
}
