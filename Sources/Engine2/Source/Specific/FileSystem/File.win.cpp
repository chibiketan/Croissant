#include <windows.h>
#include <string>

#define PATH_SEPARATOR "\\"

namespace Croissant
{
	namespace FileSystem
	{
		namespace
		{
			bool DoFileExist(const std::string& path)
			{
				DWORD attr = GetFileAttributes(path.c_str());

				if (INVALID_FILE_ATTRIBUTES == attr)
				{
					// TODO check errors ?
					return false;
				}
				else if((attr & FILE_ATTRIBUTE_DIRECTORY) == 0)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}
	}
}
