#include "Debug/MemoryManager.hpp"
#include "FileSystem/Directory.hpp"
#include "FileSystem/File.hpp"
//#include <direct.h>
#include <string>

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

		}
	}
}

#if defined(CROISSANT_WINDOWS)
#  include "../Source/Specific/FileSystem/Directory.win.cpp"
#elif defined(CROISSANT_LINUX)
#  include "../Source/Specific/FileSystem/Directory.uni.cpp"
#else
#  error "Plateforme non prise en charge"
#endif
namespace Croissant
{
	namespace FileSystem
	{
		namespace
		{
			std::string GetUpFolder(const std::string& path)
			{
				bool first = true;
				auto begin = path.rbegin();
				auto end = path.rend();

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
					end = path.rend() - 3;
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

				return std::string(path.begin(), begin.base());
			}

			std::string RTrimSeparator(const std::string& path)
			{
				if (3 == path.length() && ':' == path[1])
				{
					return path;
				}

				// TODO: v�rifier si c'est la base d'un chemin r�seau et ne pas enlever le dernier s�parateur si c'est le cas

				auto begin = path.rbegin();
				char chr = *begin;

				if (!SEPARATOR_TEST(chr))
				{
					return path;
				}

				auto end = path.rend();

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

				return std::string(path.begin(), begin.base());
			}


		}
		// --------------------------------------------------- implémentation de Directory
		Directory::Directory(DEFAULT_DIRECTORY mode)
		{
			if (DEFAULT_DIRECTORY::CURRENT_DIRECTORY == mode)
			{
				char* path = GetWorkingDirectory();

				m_fullPath = path;
				free(path);
			}
			else if (DEFAULT_DIRECTORY::PROGRAM_DIRECTORY == mode)
			{
				char fullPath[MAX_PATH];

				GetProgramPath(fullPath);
				m_fullPath = GetUpFolder(fullPath);
				TRACE("Création d'un objet pour le dossier contenant l'application : ");
				TRACE(fullPath);
				TRACE(m_fullPath.c_str());
			}

			m_name = GetName(m_fullPath);
			Refresh();
		}

		Directory::Directory(std::string const& path)
			: Directory(path, false)
		{
		}

		Directory::Directory(std::string const& path, bool needParent)
			: m_fullPath { "" }, m_name { "" }, m_exist { false }
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

		std::string const& Directory::FullPath() const
		{
			return m_fullPath;
		}

		std::string const& Directory::Name() const
		{
			return m_name;
		}

		bool Directory::Exist() const
		{
			return m_exist;
		}

		void Directory::Refresh()
		{
			m_exist = CheckDirectoryExistence(m_fullPath);
		}

		Directory Directory::Parent() const
		{
			return Directory(m_fullPath, true);
		}

		bool Directory::operator==(Directory const& source) const
		{
			return m_fullPath == source.m_fullPath;
		}

		Directory::Filelist Directory::Search(std::string const& pattern) const
		{
			Filelist result;
			std::string fileName;
			auto handle = InitializeSearch(m_fullPath, pattern);

			while (NextSearch(handle, fileName))
			{
				result.emplace_back(m_fullPath + PATH_SEPARATOR + fileName);
			}

			CloseSearch(handle);
			return result;
		}

		Directory Directory::Child( std::string const& name ) const
		{
			return Directory(m_fullPath + PATH_SEPARATOR + name);
		}

		bool Directory::Create( bool createParent )
		{
			TRACE("Début création de dossier pour ");
			TRACE(FullPath().c_str());
			if (m_exist)
			{
				TRACE("dossier existe");
				return false;
			}

			auto parent = Parent();

			if (!parent.Exist())
			{
				TRACE("Le dossier Parent n'existe pas");
				if (!createParent || !parent.Create())
				{
					TRACE("Impossible de créer le dossier parent");
					return false;
				}
			}

			TRACE("Appel de la création interne pour le dossier");
			return CreateDirectoryInternal(FullPath());
		}

		std::string Directory::NormalizePath(std::string const& path)
		{
			// TODO: faire disparaitre les appels au parent dans les chemins (../, ./)
			std::string result;
			auto begin = path.begin();
			auto current = path.begin();
			auto end = path.end();
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
				Directory tmpDir(DEFAULT_DIRECTORY::CURRENT_DIRECTORY);

				result.reserve(path.length() + 1 + tmpDir.FullPath().length());
				result.append(tmpDir.FullPath());
				result.append(1, PATH_SEPARATOR);
			}
#ifndef CROISSANT_LINUX
			else if (PATH_ABOLUTE_WITHOUT_LETTER == kind)
			{
				Directory tmpDir(DEFAULT_DIRECTORY::CURRENT_DIRECTORY);

				result.reserve(path.length() + 2);
				result.append(tmpDir.FullPath().substr(0, 2));
			}
#endif
			else
			{
				result.reserve(path.length());
			}

			for (begin = path.begin(); current != end; ++current)
			{
				char chr = *current;
				if (SEPARATOR_TEST(chr))
				{
					if (!processSep)
					{
						result.append(begin, current);
						result.append(1, PATH_SEPARATOR);
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
				result.append(begin, end);
			}

			if (result.length() == 2 && ':' == result[1])
			{
				result.append(1, PATH_SEPARATOR);
			}

			return result;
		}

		std::string Directory::GetName(std::string const& path)
		{
			auto begin = path.rbegin();
			auto end = path.rend();
			auto searchEnd = path.end();
			bool first = true;

			for (; begin != end; ++begin)
			{
				char chr = *begin;

				if (SEPARATOR_TEST(chr))
				{
					if (first)
					{
						searchEnd = begin.base();
					}
					else
					{
						break;
					}
				}

				first = false;
			}

			return std::string(begin.base(), searchEnd);
		}

		Directory::Filelist Directory::Files() const
		{
			return Search("*");
		}
	}
}
