#include "stdafx.hpp"
#include "FileSystem/File.hpp"
#include "FileSystem/Directory.hpp"
#include "Core/DebugMemoryManager.hpp"

#include <unistd.h>
#include <sys/stat.h>

#define PATH_SEPARATOR "/"

namespace Croissant
{
	namespace FileSystem
	{
		class File::Pimpl
		{
		public:
			Pimpl(const String& path);
			Pimpl(const String& path, const Directory& parent);
			~Pimpl();
			inline bool Exist() const;
			inline const String& Name() const;
			inline const String& Extension() const;
			inline const String& FullPath() const;
			inline Directory Parent() const;
			inline void Refresh();

		private:
			String m_fullPath;
			String m_name;
			String m_extension;
			bool m_exist;
		};

		// -------------------------------- File::Pimpl impl�mentation

		File::Pimpl::Pimpl(const String& path)
			: m_fullPath(""), m_name(""), m_extension(""), m_exist(false)
		{
			m_fullPath = Directory::NormalizePath(path);
			m_name = Directory::GetName(m_fullPath);
			auto dotIndex = m_name.FindLastIndexOf('.');

			if (String::Npos != dotIndex)
			{
				m_extension = String(m_name).SubString(dotIndex + 1);
				m_name = m_name.SubString(0, dotIndex);
			}

			Refresh();
		}

		File::Pimpl::Pimpl(const String& path, const Directory& parent)
			: Pimpl(parent.FullPath() + PATH_SEPARATOR + path)
		{
		}

		File::Pimpl::~Pimpl()
		{

		}

		inline bool File::Pimpl::Exist() const
		{
			return m_exist;
		}

		inline const String& File::Pimpl::FullPath() const
		{
			return m_fullPath;
		}

		inline const String& File::Pimpl::Name() const
		{
			return m_name;
		}

		inline const String& File::Pimpl::Extension() const
		{
			return m_extension;
		}

		inline Directory File::Pimpl::Parent() const
		{
			return Directory(m_fullPath, true);
		}

		inline void File::Pimpl::Refresh()
		{
			if (-1 == access(m_fullPath.Str(), F_OK))
			{
				m_exist = false;
				return;
			}
			
			struct stat st;

			if (-1 == stat(m_fullPath.Str(), &st))
			{
				// une erreur lors du stat
				m_exist = false;
			}
			else if (S_ISREG(st.st_mode))
			{
				m_exist = true;
			}
			else
			{
				m_exist = false;
			}
		}
		
		// -------------------------------- File impl�mentation

		File::File(const String& path)
			: m_pimpl(nullptr)
		{
			m_pimpl = CROISSANT_NEW File::Pimpl(path);
		}

		File::File(const String& path, const Directory& parent)
			: m_pimpl { nullptr }
		{
			m_pimpl = CROISSANT_NEW File::Pimpl(path, parent);
		}

		File::File(File&& source)
			: m_pimpl(source.m_pimpl)
		{
			source.m_pimpl = nullptr;
		}

		File::~File()
		{
			if (nullptr != m_pimpl)
			{
				CROISSANT_DELETE(m_pimpl);
				m_pimpl = nullptr;
			}
		}

		bool File::Exist() const
		{
			return m_pimpl->Exist();
		}

		const String& File::FullPath() const
		{
			return m_pimpl->FullPath();
		}

		const String& File::Name() const
		{
			return m_pimpl->Name();
		}

		const String& File::Extension() const
		{
			return m_pimpl->Extension();
		}

		Directory File::Parent() const
		{
			return m_pimpl->Parent();
		}

	}
}

