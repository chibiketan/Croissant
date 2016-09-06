#include "Graphic/OpenGLShaderLoader.hpp"
#include "Core/VertexBufferDescriptor.hpp"
#include <fstream>

namespace Croissant
{
	namespace Graphic
	{
		OpenGLShaderLoader::OpenGLShaderLoader(OpenGLShaderTypeEnum shaderType, FileSystem::File const& file)
			: m_shaderType(shaderType), m_file(file)
		{
		}

		void OpenGLShaderLoader::GetContent(std::string& content, Core::VertexBufferDescriptor const& descriptor) const
		{
			static std::string const newLine = "\r\n";
			content = "";

			// Ajout de la version de GLSL demandée
			content += "#version 430" + newLine;

			// Ajout de define suivant le descriptor
			for (auto i = 0; i <= static_cast<int>(Core::VertexComponentEnum::MAX_ELEMENT); ++i)
			{
				auto component = descriptor.GetDescriptor(static_cast<Core::VertexComponentEnum>(i));

				if (!component.IsActive())
				{
					continue;
				}

				switch (static_cast<Core::VertexComponentEnum>(i))
				{
				case Core::VertexComponentEnum::Position:
					content += "#define POSITION";
					break;
				case Core::VertexComponentEnum::Color:
					content += "#define COLOR";
					break;
				default:
					Core::WriteTraceWarn("Valeur de VertexComponentEnum inconnu : " + std::to_string(i));
					continue;
				}

				content += " 1" + newLine;
			}

			// On lit le fichier shader
			auto stream = std::ifstream(m_file.FullPath());
			std::string line;

			while (getline(stream, line))
			{
				content += line + newLine;
			}
		}
	}
}