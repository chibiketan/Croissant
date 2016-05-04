#include "Graphic/OpenGLBuffer.hpp"
#include "Graphic/OpenGLWrapper.hpp"

#undef max

namespace Croissant
{
	namespace Graphic
	{
		OpenGLBuffer::OpenGLBuffer(OpenGLWrapper& m_wrapper, uint32_t bufferSize)
			: m_wrapper{ m_wrapper }, m_bufferId{ std::numeric_limits<uint32_t>::max() }
		{
			// TODO gen buffer
			m_wrapper.GenBuffers(1, &m_bufferId);
			m_wrapper.BindBuffer(GL_ARRAY_BUFFER, m_bufferId);
			m_wrapper.BufferData(GL_ARRAY_BUFFER, bufferSize, nullptr, GL_STATIC_DRAW);
			
			//opengl.BufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		}

	}
}