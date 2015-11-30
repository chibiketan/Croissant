#pragma once
#ifndef WINDOWSDX11MODULE_H_
#define WINDOWSDX11MODULE_H_

#include "Module/IModule.hpp"
#include "Core/Delegate.hpp"

#include <memory>
#include <Windows.h>
#include <GL/glew.h>
#include <gl/gl.h>


namespace Croissant
{
	namespace Core
	{
		class IFrameProvider;
	}
	namespace Event
	{
		class Event;
		class EventManager;
		class IEventManagerImplementor;
	}

	namespace Module
	{
		namespace OpenGL
		{
			class Window;

			class WindowsOpenGLModule : public Croissant::Module::IModule
			{
			public:
				WindowsOpenGLModule(std::shared_ptr<Croissant::Module::OpenGL::Window> windows);
				virtual ~WindowsOpenGLModule();
				virtual void ProcessEvent( Croissant::Event::Event* evt );
				virtual bool Init(Croissant::Module::ServiceProvider& serviceProvider);
				virtual void Cleanup();
				virtual const String& Name() const;
				//HRESULT CompileShaderFromFile( LPCSTR szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut );

			private:
				bool SetupPixelFormat(HDC hdc);
				void InitializeGL(GLsizei width, GLsizei height);
				void CreateObjects();

				std::shared_ptr<Croissant::Module::OpenGL::Window> m_windows;
				static const String s_name;

				std::unique_ptr<Croissant::Event::IEventManagerImplementor> m_eventManager;
				Croissant::Core::Delegate<std::function<void (Croissant::Event::Event*)>> m_renderDelegate;
				HDC m_ghDC;
				double m_radius;
				float m_latitude;
				float m_longitude;
				float m_latinc;
				float m_longinc;
				std::unique_ptr<Croissant::Core::IFrameProvider> m_frameProvider;
				//D3D_DRIVER_TYPE         g_driverType;
				//D3D_FEATURE_LEVEL       g_featureLevel;
				//ID3D11Device*           g_pd3dDevice;
				//ID3D11DeviceContext*    g_pImmediateContext;
				//IDXGISwapChain*         g_pSwapChain;
				//ID3D11RenderTargetView* g_pRenderTargetView;
				//ID3D11VertexShader*     g_pVertexShader;
				//ID3D11PixelShader*      g_pPixelShader;
				//ID3D11InputLayout*      g_pVertexLayout;
				//ID3D11Buffer*           g_pVertexBuffer;
				//ID3D11Buffer*			m_constantBuffer;
			};

#define SWAPBUFFERS SwapBuffers(m_ghDC)
#define BLACK_INDEX     0
#define RED_INDEX       13
#define GREEN_INDEX     14
#define BLUE_INDEX      16
#define WIDTH           300
#define HEIGHT          200

#define GLOBE    1
#define CYLINDER 2
#define CONE     3
		}
	}
}

#endif
