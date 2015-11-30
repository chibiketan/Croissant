/*
 * OpenGLWrapper.hpp
 *
 *  Created on: 6 févr. 2015
 *      Author: Nadege
 */

#ifndef OPENGLWRAPPER_HPP_INC
#define OPENGLWRAPPER_HPP_INC

#include "Engine.hpp"
#include <Graphic/Enums.hpp>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <string>

namespace Croissant
{
	namespace Core
	{
		class LogManager;
	}

	namespace Graphic
	{
		using DebugMessageCallback_t = void (APIENTRY *)(GLenum,
				 GLenum,
				 GLuint,
				 GLenum,
				 GLsizei,
				 const GLchar*,
				 void*);

		// ---------------------------------------- début alias pour OpenGL
		using glDeleteBuffers_t = void (APIENTRY *)(GLsizei, GLuint const*);
		using wglSwapIntervalEXT_t = GLboolean(APIENTRY *)(GLint);
		using glCreateShader_t = GLuint (APIENTRY *)(GLenum);
		using glCreateProgram_t = GLuint (APIENTRY *)();
		using glLinkProgram_t = void (APIENTRY *)(GLuint);
		using glAttachShader_t = void (APIENTRY *)(GLuint, GLuint);
		using glShaderSource_t = void (APIENTRY *)(GLuint, GLsizei, GLchar const**, GLint const*);
		using glBindAttribLocation_t = void (APIENTRY *)(GLuint, GLuint, GLchar const*);
		using glUseProgram_t = void (APIENTRY *)(GLuint);
		using glDeleteProgram_t = void (APIENTRY *)(GLuint);
		using glDeleteShader_t = void (APIENTRY *)(GLuint);
		using glDetachShader_t = void (APIENTRY *)(GLuint, GLuint);
		using glBindBuffer_t = void (APIENTRY *)(GLenum, GLuint);
		using glGenBuffers_t = void (APIENTRY *)(GLsizei, GLuint*);
		using glBufferData_t = void (APIENTRY *)(GLenum, GLsizei, const GLvoid*, GLenum);
		using glDebugMessageCallbackARB_t = void (APIENTRY *)(DebugMessageCallback_t, const void*);
		using glGetStringi_t = const  GLubyte* (APIENTRY *)(GLenum, GLuint);
		using glClearIndex_t = void (APIENTRY *)(GLfloat);
		using glClearDepth_t = void (APIENTRY *)(GLclampd);
		using glMatrixMode_t = void (APIENTRY *)(GLenum);
		using glClear_t = void (APIENTRY *)(GLbitfield);
		using glPushMatrix_t = void (APIENTRY *)(void);
		using glPopMatrix_t = void (APIENTRY *)(void);
		using glEnableClientState_t = void (APIENTRY *)(GLenum);
		using glDisableClientState_t = void (APIENTRY *)(GLenum);
		using glVertexPointer_t = void (APIENTRY *)(GLint, GLenum, GLsizei, GLvoid const*);
		using glColorPointer_t = void (APIENTRY *)(GLint, GLenum, GLsizei, GLvoid const*);
		using glPolygonMode_t = void (APIENTRY *)(GLenum, GLenum);
		using glDrawElements_t = void (APIENTRY *)(GLenum, GLsizei, GLenum, GLvoid const*);
		using glMapBuffer_t = void* (APIENTRY *)(GLenum, GLenum);
		using glUnmapBuffer_t = GLboolean (APIENTRY *)(GLenum);
		using glCompileShader_t = void (APIENTRY *)(GLuint);

		// ---------------------------------------- fin alias pour OpenGL

		class ENGINE_API OpenGLWrapper final
		{
		public:
			template<unsigned int funcIndex> friend inline bool glCheckForError(const OpenGLWrapper& wrapper);
						OpenGLWrapper(Core::LogManager& logManager);
			int32_t		GetLastError() const;
			void		DeleteBuffers(size_t size, uint32_t const* tab) const;
			bool		EnableVSync(bool enable) const;
			uint32_t	CreateShader(GLenum shaderType) const;
			uint32_t	CreateProgram(void) const;
			void		LinkProgram(uint32_t programId) const;
			void		AttachShader(uint32_t programId, uint32_t shaderId) const;
			void		ShaderSource(uint32_t shaderId, std::string const& source) const;
			void		BindAttribLocation(uint32_t programId, uint32_t index, std::string const name) const;
			void		UseProgram(uint32_t programId) const;
			void		DeleteProgram(uint32_t programId) const;
			void		DeleteShader(uint32_t shaderId) const;
			void		DetachShader(uint32_t programId, uint32_t shaderId) const;
			void		BindBuffer(GLenum type, uint32_t shaderId) const;
			void		GenBuffers(size_t size, uint32_t* tab) const;
			void		BufferData(GLenum target, size_t size, void const* buffer, GLenum usageType) const;
			void		RegisterDebugMessageCallback(DebugMessageCallback_t callback) const;
			void		Enable(GLenum capacity) const;
			void		ClearIndex(GLfloat colorIndex) const;
			void		ClearDepth(GLclampd depth) const;
			void		MatrixMode(GLenum mode) const;
			void		Clear(GLbitfield mask) const;
			void		PushMatrix(void) const;
			void		PopMatrix(void) const;
			/// <summary>Method used to enable a client state in the render context</summary>
			void		EnableClientState(GLenum cap) const;
			void		DisableClientState(GLenum cap) const;
			void		VertexPointer(GLint size, GLenum type, GLsizei stride, GLvoid const* pointer) const;
			void		ColorPointer(GLint size, GLenum type, GLsizei stride, GLvoid const* pointer) const;
			void		PolygonMode(GLenum face, GLenum mode) const;
			void		DrawElements(GLenum mode, GLsizei count, GLenum type, GLvoid const* indices) const;
			void*		MapBuffer(GLenum target, GLenum access) const;
			GLboolean	UnmapBuffer(GLenum target) const;
			int32_t		GetInteger(OpenGLValueNameEnum valueName) const;
			void		CompileShader(uint32_t shaderId) const;

		private:
			Core::LogManager&			m_logManager;

			mutable GLint				m_lastGLError = 0;
			glBindBuffer_t				ext_glBindBuffer = nullptr;
			glGenBuffers_t				ext_glGenBuffers = nullptr;
			glBufferData_t				ext_glBufferData = nullptr;
			glDebugMessageCallbackARB_t	ext_glDebugMessageCallbackARB = nullptr;
			glDeleteBuffers_t			ext_glDeleteBuffers = nullptr;
			wglSwapIntervalEXT_t		ext_wglSwapIntervalEXT = nullptr;
			glCreateShader_t			ext_glCreateShader = nullptr;
			glCreateProgram_t			ext_glCreateProgram = nullptr;
			glLinkProgram_t				ext_glLinkProgram = nullptr;
			glAttachShader_t			ext_glAttachShader = nullptr;
			glShaderSource_t			ext_glShaderSource = nullptr;
			glBindAttribLocation_t		ext_glBindAttribLocation = nullptr;
			glUseProgram_t 				ext_glUseProgram = nullptr;
			glDeleteProgram_t			ext_glDeleteProgram = nullptr;
			glDeleteShader_t			ext_glDeleteShader = nullptr;
			glDetachShader_t			ext_glDetachShader = nullptr;
			glGetStringi_t				ext_glGetStringi = nullptr;
			glClearIndex_t				ext_glClearIndex = nullptr;
			glClearDepth_t				ext_glClearDepth = nullptr;
			glMatrixMode_t				ext_glMatrixMode = nullptr;
			glClear_t					ext_glClear = nullptr;
			glPushMatrix_t				ext_glPushMatrix = nullptr;
			glPopMatrix_t				ext_glPopMatrix = nullptr;
			glEnableClientState_t		ext_glEnableClientState = nullptr;
			glDisableClientState_t		ext_glDisableClientState = nullptr;
			glVertexPointer_t			ext_glVertexPointer = nullptr;
			glColorPointer_t			ext_glColorPointer = nullptr;
			glPolygonMode_t				ext_glPolygonMode = nullptr;
			glDrawElements_t			ext_glDrawElements = nullptr;
			glMapBuffer_t				ext_glMapBuffer = nullptr;
			glUnmapBuffer_t				ext_glUnmapBuffer = nullptr;
			glCompileShader_t			ext_glCompileShader = nullptr;
			static GLenum				s_valueNames[static_cast<int>(OpenGLValueNameEnum::MAX_ELEMENT) + 1];
		};
	}
}






#endif /* SOURCES_ENGINE2_INCLUDE_GRAPHIC_OPENGLWRAPPER_HPP_INC */
