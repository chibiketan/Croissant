#pragma once
#ifndef CROISSANT_ENGINE_GRAPHIC_OPENGLSHADERLOADER_HPP_INC
#define CROISSANT_ENGINE_GRAPHIC_OPENGLSHADERLOADER_HPP_INC

#include "Engine.hpp"
#include "Graphic/Enums.hpp"
#include "FileSystem/File.hpp"

namespace Croissant
{
	namespace Core
	{
		class VertexBufferDescriptor;
	}

	namespace Graphic
	{
		class ENGINE_API OpenGLShaderLoader
		{
		public:
			OpenGLShaderLoader(OpenGLShaderTypeEnum shaderType, FileSystem::File const& file);
			void	GetContent(std::string& content, Core::VertexBufferDescriptor const& descriptor) const;

		private:
			OpenGLShaderTypeEnum	m_shaderType;
			FileSystem::File		m_file;
		};

	}
}

#endif // !CROISSANT_ENGINE_GRAPHIC_OPENGLSHADERLOADER_HPP_INC
