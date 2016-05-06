/*
 * OpenGLRenderer.hpp
 *
 *  Created on: 1 sept. 2014
 *      Author: Gregory_Compte
 */

#ifndef OPENGLRENDERER_HPP_INC
#define OPENGLRENDERER_HPP_INC

#include "Engine.hpp"
#include "Graphic/OpenGLWrapper.hpp"
#include "Graphic/OpenGLSystemInfo.hpp"
#include "Core/LogManager.hpp"
#include <memory>
#include <string>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <map>

namespace Croissant
{
	namespace Graphic
	{
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
			friend class OpenGLBuffer;
			friend class OpenGLBufferBinding;
			friend class OpenGLBufferMapping;
		public:
			OpenGLRenderer(Window& window);
			~OpenGLRenderer();
			void Render();
			/// <summary>Récupère un objet qui représente les informations sur le contexte OpenGL courant</summary>
			OpenGLSystemInfo const& GetSystemInfo() const;
			OpenGLWrapper const& GetOpenGLWrapper() const;

		private:
			void SetupPixelFormat(HDC hdc);
			void InitializeGL(GLsizei width, GLsizei height);
			void InitializeGLExtentions();

			OpenGLWrapper m_wrapper;
			OpenGLSystemInfo m_systemInfo;
			Window& m_window;
			Core::LogManager::Log m_logManager;
#if defined(CROISSANT_WINDOWS)
			HDC m_ghDC;
			HGLRC m_contextGl;
#elif defined(CROISSANT_LINUX)
			
#endif



		};
	}
}


#endif /* OPENGLRENDERER_HPP_INC */
