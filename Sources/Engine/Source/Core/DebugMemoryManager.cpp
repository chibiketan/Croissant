#include "stdafx.hpp"
#include <iomanip>

#if defined(__linux)
#  include <unistd.h>
#  include <string.h>
#  include <libgen.h>
#  include <sys/types.h>
#  include <sys/stat.h>

#  define MAX_PATH 255
#endif

#include <errno.h>

#define MAGIC_NUMBER 0xC2FA4576U
#define LOG_FILE_FOR_LEAK "MemoryLeak.log"
#define LOG_FILE_FOR_COMMANDS "MemoryAllocation.log"
#define LOG_FILE_FOr_ERRORS "MemoryErrors.log"

namespace Croissant
{
	namespace Core
	{
		namespace
		{
			bool g_doLogMemory = false;

			struct  MemoryNode
			{
				std::size_t Size;
				const char* File;
				unsigned int Line;
				bool IsArray;
				MemoryNode* Previous;
				MemoryNode* Next;
				uint32_t Magic;
			};

			static MemoryNode* s_memory = NULL;;

			char g_applicationPath[MAX_PATH] = "";

#if defined(__linux)

			char *GetFileRelativeToApp(const char* fileName)
			{
				if ('\0' == g_applicationPath[0])
				{
					char appPath[MAX_PATH];

					::memset(appPath, 0, MAX_PATH);
					::ssize_t length = ::readlink("/proc/self/exe", appPath, MAX_PATH);

					if (-1 == length)
					{
						// impossible de lire le lien...
						throw std::string("impossible de lire /proc/self/exe");
					}

					char* folderPath = ::dirname(appPath);
					::strcpy(g_applicationPath, folderPath);
				}

                char* result = static_cast<char*>(::malloc(MAX_PATH * sizeof(char)));

				::memset(result, 0, MAX_PATH * sizeof(char));
                ::strcpy(result, g_applicationPath);
				::strcat(result, "/Logs/");
				mkdir(result, 0777);
                ::strcat(result, fileName);
				return result;
			}

#elif defined(_WIN32) || defined(__WIN64)

			char *GetFileRelativeToApp(const char* fileName)
			{
				if ('\0' == g_applicationPath[0])
				{
				HMODULE hmodule = ::GetModuleHandle(nullptr);
				char path[MAX_PATH];
				char fullPath[MAX_PATH];
				char directoryPath[_MAX_DIR];
				char drivePath[_MAX_DRIVE];

				::GetModuleFileName(hmodule, path, MAX_PATH);
				::_fullpath(fullPath, path, MAX_PATH);
				::_splitpath_s(fullPath, drivePath, _MAX_DRIVE, directoryPath, _MAX_DIR, nullptr, 0, nullptr, 0);
				::ZeroMemory(g_applicationPath, MAX_PATH);
				::strcpy_s(g_applicationPath, MAX_PATH, drivePath);
				::strcat_s(g_applicationPath, MAX_PATH, directoryPath);
				::strcat_s(g_applicationPath, MAX_PATH, "Logs\\");
				CreateDirectory(g_applicationPath, nullptr);
				}

				char* result = static_cast<char*>(::malloc(MAX_PATH * sizeof(char)));

				::strcpy_s(result, MAX_PATH, g_applicationPath);
				::strcat_s(result, MAX_PATH, fileName);
				return result;
			}

#endif

			void PrintMemoryNotFree(const char* caller, const char* fileName, MemoryNode* node)
			{
				std::ofstream file;
				char* strTime = DebugMemoryManager::GetCurrentDate();
				char* filePath = GetFileRelativeToApp(fileName);

				file.open(filePath, std::ios::app);
				if (nullptr != node->File && node->IsArray)
				{
					file << strTime << " : " << std::left << std::setw(7) << "ERROR" << " : " << std::setw(50) << caller << " :  allocation [] of " << node->Size << " octet is not free || " << node->File << ": " << node->Line << std::endl;
				}
				else if (nullptr != node->File)
				{
					file << strTime << " : " << std::left << std::setw(7) << "ERROR" << " : " << std::setw(50) << caller << " : allocation of " << node->Size << " octet is not free || " << node->File << ": " << node->Line << std::endl;
				}
				else if (node->IsArray)
				{
					file << strTime << " : " << std::left << std::setw(7) << "ERROR" << " : " << std::setw(50) << caller << " : allocation [] of " << node->Size << " octet is not free" << std::endl;
				}
				else
				{
					file << strTime << " : " << std::left << std::setw(7) << "ERROR" << " : " << std::setw(50) << caller << " : allocation of " << node->Size << " octet is not free" << std::endl;
				}

				file.close();
				::free(filePath);
				::free(strTime);
			}

			//std::ofstream g_allocationFile;

			//void PrintMemoryAllocated(const char* caller, const char* fileName, MemoryNode* node)
			//{
			//	if (g_doLogMemory)
			//	{
			//		char* strDate = DebugMemoryManager::GetCurrentDate();
			//
			//		if (!g_allocationFile.is_open())
			//		{
			//			char* filePath = GetFileRelativeToApp(fileName);
			//
			//			g_allocationFile.open(filePath, std::ios::app);
			//			::free(filePath);
			//		}
			//
			//		if (NULL != node->File && node->IsArray)
			//		{
			//			g_allocationFile << strDate << " : " << std::left << std::setw(7) << "DEBUG" << " : " << std::setw(50) << caller << " : new[] with size " << node->Size << " || " << node->File << ":" << node->Line << std::endl;
			//		}
			//		else if (NULL != node->File)
			//		{
			//			g_allocationFile << strDate << " : " << std::left << std::setw(7) << "DEBUG" << " : " << std::setw(50) << caller << " : new with size " << node->Size << " || " << node->File << ":" << node->Line << std::endl;
			//		}
			//		else if (node->IsArray)
			//		{
			//			g_allocationFile << strDate << " : " << std::left << std::setw(7) << "DEBUG" << " : " << std::setw(50) << caller << " : new[] with size " << node->Size << std::endl;
			//		}
			//		else
			//		{
			//			g_allocationFile << strDate << " : " << std::left << std::setw(7) << "DEBUG" << " : " << std::setw(50) << caller << " : new with size " << node->Size << std::endl;
			//		}
			//
			//		::free(strDate);
			//	}
			//}

			//void PrintMemoryDeallocated(const char* caller, const char* fileName, MemoryNode* node, const char* file, unsigned int line)
			//{
			//	if (g_doLogMemory)
			//	{
			//		if (!g_allocationFile.is_open())
			//		{
			//			char* filePath = GetFileRelativeToApp(fileName);
			//
			//			g_allocationFile.open(filePath, std::ios::app);
			//			::free(filePath);
			//		}
			//
			//		if (g_allocationFile.is_open())
			//		{
			//			char* strDate = DebugMemoryManager::GetCurrentDate();
			//
			//			if (NULL != file && node->IsArray)
			//			{
			//				g_allocationFile << strDate << " : " << std::left << std::setw(7) << "DEBUG" << " : " << std::setw(50) << caller << " : delete[] with size " << node->Size << " || " << file << ":" << line << std::endl;
			//			}
			//			else if (NULL != file)
			//			{
			//				g_allocationFile << strDate << " : " << std::left << std::setw(7) << "DEBUG" << " : " << std::setw(50) << caller << " : delete with size " << node->Size << " || " << file << ":" << line << std::endl;
			//			}
			//			else if (node->IsArray)
			//			{
			//				g_allocationFile << strDate << " : " << std::left << std::setw(7) << "DEBUG" << " : " << std::setw(50) << caller << " : delete[] with size " << node->Size << std::endl;
			//			}
			//			else
			//			{
			//				g_allocationFile << strDate << " : " << std::left << std::setw(7) << "DEBUG" << " : " << std::setw(50) << caller << " : delete with size " << node->Size << std::endl;
			//			}
			//
			//			::free(strDate);
			//		}
			//	}
			//}

			struct FileData
			{
				FILE* fd;
				char* Name;
			};

			FileData g_allocationFile { nullptr, LOG_FILE_FOR_COMMANDS };

			//std::ofstream g_allocationFile;

			void PrintMemoryAllocated(const char* caller, MemoryNode* node)
			{
				if (g_doLogMemory)
				{
					char* strDate = DebugMemoryManager::GetCurrentDate();

					if (nullptr == g_allocationFile.fd)
					{
						char* filePath = GetFileRelativeToApp(g_allocationFile.Name);

						g_allocationFile.fd = fopen(filePath, "a");

						if (nullptr == g_allocationFile.fd)
						{
							auto err = errno;

							switch (err)
							{
							case ENOENT:
								std::cout << "Impossible d'ouvrir le fichier : " << filePath << std::endl;
								break;
							case EINVAL:
								std::cout << "le mode d'ouverture du fichier est incorrect" << std::endl;
								break;
							default:
								std::cout << "erreur lors de l'ouverture du fichier : " << err << std::endl;
								break;
							}
						}

						::free(filePath);
					}

					if (NULL != node->File && node->IsArray)
					{
						fprintf(g_allocationFile.fd, "%s : %-10s : %-50.50s : new[] with size %u || %s:%u\n", strDate, "DEBUG", caller, node->Size, node->File, node->Line);
						//g_allocationFile << strDate << " : " << std::left << std::setw(7) << "DEBUG" << " : " << std::setw(50) << caller << " : new[] with size " << node->Size << " || " << node->File << ":" << node->Line << std::endl;
					}
					else if (NULL != node->File)
					{
						fprintf(g_allocationFile.fd, "%s : %-10s : %-50.50s : new with size %u || %s:%u\n", strDate, "DEBUG", caller, node->Size, node->File, node->Line);
						//g_allocationFile << strDate << " : " << std::left << std::setw(7) << "DEBUG" << " : " << std::setw(50) << caller << " : new with size " << node->Size << " || " << node->File << ":" << node->Line << std::endl;
					}
					else if (node->IsArray)
					{
						fprintf(g_allocationFile.fd, "%s : %-10s : %-50.50s : new[] with size %u\n", strDate, "DEBUG", caller, node->Size);
						//g_allocationFile << strDate << " : " << std::left << std::setw(7) << "DEBUG" << " : " << std::setw(50) << caller << " : new[] with size " << node->Size << std::endl;
					}
					else
					{
						fprintf(g_allocationFile.fd, "%s : %-10s : %-50.50s : new with size %u\n", strDate, "DEBUG", caller, node->Size);
						//g_allocationFile << strDate << " : " << std::left << std::setw(7) << "DEBUG" << " : " << std::setw(50) << caller << " : new with size " << node->Size << std::endl;
					}

					::free(strDate);
				}
			}

			void PrintMemoryDeallocated(const char* caller, MemoryNode* node, const char* file, unsigned int line)
			{
				if (g_doLogMemory)
				{
					if (nullptr == g_allocationFile.fd)
					{
						char* filePath = GetFileRelativeToApp(g_allocationFile.Name);

						g_allocationFile.fd = fopen(filePath, "a");
						::free(filePath);
					}

					char* strDate = DebugMemoryManager::GetCurrentDate();

					if (NULL != file && node->IsArray)
					{
						fprintf(g_allocationFile.fd, "%s : %-10s : %-50.50s : delete[] with size %u || %s:%u\n", strDate, "DEBUG", caller, node->Size, node->File, node->Line);
						//g_allocationFile << strDate << " : " << std::left << std::setw(7) << "DEBUG" << " : " << std::setw(50) << caller << " : delete[] with size " << node->Size << " || " << file << ":" << line << std::endl;
					}
					else if (NULL != file)
					{
						fprintf(g_allocationFile.fd, "%s : %-10s : %-50.50s : delete with size %u || %s:%u\n", strDate, "DEBUG", caller, node->Size, node->File, node->Line);
						//g_allocationFile << strDate << " : " << std::left << std::setw(7) << "DEBUG" << " : " << std::setw(50) << caller << " : delete with size " << node->Size << " || " << file << ":" << line << std::endl;
					}
					else if (node->IsArray)
					{
						fprintf(g_allocationFile.fd, "%s : %-10s : %-50.50s : delete[] with size %u\n", strDate, "DEBUG", caller, node->Size);
						//g_allocationFile << strDate << " : " << std::left << std::setw(7) << "DEBUG" << " : " << std::setw(50) << caller << " : delete[] with size " << node->Size << std::endl;
					}
					else
					{
						fprintf(g_allocationFile.fd, "%s : %-10s : %-50.50s : delete with size %u\n", strDate, "DEBUG", caller, node->Size);
						//g_allocationFile << strDate << " : " << std::left << std::setw(7) << "DEBUG" << " : " << std::setw(50) << caller << " : delete with size " << node->Size << std::endl;
					}

					::free(strDate);
				}
			}

		}
		DebugMemoryManager* DebugMemoryManager::sm_instance = NULL;

		DebugMemoryManager& DebugMemoryManager::GetInstance()
		{
			//static DebugMemoryManager instance;

			//return instance;
			if (NULL == sm_instance)
			{
				sm_instance = new (::malloc(sizeof(DebugMemoryManager))) DebugMemoryManager();
				atexit(DebugMemoryManager::FreeManager);
			}

			return *sm_instance;
		}

		char* DebugMemoryManager::GetCurrentDate()
		{
			char* str = static_cast<char*>(::malloc(20 * sizeof(char)));
			time_t t = time(NULL);
			tm* tt;

			str[0] = '\0';
			tt = localtime(&t);
			if (nullptr == tt)
			{
				throw std::string("impossible de récupérer l'heure locale");
			}

			strftime(str, 20, "%d/%m/%Y %H:%M:%S", tt);
			return str;
		}

		void DebugMemoryManager::FreeManager()
		{
			if (NULL != sm_instance)
			{
				sm_instance->~DebugMemoryManager();
				::free(sm_instance);
				sm_instance = NULL;
			}
		}

		DebugMemoryManager::DebugMemoryManager()
		{
			s_memory = static_cast<MemoryNode*>(::malloc(sizeof(MemoryNode)));

			s_memory->File = NULL;
			s_memory->IsArray = false;
			s_memory->Line = 0;
			s_memory->Magic = MAGIC_NUMBER;
			s_memory->Next = s_memory;
			s_memory->Previous = s_memory;
			s_memory->Size = 0;
			m_nextFreeFile = NULL;
			m_nextFreeLine = 0u;
			m_logFileNameForLeak = LOG_FILE_FOR_LEAK;
			m_logFileNameForCommands = LOG_FILE_FOR_COMMANDS;
			char* fileName = nullptr;
			FILE* fd = nullptr;

			fileName = GetFileRelativeToApp(m_logFileNameForLeak);

			fd = fopen(fileName, "w");
			if (nullptr != fd)
			{
				fputs("=========================================\n", fd);
				fputs("=               Memory Debug            =\n", fd);
				fputs("=========================================\n", fd);
				fclose(fd);
			}

			::free(fileName);
			fileName = nullptr;

			fileName = GetFileRelativeToApp(g_allocationFile.Name);
			g_allocationFile.fd = fopen(fileName, "w");
			if (nullptr != g_allocationFile.fd)
			{
				fputs("=========================================\n", fd);
				fputs("=            Memory Allocations         =\n", fd);
				fputs("=========================================\n", fd);
			}

			::free(fileName);
			fileName = nullptr;

			fileName = GetFileRelativeToApp(LOG_FILE_FOr_ERRORS);
			fd = fopen(fileName, "w");
			if (nullptr != fd)
			{
				fclose(fd);
			}

			::free(fileName);
			fileName = nullptr;

		}

		DebugMemoryManager::~DebugMemoryManager()
		{
			if (s_memory->Next != s_memory)
			{
				// memory leak detected
				MemoryNode* node = s_memory->Next;
				do
				{
					MemoryNode* next = node->Next;

					PrintMemoryNotFree("DebugMemoryManager::~DebugMemoryManager", m_logFileNameForLeak, node);
					::free(node);
					node = next;
				} while (node != s_memory);
			}

			::free(s_memory);
		}

		void* DebugMemoryManager::Allocate(std::size_t size, const char* file, unsigned int line, bool isArray)
		{
			if (size == 0)
			{
				std::ofstream errorFile;

				errorFile.open(LOG_FILE_FOr_ERRORS, std::ios::app);
				if (errorFile.is_open())
				{
					char* strTime = GetCurrentDate();

					if (isArray)
					{
						errorFile << strTime << " - can't allocate memory for array without size";
					}
					else
					{
						errorFile << strTime << " - can't allocate memory without size";
					}

					if (NULL != file)
					{
						errorFile << " || " << file << ":" << line;
					}

					errorFile << std::endl;
					errorFile.close();
					std::free(strTime);
				}
				return NULL;
			}

			MemoryNode* node = static_cast<MemoryNode*>(::malloc(size + sizeof(MemoryNode)));

			if (NULL == node)
			{
				std::ofstream errorFile;

				errorFile.open(LOG_FILE_FOr_ERRORS, std::ios::app);
				if (errorFile.is_open())
				{
					char* strTime = GetCurrentDate();

					if (isArray)
					{
						errorFile << strTime << " - memory allocation for an array failed with size " << size;
					}
					else
					{
						errorFile << strTime << " - memory allocation failed with size " << size;
					}

					if (NULL != file)
					{
						errorFile << " || " << file << ":" << line;
					}

					errorFile << std::endl;
					errorFile.close();
					std::free(strTime);
				}
				return NULL;
			}

			node->File = file;
			node->IsArray = isArray;
			node->Line = line;
			node->Magic = MAGIC_NUMBER;
			node->Next = s_memory;
			node->Previous = s_memory->Previous;
			node->Size = size;
			s_memory->Previous->Next = node;
			s_memory->Previous = node;
			PrintMemoryAllocated("DebugMemoryManager::Allocate", node);
			return reinterpret_cast<void*>(std::size_t(node) + sizeof(MemoryNode));
		}

		void DebugMemoryManager::Deallocate( void* ptr, bool isArray )
		{
			if (NULL == ptr)
			{
				std::ofstream errorFile;

				errorFile.open(LOG_FILE_FOr_ERRORS, std::ios::app);
				if (errorFile.is_open())
				{
					char* strTime = GetCurrentDate();

					if (isArray)
					{
						errorFile << strTime << " - delete[] called on a null pointer";
					}
					else
					{
						errorFile << strTime << " - delete called on a null pointer";
					}

					if (NULL != m_nextFreeFile)
					{
						errorFile << " || " << m_nextFreeFile << ":" << m_nextFreeLine;
					}

					errorFile << std::endl;
					std::free(strTime);
					errorFile.close();
				}

				m_nextFreeFile = NULL;
				m_nextFreeLine = 0;
				return;
			}

			MemoryNode* node = reinterpret_cast<MemoryNode*>(std::size_t(ptr) - sizeof(MemoryNode));

			if (node->Magic != MAGIC_NUMBER)
			{
				std::ofstream errorFile;

				errorFile.open(LOG_FILE_FOr_ERRORS, std::ios::app);
				if (errorFile.is_open())
				{
					char* strTime = GetCurrentDate();

					if (isArray)
					{
						errorFile << strTime << " - delete[] called on an invalid pointer";
					}
					else
					{
						errorFile << strTime << " - delete called on an invalid pointer";
					}

					if (NULL != m_nextFreeFile)
					{
						errorFile << " || " << m_nextFreeFile << ":" << m_nextFreeLine;
					}

					errorFile << std::endl;
					std::free(strTime);
					errorFile.close();
				}

				m_nextFreeFile = NULL;
				m_nextFreeLine = 0;
				return;
			}

			PrintMemoryDeallocated("DebugMemoryManager::Deallocate", node, m_nextFreeFile, m_nextFreeLine);
			m_nextFreeFile = NULL;
			m_nextFreeLine = 0u;
			MemoryNode* prev = node->Previous;
			MemoryNode* next = node->Next;

			prev->Next = next;
			next->Previous = prev;
			node->Previous = NULL;
			node->Next = NULL;
			::free(node);
		}

		void DebugMemoryManager::SetNextFree( const char* file, unsigned int line )
		{
			assert(NULL != file);
			m_nextFreeFile = file;
			m_nextFreeLine = line;
		}

	}
}


