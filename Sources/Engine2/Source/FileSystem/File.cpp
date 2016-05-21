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
		class File::Pimpl
		{
		public:
			Pimpl() : m_fullPath{}, m_name{}, m_extension{}, m_exist{ false } {}
			~Pimpl() = default;
			std::string m_fullPath;
			std::string m_name;
			std::string m_extension;
			bool m_exist;
		};

		File::File(const std::string& path)
			: m_pimpl{ CROISSANT_NEW Pimpl() }
		{
			TRACE("Création d'un objet pour le fichier :");
			TRACE(path.c_str());
			m_pimpl->m_fullPath = Directory::NormalizePath(path);
			TRACE("Chemin après normailisation :");
			TRACE(m_pimpl->m_fullPath.c_str());
			m_pimpl->m_name = Directory::GetName(m_pimpl->m_fullPath);
			auto dotIndex = m_pimpl->m_name.find_last_of('.');

			if (std::string::npos != dotIndex)
			{
				m_pimpl->m_extension = std::string(m_pimpl->m_name).substr(dotIndex + 1);
				m_pimpl->m_name = m_pimpl->m_name.substr(0, dotIndex);
			}

			Refresh();
		}

		File::File(const std::string& path, const Directory& parent)
			: File(parent.FullPath() + PATH_SEPARATOR + path)
		{
		}

		File::~File()
		{
			CROISSANT_DELETE(m_pimpl);
			m_pimpl = nullptr;
		}

		bool File::Exist() const
		{
			return m_pimpl->m_exist;
		}

		const std::string& File::FullPath() const
		{
			return m_pimpl->m_fullPath;
		}

		const std::string& File::Name() const
		{
			return m_pimpl->m_name;
		}

		const std::string& File::Extension() const
		{
			return m_pimpl->m_extension;
		}

		Directory File::Parent() const
		{
			return Directory(m_pimpl->m_fullPath, true);
		}

		void File::Refresh()
		{
			m_pimpl->m_exist = DoFileExist(m_pimpl->m_fullPath);
		}
	}
}
