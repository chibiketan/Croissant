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
#include <memory>
#include <string>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <map>

namespace Croissant
{
	namespace Core
	{
		class LogManager;
	}

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

		class ENGINE_API OpenGLBuffer
		{
		public:
			OpenGLBuffer(OpenGLRenderer& renderer, BufferType type, int32_t size);
			OpenGLBuffer(OpenGLBuffer const&) = delete;
			OpenGLBuffer(OpenGLBuffer&& ref) noexcept;
			~OpenGLBuffer();
			OpenGLBuffer& operator=(OpenGLBuffer const&) = delete;
			OpenGLBuffer& operator=(OpenGLBuffer&&) noexcept;
			uint32_t GetIndex() const;
			int32_t GetSize() const;
			GLenum GetGLBufferType() const;
			BufferType GetType() const;

		private:
			BufferType m_type;
			uint32_t m_index;
			int32_t m_size;
			OpenGLRenderer& m_renderer;
		};

		class ENGINE_API OpenGLBufferBinding
		{
		public:
			OpenGLBufferBinding(OpenGLBuffer& buffer, OpenGLRenderer& renderer);
			~OpenGLBufferBinding();

		private:

			OpenGLBuffer&	m_buffer;
			OpenGLRenderer&	m_renderer;
		};

		class ENGINE_API OpenGLBufferMapping
		{
		public:
			OpenGLBufferMapping(OpenGLBuffer& buffer, OpenGLRenderer& renderer);
			~OpenGLBufferMapping();

		private:
			OpenGLBuffer&	m_buffer;
			OpenGLRenderer&	m_renderer;
			void*			m_data;
		};

		class ENGINE_API OpenGLRenderer
		{
			friend class OpenGLBuffer;
			friend class OpenGLBufferBinding;
			friend class OpenGLBufferMapping;
		public:
			OpenGLRenderer(Window& window, Croissant::Core::LogManager& logManager);
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
			Croissant::Core::LogManager& m_logManager;
#if defined(CROISSANT_WINDOWS)
			HDC m_ghDC;
			HGLRC m_contextGl;
#elif defined(CROISSANT_LINUX)
			
#endif



			// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			//         FOR TESTS ONLY
			// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

			uint32_t verticesBufferId = 0;
			uint32_t indexesBufferId = 0;
			size_t verticesSize;
			size_t indexesSize;
			uint32_t vertexShaderId = 0;
			uint32_t pixelShaderId = 0;
			uint32_t programId = 0;
			void CreateObjects();
		};
	}
}


#endif /* OPENGLRENDERER_HPP_INC */