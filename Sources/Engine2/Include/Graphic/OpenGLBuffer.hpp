#ifndef CROISSANT_ENGINE_GRAPHIC_OPENGLBUFFER_HPP_INC
#  define CROISSANT_ENGINE_GRAPHIC_OPENGLBUFFER_HPP_INC

#  include "Engine.hpp"
#  include "Core/Enums.hpp"
#  include "Graphic/Enums.hpp"
#  include "Core/AbstractBuffer.hpp"

namespace Croissant
{
	namespace Graphic
	{
		class OpenGLWrapper;

		class ENGINE_API OpenGLBuffer : public Core::AbstractBuffer
		{
		public:

			explicit OpenGLBuffer(OpenGLWrapper& m_wrapper, uint32_t bufferSize, OpenGLBufferTargetEnum target, OpenGLBufferUsageEnum usage);

			int32_t GetBufferId() const override;

		protected:
			void* Map(Core::BufferAccessEnum access) const override;
			void Unmap() const override;

		private:
			OpenGLWrapper&			m_wrapper;
			uint32_t				m_bufferId;
			OpenGLBufferTargetEnum	m_target;
		};


		// --------------------------------- implémentation inline
		inline int32_t OpenGLBuffer::GetBufferId() const
		{
			return m_bufferId;
		}

	}
}

#endif // !CROISSANT_ENGINE_GRAPHIC_OPENGLBUFFER_HPP_INC