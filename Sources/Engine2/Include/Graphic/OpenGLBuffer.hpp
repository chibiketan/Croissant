#ifndef CROISSANT_ENGINE_GRAPHIC_OPENGLBUFFER_HPP_INC
#  define CROISSANT_ENGINE_GRAPHIC_OPENGLBUFFER_HPP_INC

#  include "Engine.hpp"
#  include "Core/AbstractBuffer.hpp"

namespace Croissant
{
	namespace Graphic
	{
		class OpenGLWrapper;

		class ENGINE_API OpenGLBuffer : public Core::AbstractBuffer
		{
		public:

			explicit OpenGLBuffer(OpenGLWrapper& m_wrapper);

			int32_t GetBufferId() override;

		protected:
			void* MapInternal() override;
			void UnmapInternal() override;

		private:
			OpenGLWrapper&	m_wrapper;
			uint32_t		m_bufferId;
		};

	}
}

#endif // !CROISSANT_ENGINE_GRAPHIC_OPENGLBUFFER_HPP_INC