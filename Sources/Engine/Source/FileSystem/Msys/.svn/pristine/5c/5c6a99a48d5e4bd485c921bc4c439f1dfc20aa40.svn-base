#include "stdafx.hpp"
#include "FileSystem/Directory.hpp"
#include "FileSystem/File.hpp"
#include <direct.h>
#include <string>

#define SEPARATOR_TEST(chr) ('\\' == (chr) || '/' == (chr))

namespace Croissant
{
	namespace FileSystem
	{
		namespace
		{
			enum PathKind
			{
				PATH_RELATIVE,
				PATH_ABSOLUTE,
				PATH_ABOLUTE_WITHOUT_LETTER
			};

			String GetUpFolder(const String& path)
			{
				bool first = true;
				auto begin = path.RBegin();
				auto end = path.REnd();

				if (SEPARATOR_TEST(path[0]))
				{
					if (SEPARATOR_TEST(path[1]))
					{
						/* UNC path (Ex : \\test-serv\)
						   TODO: redefine end to the end of the UNC name */
					}
				}
				else if(':' == path[1]
					&& SEPARATOR_TEST(path[2]))
				{
					// absolute path (ex : C:\)
					end = path.REnd() - 3;
				}

				for (; begin != end; ++begin)
				{
					char chr = *begin;

					if (SEPARATOR_TEST(chr))
					{
						if ((begin + 1) == end)
						{
							break;
						}

						if (first && ':' == *(begin + 1))
						{
							break;
						}
						else if (!first)
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
				if (3 == path.Length() && ':' == path[1])
				{
					return path;
				}

				// TODO: vérifier si c'est la base d'un chemin réseau et ne pas enlever le dernier séparateur si c'est le cas

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

		// ------------------------------- Directory::Pimpl implémentation

		Directory::Pimpl::Pimpl(Directory::DEFAULT_DIRECTORY mode)
			: m_fullPath(""), m_name(""), m_exist(false)
		{
			if (Directory::CURRENT_DIRECTORY == mode)
			{
				char* path = nullptr;

				path = _getcwd(path, 0);
				// TODO check path for nullptr value
				m_fullPath = path;
				free(path);
			}
			else if (Directory::PROGRAM_DIRECTORY == mode)
			{
				HMODULE hmodule = GetModuleHandle(nullptr);
				char path[MAX_PATH];
				char fullPath[MAX_PATH];

				GetModuleFileName(hmodule, path, MAX_PATH);
				_fullpath(fullPath, path, MAX_PATH);

				m_fullPath = GetUpFolder(fullPath);
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
			DWORD attr = GetFileAttributes(m_fullPath.Str());
			if (INVALID_FILE_ATTRIBUTES == attr)
			{
				// TODO check errors ?
				m_exist = false;
			}
			else if((attr & FILE_ATTRIBUTE_DIRECTORY) != 0)
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
			return Directory(new Directory::Pimpl(m_fullPath, true));
		}

		bool Directory::Pimpl::operator==(const Directory::Pimpl& source) const
		{
			return m_fullPath == source.m_fullPath;
		}

		Directory::Filelist Directory::Pimpl::Search(const String& pattern) const
		{
			Filelist result;
			WIN32_FIND_DATA data;
			auto handle = FindFirstFile((m_fullPath + "\\" + pattern).Str(), &data);

			if (INVALID_HANDLE_VALUE != handle)
			{
				do
				{
					if (0 == (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
					{
						result.push_back(File(m_fullPath + "\\" + data.cFileName));
					}

				} while (FindNextFile(handle, &data));

				FindClose(handle);
			}

			//return std::move(result);
			return result;
		}

		Directory Directory::Pimpl::Child( const String& name ) const
		{
			return Directory(m_fullPath + "\\" + name);
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

			if (FALSE == CreateDirectory(FullPath().Str(), nullptr))
			{
				return false;
			}

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
				auto chr = *begin;

				if (SEPARATOR_TEST(chr))
				{
					if ((++begin) != end
						&& SEPARATOR_TEST(*begin))
					{
						// UNC path : //my_serv/
						kind = PATH_ABSOLUTE;
					}
					else
					{
						// absolute path without letter : /test
						kind = PATH_ABOLUTE_WITHOUT_LETTER;
					}
				}
				else
				{
					if ((++begin) != end
						&& ':' == (*begin)
						&& (++begin) != end
						&& SEPARATOR_TEST(*begin))
					{
						// absolute path with letter : C:/
						kind = PATH_ABSOLUTE;
					}
				}
			}

			if (PATH_RELATIVE == kind)
			{
				Directory tmpDir(Directory::CURRENT_DIRECTORY);

				result.Reserve(path.Length() + 1 + tmpDir.FullPath().Length());
				result.Append(tmpDir.FullPath());
				result.Append(1, '\\');
			}
			else if (PATH_ABOLUTE_WITHOUT_LETTER == kind)
			{
				Directory tmpDir(Directory::CURRENT_DIRECTORY);

				result.Reserve(path.Length() + 2);
				result.Append(tmpDir.FullPath().SubString(0, 2));
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
						result.Append(1, '\\');
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
				result.Append(1, '\\');
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
			return m_pimpl->Search("*");
		}

		Directory::Filelist Directory::Search(const String& pattern) const
		{
			return m_pimpl->Search(pattern);
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
