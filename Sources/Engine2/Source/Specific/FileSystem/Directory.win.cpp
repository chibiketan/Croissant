#include <windows.h>
#include <direct.h>
#include <string>

#define PATH_SEPARATOR '\\'
#define SEPARATOR_TEST(chr) ('\\' == (chr) || '/' == (chr))

namespace
{
	char* GetWorkingDirectory()
	{
		char* result;
		
		result = _getcwd(nullptr, 0);
		return result;
	}
		
	void GetProgramPath(char buffer[MAX_PATH])
	{
		char path[MAX_PATH] = {};
		HMODULE hmodule = GetModuleHandle(nullptr);
	
		buffer[0] = '\0';
		GetModuleFileName(hmodule, path, MAX_PATH);
		_fullpath(buffer, path, MAX_PATH);
	}
	
	bool CheckDirectoryExistence(std::string const& path)
	{
		DWORD attr = GetFileAttributes(path.c_str());
		if (INVALID_FILE_ATTRIBUTES == attr)
		{
			// TODO check errors ?
			return false;
		}
		else if((attr & FILE_ATTRIBUTE_DIRECTORY) != 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	struct SearchHandle
	{
		WIN32_FIND_DATA Data;
		HANDLE Handle;
		bool IsFirst = true;
	};

	SearchHandle InitializeSearch(std::string const& baseDir, std::string const& pattern)
	{
		SearchHandle sh;

		sh.Handle = FindFirstFile((baseDir + "\\" + pattern).c_str(), &sh.Data);
		return sh;
	}

	bool NextSearch(SearchHandle& sh, std::string& fileName)
	{
		if (INVALID_HANDLE_VALUE == sh.Handle)
		{
			return false;
		}

		if (sh.IsFirst)
		{
			sh.IsFirst = false;
			if (0 == (sh.Data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				fileName = sh.Data.cFileName;
				return true;
			}
		}

		while (FindNextFile(sh.Handle, &sh.Data))
		{
			if (0 == (sh.Data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				fileName = sh.Data.cFileName;
				return true;
			}
		}

		return false;
	}

	void CloseSearch(SearchHandle& sh)
	{
		if (INVALID_HANDLE_VALUE == sh.Handle)
		{
			return;
		}

		FindClose(sh.Handle);
		sh.Handle = INVALID_HANDLE_VALUE;
	}
	
	bool CreateDirectoryInternal(std::string const& path)
	{
		if (FALSE == CreateDirectory(path.c_str(), nullptr))
		{
			return false;
		}

		return true;
	}

}
