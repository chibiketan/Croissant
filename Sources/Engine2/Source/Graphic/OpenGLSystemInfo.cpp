/*
 * OpenGLSystemInfo.cpp
 *
 *  Created on: 6 févr. 2015
 *      Author: Nadege
 */

#include "Debug/MemoryManager.hpp"
#include "Graphic/OpenGLSystemInfo.hpp"
#include "Graphic/OpenGLWrapper.hpp"
#include "Core/LogManager.hpp"
#include <GL/gl.h>
#include <GL/glext.h>
#include <string>


#define LOGGER_NAME "OpenGLSystemInfo"

namespace Croissant
{
	namespace Graphic
	{
		OpenGLSystemInfo::OpenGLSystemInfo(OpenGLWrapper const& wrapper)
			: m_logManager{ CROISSANT_GET_LOG(OpenGLSystemInfo) }, m_wrapper(wrapper)
		{
			m_logManager->Write("Entrée dans OpenGLSystemInfo constructeur");
			m_major = m_wrapper.GetInteger(OpenGLValueNameEnum::MajorVersion);
			m_minor = m_wrapper.GetInteger(OpenGLValueNameEnum::MinorVersion);
			m_max3DTextureSize = m_wrapper.GetInteger(OpenGLValueNameEnum::Max3DTextureSize);
			m_maxClipDistance = m_wrapper.GetInteger(OpenGLValueNameEnum::MaxClipDistances);
			m_maxDrawBuffer = m_wrapper.GetInteger(OpenGLValueNameEnum::MaxDrawBuffers);
			m_maxIndicesArraySize = m_wrapper.GetInteger(OpenGLValueNameEnum::MaxElementsIndices);
			m_maxVertexArraySize = m_wrapper.GetInteger(OpenGLValueNameEnum::MaxElementsVertices);
			m_maxRenderbufferSize = m_wrapper.GetInteger(OpenGLValueNameEnum::MaxRenderbufferSize);
			m_numExtensions = m_wrapper.GetInteger(OpenGLValueNameEnum::NumExtentions);

			// TODO : get extension list
			GLboolean doubleBufferEnabled;
			glGetBooleanv(GL_DOUBLEBUFFER, &doubleBufferEnabled);

			if (doubleBufferEnabled == 1)
			{
				m_logManager->Write(LOGGER_NAME, "double buffer supporté.");
			}
			else
			{
				m_logManager->Write(LOGGER_NAME, "double buffer non supporté.");
			}

			m_logManager->Write("Sortie de OpenGLSystemInfo constructeur");
		}

		uint32_t OpenGLSystemInfo::GetMajorVersion() const
		{
			return m_major;
		}

		uint32_t OpenGLSystemInfo::GetMinorVersion() const
		{
			return m_minor;
		}

		uint32_t OpenGLSystemInfo::GetMax3DTextureSize() const
		{
			return m_max3DTextureSize;
		}

		uint32_t OpenGLSystemInfo::GetMaxClipDistance() const
		{
			return m_maxClipDistance;
		}

		uint32_t OpenGLSystemInfo::GetMaxDrawBuffer() const
		{
			return m_maxDrawBuffer;
		}

		uint32_t OpenGLSystemInfo::GetMaxIndicesArraySize() const
		{
			return m_maxIndicesArraySize;
		}

		uint32_t OpenGLSystemInfo::GetMaxVertexArraySize() const
		{
			return m_maxVertexArraySize;
		}

		uint32_t OpenGLSystemInfo::GetMaxRenderBufferSize() const
		{
			return m_maxRenderbufferSize;
		}

		uint32_t OpenGLSystemInfo::GetNumExtension() const
		{
			return m_numExtensions;
		}
	}
}
