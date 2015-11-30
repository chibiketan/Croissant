#include "Debug/MemoryManager.hpp"
#include "FileSystem/File.hpp"
#include "FileSystem/Directory.hpp"


#if defined(CROISSANT_WINDOWS)
#  include "../Source/Specific/FileSystem/File.win.cpp"
#elif defined(CROISSANT_LINUX)
#  include "../Source/Specific/FileSystem/File.uni.cpp"
#else
#  error "Plateforme non prise en charge"
#endif

namespace Croissant
{
	namespace FileSystem
	{
		File::File(const std::string& path)
			: m_fullPath(""), m_name(""), m_extension(""), m_exist(false)
		{
			TRACE("Création d'un objet pour le fichier :");
			TRACE(path.c_str());
			m_fullPath = Directory::NormalizePath(path);
			TRACE("Chemin après normailisation :");
			TRACE(m_fullPath.c_str());
			m_name = Directory::GetName(m_fullPath);
			auto dotIndex = m_name.find_last_of('.');

			if (std::string::npos != dotIndex)
			{
				m_extension = std::string(m_name).substr(dotIndex + 1);
				m_name = m_name.substr(0, dotIndex);
			}

			Refresh();
		}

		File::File(const std::string& path, const Directory& parent)
			: File(parent.FullPath() + PATH_SEPARATOR + path)
		{
		}

		File::~File()
		{

		}

		bool File::Exist() const
		{
			return m_exist;
		}

		const std::string& File::FullPath() const
		{
			return m_fullPath;
		}

		const std::string& File::Name() const
		{
			return m_name;
		}

		const std::string& File::Extension() const
		{
			return m_extension;
		}

		Directory File::Parent() const
		{
			return Directory(m_fullPath, true);
		}

		void File::Refresh()
		{
			m_exist = DoFileExist(m_fullPath);
		}
	}
}
