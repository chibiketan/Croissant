#include <sys/stat.h>
#include <string>

#define PATH_SEPARATOR "/"

namespace Croissant
{
	namespace FileSystem
	{
		namespace
		{
			bool DoFileExist(const std::string& path)
			{
				struct stat buffer;

				return stat(path.c_str(), &buffer) == 0;
			}
		}
	}
}
