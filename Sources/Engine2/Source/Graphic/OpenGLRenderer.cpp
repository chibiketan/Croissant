#include "Graphic/OpenGLRenderer.hpp"
// http://www.irit.fr/~Mathias.Paulin/M2IM/RenduTempsReel/VBO.html

#include "Debug/MemoryManager.hpp"
#include "Graphic/Window.hpp"
#include "Core/LogManager.hpp"
#include "Exception/CroissantException.hpp"
#include "Graphic/OpenGLBuffer.hpp"
#include "Graphic/Camera.hpp"
#include <iostream>
#include <chrono>
#include <Core/VertexBuffer.hpp>
#if defined(CROISSANT_LINUX)
#  include <GL/glx.h>
#endif


#define BLACK_INDEX     0
#define RED_INDEX       13
#define GREEN_INDEX     14
#define BLUE_INDEX      16

#define GLOBE    1
#define CYLINDER 2
#define CONE     3

#if defined(CROISSANT_WINDOWS)
#  define SWAPBUFFERS SwapBuffers(m_ghDC)
#elif defined(CROISSANT_LINUX)
#  define SWAPBUFFERS glXSwapBuffers(m_window.GetSystemHandle().m_display, m_window.GetSystemHandle().m_window)
#endif
#define BUFFER_OFFSET(val) reinterpret_cast<void*>(val)
#define GLX_CONTEXT_MAJOR_VERSION_ARB       0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB       0x2092
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

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

		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//         ENd OF TESTS ONLY
		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		// -------------------------------- OpenGLRenderer::Impl definition
		OpenGLRenderer::OpenGLRenderer(Window& window)
			: m_wrapper { }, m_systemInfo { m_wrapper }, m_window{ window }, m_logManager { CROISSANT_GET_LOG(OpenGLRenderer) }
		{
			m_logManager.Write("Initialisation du renderer OpenGL");
			//serviceProvider.Resolve(m_frameProvider);

#if defined(CROISSANT_WINDOWS)
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
#elif defined(CROISSANT_LINUX)
            auto hwnd = m_window.GetSystemHandle();
			// Get a matching FB config
			static int visual_attribs[] =
					{
							GLX_X_RENDERABLE    , True,
							GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
							GLX_RENDER_TYPE     , GLX_RGBA_BIT,
							GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
							GLX_RED_SIZE        , 8,
							GLX_GREEN_SIZE      , 8,
							GLX_BLUE_SIZE       , 8,
							GLX_ALPHA_SIZE      , 8,
							GLX_DEPTH_SIZE      , 24,
							GLX_STENCIL_SIZE    , 8,
							GLX_DOUBLEBUFFER    , True,
							//GLX_SAMPLE_BUFFERS  , 1,
							//GLX_SAMPLES         , 4,
							None
					};

			int glx_major, glx_minor;

			// FBConfigs were added in GLX version 1.3.
			if ( !glXQueryVersion( hwnd.m_display, &glx_major, &glx_minor ) ||
				 ( ( glx_major == 1 ) && ( glx_minor < 3 ) ) || ( glx_major < 1 ) )
			{
				printf("Invalid GLX version");
				exit(1);
			}

			printf( "Getting matching framebuffer configs\n" );
			int fbcount;
			GLXFBConfig* fbc = glXChooseFBConfig(hwnd.m_display, DefaultScreen(hwnd.m_display), visual_attribs, &fbcount);
			if (!fbc)
			{
				printf( "Failed to retrieve a framebuffer config\n" );
				exit(1);
			}
			printf( "Found %d matching FB configs.\n", fbcount );

			// Pick the FB config/visual with the most samples per pixel
			printf( "Getting XVisualInfos\n" );
			int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;

			int i;
			for (i=0; i<fbcount; ++i)
			{
				XVisualInfo *vi = glXGetVisualFromFBConfig( hwnd.m_display, fbc[i] );
				if ( vi )
				{
					int samp_buf, samples;
					glXGetFBConfigAttrib( hwnd.m_display, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf );
					glXGetFBConfigAttrib( hwnd.m_display, fbc[i], GLX_SAMPLES       , &samples  );

					printf( "  Matching fbconfig %d, visual ID 0x%2x: SAMPLE_BUFFERS = %d,"
									" SAMPLES = %d\n",
							i, vi -> visualid, samp_buf, samples );

					if ( best_fbc < 0 || samp_buf && samples > best_num_samp )
						best_fbc = i, best_num_samp = samples;
					if ( worst_fbc < 0 || !samp_buf || samples < worst_num_samp )
						worst_fbc = i, worst_num_samp = samples;
				}
				XFree( vi );
			}

			GLXFBConfig bestFbc = fbc[ best_fbc ];

			// Be sure to free the FBConfig list allocated by glXChooseFBConfig()
			XFree( fbc );

			// Get a visual
			//XVisualInfo *vi = glXGetVisualFromFBConfig( hwnd, bestFbc );
			//printf( "Chosen visual ID = 0x%x\n", vi->visualid );


			// NOTE: It is not necessary to create or make current to a context before
			// calling glXGetProcAddressARB
			glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
			glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)
					glXGetProcAddressARB( (const GLubyte *) "glXCreateContextAttribsARB" );

			int context_attribs[] =
					{
							GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
							GLX_CONTEXT_MINOR_VERSION_ARB, 0,
							//GLX_CONTEXT_FLAGS_ARB        , GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
							None
					};

			printf( "Creating context\n" );
			m_contextGl = glXCreateContextAttribsARB( hwnd.m_display, bestFbc, 0,
											  True, context_attribs );

			// Sync to ensure any errors generated are processed.
			XSync( hwnd.m_display, False );

			// Verifying that context is a direct context
			if ( ! glXIsDirect ( hwnd.m_display, m_contextGl ) )
			{
				printf( "Indirect GLX rendering context obtained\n" );
			}
			else
			{
				printf( "Direct GLX rendering context obtained\n" );
			}

			printf( "Making context current\n" );
			glXMakeCurrent( hwnd.m_display, hwnd.m_window, m_contextGl );

#endif


			//serviceProvider.Resolve(m_eventManager);
			//m_eventManager->RegisterListener("Frame::Render", m_renderDelegate);
		    m_logManager.Write("Renderer OpenGL initialisé avec succès");
		}

		OpenGLRenderer::~OpenGLRenderer()
		{
			m_logManager.Write("Destruction du renderer OpenGL");
		}

#if defined(CROISSANT_WINDOWS)
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
#endif

		void OpenGLRenderer::InitializeGL(GLsizei width, GLsizei height)
		{
			float     aspect;

			m_wrapper.ClearIndex((GLfloat)BLACK_INDEX);
			m_wrapper.ClearDepth(1.0);

			m_wrapper.Enable(GL_DEPTH_TEST);
			m_wrapper.Enable(GL_CULL_FACE);

			//m_wrapper.MatrixMode(GL_PROJECTION);
		 //   aspect = static_cast<GLfloat>(width) / static_cast<GLfloat>(height);
		 //   // TODO replace with camera
		 //   gluPerspective( 45.0, aspect, 3.0, 7.0 );
			//m_wrapper.MatrixMode(GL_MODELVIEW);
	        // désactive la synchronisation verticale
		    m_wrapper.EnableVSync(false);
		}

		void OpenGLRenderer::InitializeGLExtentions()
		{


		}

		void OpenGLRenderer::UpdateCamera(std::shared_ptr<Camera> camera)
		{
			if (m_camera == camera)
			{
				// no camera change, nothing to do
				return;
			}

			// TODO : unregister to camera updates from old camera

			m_camera = camera;

			// TODO : register to camera updates
			// TODO : Update view/projection matrixes
		}

		void OpenGLRenderer::UpdateMatrixes()
		{
			if (nullptr == m_camera)
			{
				Core::WriteTraceWarn("Méthode UpdateMatrixes() appellée sans caméra");
				return;
			}
		}

		void OpenGLRenderer::UpdateVertexDescription(std::shared_ptr<Core::VertexBuffer> vertexBuffer)
		{
			if (vertexBuffer->GetDescriptor() == m_vertexDescriptor)
			{
				// pas de mise à jour des composant du prochain Vertex
				return;
			}

			m_vertexDescriptor = vertexBuffer->GetDescriptor();
			// TODO : 
		}

		void OpenGLRenderer::Render(std::shared_ptr<Camera> camera, std::shared_ptr<Core::Node> node)
		{
			if (nullptr == camera)
			{
				Core::WriteTraceError("Appel de la méthode Render() sans valeur pour la caméra");
				return;
			}

			UpdateCamera(camera);
			UpdateMatrixes();
			for (auto& mesh : node->GetMeshes())
			{

			}
			// TODO : render complete graph ?
			// TODO : define vertex elements
			// TODO : render


			// TODO : SWAPBUFFER devrait être dans la classe Window et pas dans le renderer
		    SWAPBUFFERS;
		}

		const OpenGLSystemInfo& OpenGLRenderer::GetSystemInfo() const
		{
			return m_systemInfo;
		}

		OpenGLWrapper const & OpenGLRenderer::GetOpenGLWrapper() const
		{
			return m_wrapper;
		}

		std::shared_ptr<Core::AbstractBuffer> OpenGLRenderer::CreateBuffer(uint32_t size, Core::BufferTypeEnum type)
		{
			OpenGLBufferUsageEnum glUsage;
			OpenGLBufferTargetEnum glAccess;

			glUsage = OpenGLBufferUsageEnum::StaticDraw;

			if (type == Core::BufferTypeEnum::Index)
			{
				glAccess = OpenGLBufferTargetEnum::ArrayBuffer;
			}
			else
			{
				glAccess = OpenGLBufferTargetEnum::ElementArrayBuffer;
			}

			return std::make_shared<Croissant::Graphic::OpenGLBuffer>(m_wrapper, size, glAccess, glUsage);
		}
	}
}
