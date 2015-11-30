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

		class Directory::Impl final
		{
		public:
			Impl(Impl&&) = delete;
			Impl& operator=(Impl&& source) = delete;
			Impl(Impl const&) = delete;
			Impl& operator=(Impl const& source) = delete;

			Impl(DEFAULT_DIRECTORY mode = DEFAULT_DIRECTORY::CURRENT_DIRECTORY);
			Impl(const std::string& path, bool doGoUp);
			~Impl();
			const std::string& FullPath() const;
			const std::string& Name() const;
			bool Exist() const;
			Directory Parent() const;
			Directory Child(const std::string& name) const;
			Filelist Files() const;
			Filelist Search(const std::string& pattern) const;
			bool Create(bool createParents = false);
			void Refresh();

			bool operator==(const Impl& source) const;

			static std::string NormalizePath(const std::string& path);
			static std::string GetName(const std::string& path);
		private:

			std::string m_fullPath;
			std::string m_name;
			bool m_exist;
		};

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
		// --------------------------------------------------- impl�mentation de Directory::Pimpl
		Directory::Impl::Impl(DEFAULT_DIRECTORY mode)
			: m_fullPath { "" }, m_name { "" }, m_exist { false }
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

		Directory::Impl::Impl(const std::string& path, bool needParent)
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

		Directory::Impl::~Impl()
		{

		}

		const std::string& Directory::Impl::FullPath() const
		{
			return m_fullPath;
		}

		const std::string& Directory::Impl::Name() const
		{
			return m_name;
		}

		bool Directory::Impl::Exist() const
		{
			return m_exist;
		}

		void Directory::Impl::Refresh()
		{
			m_exist = CheckDirectoryExistence(m_fullPath);
		}

		Directory Directory::Impl::Parent() const
		{
			return Directory(m_fullPath, true);
		}

		bool Directory::Impl::operator==(const Directory::Impl& source) const
		{
			return m_fullPath == source.m_fullPath;
		}

		Directory::Filelist Directory::Impl::Search(const std::string& pattern) const
		{
			Filelist result;
			std::string fileName;
			auto handle = InitializeSearch(m_fullPath, pattern);

			while (NextSearch(handle, fileName))
			{
				result.push_back(File(m_fullPath + PATH_SEPARATOR + fileName));
			}

			CloseSearch(handle);
			return result;
		}

		Directory Directory::Impl::Child( const std::string& name ) const
		{
			return Directory(m_fullPath + PATH_SEPARATOR + name);
		}

		bool Directory::Impl::Create( bool createParent )
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

		std::string Directory::Impl::NormalizePath(const std::string& path)
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

		std::string Directory::Impl::GetName(const std::string& path)
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

		Directory::Filelist Directory::Impl::Files() const
		{
			return Search("*");
		}



		// --------------------------------------------------- impl�mentation de Directory
		Directory::Directory(DEFAULT_DIRECTORY mode)
			: m_impl { CROISSANT_NEW Impl { mode } }
		{
		}

		Directory::Directory(std::string const& path)
			: m_impl { CROISSANT_NEW Impl { path, false } }
		{
		}

		Directory::Directory(std::string const& path, bool needParent)
			: m_impl { CROISSANT_NEW Impl { path, needParent } }
		{
		}

		Directory::~Directory()
		{
		}

		std::string const& Directory::FullPath() const
		{
			return m_impl->FullPath();
		}

		std::string const& Directory::Name() const
		{
			return m_impl->Name();
		}

		bool Directory::Exist() const
		{
			return m_impl->Exist();
		}

		void Directory::Refresh()
		{
			m_impl->Refresh();
		}

		Directory Directory::Parent() const
		{
			return m_impl->Parent();
		}

		bool Directory::operator==(Directory const& source) const
		{
			return *(m_impl.get()) == *(source.m_impl.get());
		}

		Directory::Filelist Directory::Search(std::string const& pattern) const
		{
			return m_impl->Search(pattern);
		}

		Directory Directory::Child( std::string const& name ) const
		{
			return m_impl->Child(name);
		}

		bool Directory::Create( bool createParent )
		{
			return m_impl->Create(createParent);
		}

		std::string Directory::NormalizePath(std::string const& path)
		{
			return Directory::Impl::NormalizePath(path);
		}

		std::string Directory::GetName(std::string const& path)
		{
			return Directory::Impl::GetName(path);
		}

		Directory::Filelist Directory::Files() const
		{
			return m_impl->Files();
		}
	}
}
