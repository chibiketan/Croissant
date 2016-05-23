#include "Graphic/Window.hpp"
#include <GL/glx.h>
#include "Graphic/WindowEventNone.hpp"

namespace Croissant
{
    namespace Graphic
    {
        class WindowInternal
        {
        public:
            WindowInternal(Math::Point2 const& position, uint32_t width, uint32_t height);
            ~WindowInternal();
            Window::SystemHandle const&    GetSystemHandle() const;
            void        Resize(uint32_t width, uint32_t height);
            void        Position(Math::Point2 const& position);
            void        Show() const;
            void        Hide() const;
            void        SetTitle(std::string const& title);
            void        SetCursorPosition(int x, int y);
            std::unique_ptr<WindowEvent const> PeekEvent();

        private:
            Window::SystemHandle    m_handle;
            Colormap                m_cmap;
        };

        WindowInternal::WindowInternal(Math::Point2 const& position, uint32_t width, uint32_t height)
        {
            m_handle.m_display = XOpenDisplay(nullptr);
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
            if ( !glXQueryVersion( m_handle.m_display, &glx_major, &glx_minor ) ||
                 ( ( glx_major == 1 ) && ( glx_minor < 3 ) ) || ( glx_major < 1 ) )
            {
                printf("Invalid GLX version");
                exit(1);
            }

            printf( "Getting matching framebuffer configs\n" );
            int fbcount;
            GLXFBConfig* fbc = glXChooseFBConfig(m_handle.m_display, DefaultScreen(m_handle.m_display), visual_attribs, &fbcount);
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
                XVisualInfo *vi = glXGetVisualFromFBConfig( m_handle.m_display, fbc[i] );
                if ( vi )
                {
                    int samp_buf, samples;
                    glXGetFBConfigAttrib( m_handle.m_display, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf );
                    glXGetFBConfigAttrib( m_handle.m_display, fbc[i], GLX_SAMPLES       , &samples  );

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
            XVisualInfo *vi = glXGetVisualFromFBConfig( m_handle.m_display, bestFbc );
            printf( "Chosen visual ID = 0x%x\n", vi->visualid );

            printf( "Creating colormap\n" );
            XSetWindowAttributes swa;

            swa.colormap = m_cmap = XCreateColormap( m_handle.m_display,
                                                   RootWindow( m_handle.m_display, vi->screen ),
                                                   vi->visual, AllocNone );
            swa.background_pixmap = None ;
            swa.border_pixel      = 0;
            swa.event_mask        = StructureNotifyMask;

            m_handle.m_window = XCreateWindow( m_handle.m_display, RootWindow( m_handle.m_display, vi->screen ),
                                        0, 0, width, height, 0, vi->depth, InputOutput,
                                        vi->visual,
                                        CWBorderPixel|CWColormap|CWEventMask, &swa );
            // Done with the visual info data
            XFree( vi );

            SetTitle("Test window");

        }

        void WindowInternal::Show() const
        {
            XMapWindow( m_handle.m_display, m_handle.m_window);
        }

        void WindowInternal::SetTitle(std::string const &title)
        {
            XStoreName( m_handle.m_display, m_handle.m_window, title.c_str() );
        }

        void WindowInternal::Hide() const
        {
            XUnmapWindow(m_handle.m_display, m_handle.m_window);
        }



        WindowInternal::~WindowInternal()
        {
            XDestroyWindow( m_handle.m_display, m_handle.m_window );
            XFreeColormap( m_handle.m_display, m_cmap );
            XCloseDisplay( m_handle.m_display );
            m_handle.m_display = nullptr;
        }

        void WindowInternal::Position(Math::Point2 const &position)
        {
            XMoveWindow(m_handle.m_display, m_handle.m_window, position.X(), position.Y());
        }

        void WindowInternal::Resize(uint32_t width, uint32_t height)
        {
            XResizeWindow(m_handle.m_display, m_handle.m_window, width, height);
        }

        Window::SystemHandle const& WindowInternal::GetSystemHandle() const
        {
            return m_handle;
        }

        void WindowInternal::SetCursorPosition(int x, int y)
        {
            XWarpPointer(m_handle.m_display, None, m_handle.m_window, 0, 0, 0, 0, x, y);
        }

        std::unique_ptr<WindowEvent const> WindowInternal::PeekEvent()
        {
            return std::make_unique<WindowEventNone>();
        }

    }
}