#ifndef CROISSANT_ENGINE_GRAPHIC_OPENGLRENDERER_HPP_INC
#  define CROISSANT_ENGINE_GRAPHIC_OPENGLRENDERER_HPP_INC

#  include "Engine.hpp"
#  include "Core/Enums.hpp"
#  include "Graphic/OpenGLWrapper.hpp"
#  include "Graphic/OpenGLSystemInfo.hpp"
#  include "Core/LogManager.hpp"
#  include "Core/VertexBufferDescriptor.hpp"
#  include <memory>
#  include <string>
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/glext.h>
#  include <map>
#  if defined(CROISSANT_LINUX)
#    include <GL/glx.h>
#  endif

namespace Croissant
{
	namespace Core
	{
		class VertexBuffer;
		class Node;
		class AbstractBuffer;
	}

	namespace Graphic
	{
		class Camera;
		class Window;
		class OpenGLRenderer;
		class OpenGLSystemInfo;

		// ------------------- Début alias méthodes OpenGL


		// ------------------- Fin alias méthodes OpenGL
		template<typename EnumType>
		class EnumConverter
		{
		public:
			static std::string ToString(EnumType val)
			{
				auto item = m_valMap.find(val);

				if (item == m_valMap.end())
				{
					// not found....
					return std::string("");
				}

				return item->second;
			}

		private:
			static std::map<EnumType, std::string> m_valMap;
		};

		enum class BufferType
		{
			INDEX,
			VERTEX
		};

		class ENGINE_API OpenGLRenderer
		{
		public:
			OpenGLRenderer(Window& window);
			~OpenGLRenderer();
			/// <summary>Fait le rendu d'un noeud par rapport à une caméra</summary>
			void Render(std::shared_ptr<Camera> camera, std::shared_ptr<Core::Node> node);
			/// <summary>Récupère un objet qui représente les informations sur le contexte OpenGL courant</summary>
			OpenGLSystemInfo const& GetSystemInfo() const;
			OpenGLWrapper const& GetOpenGLWrapper() const;
			std::shared_ptr<Core::AbstractBuffer>	CreateBuffer(uint32_t size, Core::BufferTypeEnum type);

		private:
#if defined(CROISSANT_WINDOWS)
			void SetupPixelFormat(HDC hdc);
#endif
			void InitializeGL(GLsizei width, GLsizei height);
			void InitializeGLExtentions();
			void UpdateCamera(std::shared_ptr<Camera> camera);
			void UpdateMatrixes();
			void UpdateVertexDescription(std::shared_ptr<Core::VertexBuffer> vertexBuffer);

			OpenGLWrapper m_wrapper;
			OpenGLSystemInfo m_systemInfo;
			Window& m_window;
			Core::LogManager::Log m_logManager;
			std::shared_ptr<Camera>	m_camera;
			Core::VertexBufferDescriptor	m_vertexDescriptor;
			uint32_t						m_vertexBufferId;
#if defined(CROISSANT_WINDOWS)
			HDC m_ghDC;
			HGLRC m_contextGl;
#elif defined(CROISSANT_LINUX)
			GLXContext m_contextGl;
#endif



		};
	}
}


#endif /* !CROISSANT_ENGINE_GRAPHIC_OPENGLRENDERER_HPP_INC */
