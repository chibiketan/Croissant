#include "stdafx.hpp"
#include "FileSystem/Directory.hpp"
#include "FileSystem/File.hpp"
#include <string>

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <libgen.h>
#include <sys/stat.h>
#include <regex>
#include <dirent.h>
#include <stdexcept>

#define MAX_PATH 255
#define PATH_SEPARATOR "/"
#define SEPARATOR_TEST(chr) ('/' == (chr))

namespace Croissant
{
	namespace FileSystem
	{
		namespace
		{
			enum PathKind
			{
				PATH_RELATIVE,
				PATH_ABSOLUTE
			};

			String GetUpFolder(const String& path)
			{
				bool first = true;
				auto begin = path.RBegin();
				auto end = path.REnd();

				for (; begin != end; ++begin)
				{
					auto chr = *begin;

					if (SEPARATOR_TEST(chr))
					{
						if ((begin + 1) == end)
						{
							break;
						}

						if (!first)
						{
							++begin;
							break;
						}
					}

					first = false;
				}

				return String(path.Begin(), begin.Base());
			}

			String RTrimSeparator(const String& path)
			{
				auto begin = path.RBegin();
				char chr = *begin;

				if (!SEPARATOR_TEST(chr))
				{
					return path;
				}

				auto end = path.REnd();

				while (SEPARATOR_TEST(chr))
				{
					auto next = begin + 1;

					if (next == end)
					{
						break;
					}

					chr = *next;
					begin = next;
				}

				return String(path.Begin(), begin.Base());
			}

		}

		class Directory::Pimpl
		{
		public:
			Pimpl(Directory::DEFAULT_DIRECTORY mode);
			Pimpl(const String& path, bool needParent);
			~Pimpl();
			const String& FullPath() const;
			const String& Name() const;
			bool Exist() const;
			Directory Parent() const;
			Directory Child(const String& name) const;
			Directory::Filelist Search(const String& pattern) const;
			bool Create(bool createParents);
			bool operator==(const Pimpl& source) const;

		private:
			String m_fullPath;
			String m_name;
			bool m_exist;

			void Refresh();
		};

		// ------------------------------- Directory::Pimpl impl�mentation

		Directory::Pimpl::Pimpl(Directory::DEFAULT_DIRECTORY mode)
			: m_fullPath(""), m_name(""), m_exist(false)
		{
			if (Directory::CURRENT_DIRECTORY == mode)
			{
				char path[MAX_PATH];

				memset(path, 0, MAX_PATH);
				getcwd(path, MAX_PATH);
				// TODO check path for nullptr value
				m_fullPath = path;
			}
			else if (Directory::PROGRAM_DIRECTORY == mode)
			{
				char appPath[MAX_PATH];

                ::memset(appPath, 0, MAX_PATH);
                ::ssize_t length = ::readlink("/proc/self/exe", appPath, MAX_PATH);

                if (-1 == length)
                {
					// impossible de lire le lien...
					throw String("impossible de lire /proc/self/exe");
				}

				char* folderPath = ::dirname(appPath);
				m_fullPath = folderPath;
			}

			m_name = GetName(m_fullPath);
			Refresh();
		}

		Directory::Pimpl::Pimpl(const String& path, bool needParent)
			: m_fullPath(""), m_name(""), m_exist(false)
		{
			if (needParent)
			{
				m_fullPath = GetUpFolder(path);
			}
			else
			{
				m_fullPath = RTrimSeparator(NormalizePath(path));
			}
		
			m_name = GetName(m_fullPath);
			Refresh();
		}

		Directory::Pimpl::~Pimpl()
		{

		}

		const String& Directory::Pimpl::FullPath() const
		{
			return m_fullPath;
		}

		const String& Directory::Pimpl::Name() const
		{
			return m_name;
		}

		bool Directory::Pimpl::Exist() const
		{
			return m_exist;
		}

		void Directory::Pimpl::Refresh()
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
			else if (S_ISDIR(st.st_mode))
			{
				m_exist = true;
			}
			else
			{
				m_exist = false;
			}
		}

		Directory Directory::Pimpl::Parent() const
		{
			return Directory(CROISSANT_NEW Directory::Pimpl(m_fullPath, true));
		}

		bool Directory::Pimpl::operator==(const Directory::Pimpl& source) const
		{
			return m_fullPath == source.m_fullPath;
		}

		Directory::Filelist Directory::Pimpl::Search(const String& pattern) const
		{
			Filelist result;
			std::regex test(pattern.Str());

			if (!Exist())
			{
				return result;
			}

			auto dir = opendir(m_fullPath.Str());

			if (nullptr == dir)
			{
				throw std::runtime_error((String("une erreur est survenue en ouvrant le dossier : ") + m_fullPath).Str());
			}

			while (dir)
			{
				dirent entry;
				dirent* tmpEntry = nullptr;
				if (0 != readdir_r(dir, &entry, &tmpEntry))
				{
					closedir(dir);
					throw std::runtime_error("une erreur est survenue lors du parcourt du dossier");
				}

				if (nullptr == tmpEntry)
				{
					// tous les fichiers sont lus
					break;
				}

				if (String(entry.d_name) == String("..") || String(entry.d_name) == String("."))
				{
					continue;
				}

				if (std::regex_match(entry.d_name, test))
				{
					// la regex match !
					File tmpFile(m_fullPath + PATH_SEPARATOR + entry.d_name);
					if (tmpFile.Exist())
					{
						result.push_back(std::move(tmpFile));
					}
				}
			}

			closedir(dir);
			return result;
		}

		Directory Directory::Pimpl::Child( const String& name ) const
		{
			return Directory(m_fullPath + PATH_SEPARATOR + name);
		}

		bool Directory::Pimpl::Create( bool createParent )
		{
			if (m_exist)
			{
				return false;
			}

			auto parent = Parent();
			if (!parent.Exist())
			{
				if (!createParent || !parent.Create())
				{
					return false;
				}
			}

			if (-1 == mkdir(FullPath().Str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH))
			{
				return false;
			}

			m_exist = true;
			return true;
		}

		// ------------------------------- Directory implementation

		String Directory::NormalizePath(const String& path)
		{
			// TODO: faire disparaitre les appels au parent dans les chemins (../, ./)
			String result;

			auto begin = path.Begin();
			auto current = path.Begin();
			auto end = path.End();
			bool processSep = false;
			PathKind kind = PATH_RELATIVE;

			if (begin != end)
			{
				char chr = *begin;

				if (SEPARATOR_TEST(chr))
				{
					kind = PATH_ABSOLUTE;
				}
			}

			if (PATH_RELATIVE == kind)
			{
				Directory tmpDir(Directory::CURRENT_DIRECTORY);

				result.Reserve(path.Length() + 1 + tmpDir.FullPath().Length());
				result.Append(tmpDir.FullPath());
				result.Append(1, '/');
			}
			else
			{
				result.Reserve(path.Length());
			}

			for (begin = path.Begin(); current != end; ++current)
			{
				char chr = *current;
				if (SEPARATOR_TEST(chr))
				{
					if (!processSep)
					{
						result.Append(begin, current);
						result.Append(1, '/');
						begin = current + 1;
						processSep = true;
					}
					else
					{
						begin = current + 1;
					}
				}
				else
				{
					processSep = false;
				}
			}

			if (!processSep)
			{
				result.Append(begin, end);
			}

			if (result.Length() == 2 && ':' == result[1])
			{
				result.Append(1, '/');
			}

			return std::move(result);
		}

		String Directory::GetName(const String& path)
		{
			auto begin = path.RBegin();
			auto end = path.REnd();
			auto searchEnd = path.End();
			bool first = true;

			for (; begin != end; ++begin)
			{
				char chr = *begin;

				if (SEPARATOR_TEST(chr))
				{
					if (first)
					{
						searchEnd = begin.Base();
					}
					else
					{
						break;
					}
				}

				first = false;
			}

			return String(begin.Base(), searchEnd);
		}

		Directory::Directory(Directory::DEFAULT_DIRECTORY mode) : m_pimpl(nullptr)
		{
			m_pimpl = CROISSANT_NEW Directory::Pimpl(mode);
		}

		Directory::Directory(const String& path)
		{
			m_pimpl = CROISSANT_NEW Directory::Pimpl(path, false);
		}

		Directory::Directory(Directory::Pimpl* pimpl)
			: m_pimpl(pimpl)
		{

		}

		Directory::Directory(Directory&& source)
			: m_pimpl(source.m_pimpl)
		{
			source.m_pimpl = nullptr;
		}

		Directory::Directory(const String& path, bool doGoUp)
			: m_pimpl(nullptr)
		{
			m_pimpl = CROISSANT_NEW Directory::Pimpl(path, doGoUp);
		}

		Directory::~Directory()
		{
			if (nullptr != m_pimpl)
			{
				CROISSANT_DELETE(m_pimpl);
				m_pimpl = nullptr;
			}
		}

		const String& Directory::FullPath() const
		{
			return m_pimpl->FullPath();

		}

		const String& Directory::Name() const
		{
			return m_pimpl->Name();
		}

		bool Directory::Exist() const
		{
			return m_pimpl->Exist();
		}

		Directory Directory::Parent() const
		{
			return m_pimpl->Parent();
		}

		Directory::Filelist Directory::Files() const
		{
			return m_pimpl->Search(".*");
		}

		Directory::Filelist Directory::Search(const String& pattern) const
		{
			return m_pimpl->Search(String(pattern).Replace("*", ".*"));
		}

		Directory& Directory::operator=(Directory&& source)
		{
			if (this != &source)
			{
				m_pimpl = source.m_pimpl;
				source.m_pimpl = nullptr;
			}
			return *this;
		}

		bool Directory::operator==(const Directory& source) const
		{
			return (*m_pimpl) == (*source.m_pimpl);
		}

		Directory Directory::Child( const String& name ) const
		{
			return m_pimpl->Child(name);
		}

		bool Directory::Create( bool createParent /*= false*/ )
		{
			return m_pimpl->Create(createParent);
		}
	}
}
