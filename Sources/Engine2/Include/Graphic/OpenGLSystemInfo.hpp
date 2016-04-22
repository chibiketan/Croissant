/*
 * OpenGLSystemInfo.hpp
 *
 *  Created on: 6 févr. 2015
 *      Author: Nadege
 */

#ifndef OPENGLSYSTEMINFO_HPP_INC
#define OPENGLSYSTEMINFO_HPP_INC

#include "Engine.hpp"
#include "Core/LogManager.hpp"
#include <cstdint>
#include <memory>

namespace Croissant
{
	namespace Core
	{
		class LogManager;
	}

	namespace Graphic
	{
		class OpenGLWrapper;

		class ENGINE_API OpenGLSystemInfo final
		{
		public:
			OpenGLSystemInfo(OpenGLWrapper const& wrapper);
			/// <summary>Récupère le numéro de version major d'OpenGL supportée</summary>
			uint32_t GetMajorVersion() const;
			/// <summary>Récupère le numéro de version mineur d'OpenGL supportée</summary>
			uint32_t GetMinorVersion() const;
			/// <summary>Récupère la taille maximale pour une texture 3D</summary>
			uint32_t GetMax3DTextureSize() const;
			/// <summary>Récupère la distance maximum pour le clipping</summary>
			uint32_t GetMaxClipDistance() const;
			/// <summary>Récupère le nombre maximal de buffer de dessin</summary>
			uint32_t GetMaxDrawBuffer() const;
			/// <summary>Récupère la taille maximale pour un buffer d'indices</summary>
			uint32_t GetMaxIndicesArraySize() const;
			/// <summary>Récupère la taille maximale pour un buffer de vertex</summary>
			uint32_t GetMaxVertexArraySize() const;
			/// <summary>Récupère la taille maximal pour un buffer de rendu</summary>
			uint32_t GetMaxRenderBufferSize() const;
			/// <summary>Récupère le nombre d'extention OpenGL supportées par ce contexte</summary>
			uint32_t GetNumExtension() const;

		private:
			Core::LogManager::Log		m_logManager;
			OpenGLWrapper const&	m_wrapper;
			uint32_t				m_major = 0;
			uint32_t				m_minor = 0;
			uint32_t				m_max3DTextureSize = 0;
			uint32_t				m_maxClipDistance = 0;
			uint32_t				m_maxDrawBuffer = 0;
			uint32_t				m_maxIndicesArraySize = 0;
			uint32_t				m_maxVertexArraySize = 0;
			uint32_t				m_maxRenderbufferSize = 0;
			uint32_t				m_numExtensions = 0;
		};
	}
}

#endif /* OPENGLSYSTEMINFO_HPP_INC */
