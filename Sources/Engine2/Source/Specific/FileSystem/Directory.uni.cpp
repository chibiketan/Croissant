//#include <sys/stat.h>
#include <sys/types.h>
//#include <string.h>
//#include <unistd.h>
//#include <libgen.h>
//#include <dirent.h>
//#include <regex>
//#include <stdexcept>

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <libgen.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdexcept>
#include <regex>

#define PATH_SEPARATOR '/'
#define SEPARATOR_TEST(chr) ('/' == (chr))
#define MAX_PATH 500

namespace Croissant
{
	namespace FileSystem
	{
		namespace
		{
			char* GetWorkingDirectory()
			{
				char* result;
				
				result = getcwd(nullptr, 0);
				return result;
			}
		
			void GetProgramPath(char buffer[MAX_PATH])
			{
				char appPath[MAX_PATH] = {};

				memset(buffer, 0, MAX_PATH);
				memset(appPath, 0, MAX_PATH);
				ssize_t length = readlink("/proc/self/exe", appPath, MAX_PATH);

				if (-1 == length)
				{
					throw "impossible de lire le lien symbolique";
				}
	
				//char* folderPath = dirname(appPath);

				//strcpy(buffer, folderPath);
				strcpy(buffer, appPath);
			}
	
			bool CheckDirectoryExistence(std::string const& path)
			{
				DIR* dir = opendir(path.c_str());

				if (dir)
				{
					closedir(dir);
					return true;
				}

				return false;
			}
	
			struct SearchHandle
			{
				std::regex reg;
				DIR* Handle;
				const std::string& baseDir;
				bool IsFirst;
			};

			std::string TransformPatternToRegex(std::string const& pattern)
			{
				std::string result = std::string("^") + pattern + "$";

				int pos = result.find('*');

				while (pos != std::string::npos)
				{
					result.replace(pos, 1, ".*");
					pos = result.find('*', pos + 2);
				}

				return result;
			}

			SearchHandle InitializeSearch(std::string const& baseDir, std::string const& pattern)
			{
				SearchHandle sh { std::regex { TransformPatternToRegex(pattern) }, nullptr, baseDir, true };

				sh.Handle = opendir(baseDir.c_str());
				return sh;
			}

			bool NextSearch(SearchHandle& sh, std::string& fileName)
			{
				while (sh.Handle)
				{
					dirent entry;
					dirent* tmpEntry = nullptr;

					if (0 != readdir_r(sh.Handle, &entry, &tmpEntry))
					{
						closedir(sh.Handle);
						sh.Handle = nullptr;
						throw std::runtime_error("une erreur est survenue lors du parcourt du dossier");
					}

					if (nullptr == tmpEntry)
					{
						// plus de fichier à traiter
						// on sort de la boucle
						break;
					}

					if (std::string(entry.d_name) == std::string("..") || std::string(entry.d_name) == std::string("."))
					{
						// on ignore le répertoire parent et le courant
						continue;
					}

					if (std::regex_match(entry.d_name, sh.reg))
					{
						File tmpFile(sh.baseDir + PATH_SEPARATOR + entry.d_name);

						if (tmpFile.Exist())
						{
							fileName = entry.d_name;
							return true;
						}
					}
				}

				return false;
			}

			void CloseSearch(SearchHandle& sh)
			{
				if (nullptr == sh.Handle)
				{
					return;
				}

				closedir(sh.Handle);
				sh.Handle = nullptr;
			}
			
			bool CreateDirectoryInternal(std::string const& path)
			{
				if (-1 == mkdir(path.c_str(), 0777))
				{
					return false;
				}

				return true;
			}

		}
	}
}
