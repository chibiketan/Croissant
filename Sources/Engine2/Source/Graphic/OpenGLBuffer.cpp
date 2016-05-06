#include "Graphic/OpenGLBuffer.hpp"
#include "Graphic/OpenGLWrapper.hpp"

#undef max
#define CAST(p) static_cast<size_t>(p)

namespace Croissant
{
	namespace Graphic
	{
		std::array<OpenGLMapBufferAccessEnum, CAST(Core::BufferAccessEnum::MAX_ELEMENT) + 1> sp_access {
			OpenGLMapBufferAccessEnum::ReadOnly,
			OpenGLMapBufferAccessEnum::WriteOnly,
			OpenGLMapBufferAccessEnum::ReadWrite
		};

		OpenGLBuffer::OpenGLBuffer(OpenGLWrapper& m_wrapper, uint32_t bufferSize, OpenGLBufferTargetEnum target, OpenGLBufferUsageEnum usage)
			: m_wrapper{ m_wrapper }, m_bufferId{ std::numeric_limits<uint32_t>::max() },
			m_target { target }
		{
			
			m_wrapper.GenBuffers(1, &m_bufferId);
			m_wrapper.BindBuffer(target, m_bufferId);
			m_wrapper.BufferData(target, bufferSize, nullptr, usage);
		}

		void* OpenGLBuffer::Map(Core::BufferAccessEnum access) const
		{
			m_wrapper.BindBuffer(m_target, m_bufferId);
			auto buffer = m_wrapper.MapBuffer(m_target, sp_access[CAST(access)]);

			return buffer;
		}

		void OpenGLBuffer::Unmap() const
		{
			m_wrapper.BindBuffer(m_target, m_bufferId);
			m_wrapper.UnmapBuffer(m_target);
		}
	}
}