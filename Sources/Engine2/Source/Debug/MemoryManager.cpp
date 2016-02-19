#include "Debug/MemoryManager.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <assert.h>

#if defined(CROISSANT_LINUX)
#  include <unistd.h>
#  include <string.h>
#  include <libgen.h>
#  include <sys/types.h>
#  include <sys/stat.h>

#  define MAX_PATH 255

#elif defined(CROISSANT_WINDOWS)
#  include <windows.h>

#endif

#include <errno.h>

#define MAGIC_NUMBER 0xC2FA4576U
#define LOG_FILE_FOR_LEAK "MemoryLeak.log"
#define LOG_FILE_FOR_COMMANDS "MemoryAllocation.log"
#define LOG_FILE_FOr_ERRORS "MemoryErrors.log"

namespace Croissant
{
	namespace Debug
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

#if defined(CROISSANT_LINUX)

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

                char* result = static_cast<char*>(std::malloc(MAX_PATH * sizeof(char)));

				::memset(result, 0, MAX_PATH * sizeof(char));
                ::strcpy(result, g_applicationPath);
				::strcat(result, "/Logs/");
				mkdir(result, 0777);
                ::strcat(result, fileName);
				return result;
			}

			void OpenFile(FILE* (&file), const char* path, const char* mode)
			{
				file = fopen(path, mode);
			}

#elif defined(CROISSANT_WINDOWS)



			template<typename C, size_t size, size_t size2>
			void CopyString(C (&output)[size], const C (&source)[size2])
			{
#  if defined(COMPILER_MSVC)
				::strcpy_s(output, source);
#  else
                ::strcpy(output, source);
#  endif
			}

			void CopyString(char* output, size_t size, const char* source)
			{
#  if defined(COMPILER_MSVC)
				::strcpy_s(output, size, source);
#  else
                (void)size;
                ::strcpy(output, source);
#  endif
			}

			template<typename C, size_t size>
			void CatString(C(&output)[size], const C* source)
			{
#  if defined(COMPILER_MSVC)
				::strcat_s(output, source);
#  else
                ::strcat(output, source);
#  endif
			}

			void CatString(char* output, size_t size, const char* source)
			{
#  if defined(COMPILER_MSVC)
				::strcat_s(output, size, source);
#  else
                (void)size;
                ::strcat(output, source);
#  endif
			}

			template<typename C, size_t size, size_t sizeDrive, size_t sizeDir>
			void SplitPath(const C(&output)[size], C (&drivePath)[sizeDrive], C (&dir)[sizeDir])
			{
#  if defined(COMPILER_MSVC)
				::_splitpath_s(output, drivePath, sizeDrive, dir, sizeDir, nullptr, 0, nullptr, 0);
#  else
                ::_splitpath(output, drivePath, dir, nullptr, nullptr);
#  endif
			}

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
				SplitPath(fullPath, drivePath, directoryPath);
				//::_splitpath(fullPath, drivePath, directoryPath, nullptr, nullptr);
				::ZeroMemory(g_applicationPath, MAX_PATH);
				CopyString(g_applicationPath, drivePath);
				CatString(g_applicationPath, directoryPath);
				CatString(g_applicationPath, "Logs\\");
				//::strcpy(g_applicationPath, drivePath);
				//::strcat(g_applicationPath, directoryPath);
				//::strcat(g_applicationPath, "Logs\\");
				CreateDirectory(g_applicationPath, nullptr);
				}

				auto size = MAX_PATH * sizeof(char);
				char* result = static_cast<char*>(std::malloc(size));

				CopyString(result, size, g_applicationPath);
				CatString(result, size, fileName);
				//::strcpy(result, g_applicationPath);
				//::strcat(result, fileName);
				return result;
			}

			void OpenFile(FILE* (&file), const char* path, const char* mode)
			{
#  if defined(COMPILER_MSVC)
				fopen_s(&file, path, mode);
#  else
                file = ::fopen(path, mode);
#  endif
			}


#endif

			void PrintMemoryNotFree(const char* caller, const char* fileName, MemoryNode* node)
			{
				std::ofstream file;
				char* strTime = MemoryManager::GetCurrentDate();
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
				std::free(filePath);
				std::free(strTime);
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
			//			std::free(filePath);
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
			//		std::free(strDate);
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
			//			std::free(filePath);
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
			//			std::free(strDate);
			//		}
			//	}
			//}

			struct FileData
			{
				FILE* fd;
				const char* Name;
			};

			FileData g_allocationFile { nullptr, LOG_FILE_FOR_COMMANDS };

			//std::ofstream g_allocationFile;

			void PrintMemoryAllocated(const char* caller, MemoryNode* node)
			{
				if (g_doLogMemory)
				{
					char* strDate = MemoryManager::GetCurrentDate();

					if (nullptr == g_allocationFile.fd)
					{
						char* filePath = GetFileRelativeToApp(g_allocationFile.Name);

						OpenFile(g_allocationFile.fd, filePath, "a");

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

						std::free(filePath);
					}

					if (NULL != node->File && node->IsArray)
					{
						fprintf(g_allocationFile.fd, "%s : %-10s : %-50.50s : new[] with size %llu || %s:%u\n", strDate, "DEBUG", caller, node->Size, node->File, node->Line);
						//g_allocationFile << strDate << " : " << std::left << std::setw(7) << "DEBUG" << " : " << std::setw(50) << caller << " : new[] with size " << node->Size << " || " << node->File << ":" << node->Line << std::endl;
					}
					else if (NULL != node->File)
					{
						fprintf(g_allocationFile.fd, "%s : %-10s : %-50.50s : new with size %llu || %s:%u\n", strDate, "DEBUG", caller, node->Size, node->File, node->Line);
						//g_allocationFile << strDate << " : " << std::left << std::setw(7) << "DEBUG" << " : " << std::setw(50) << caller << " : new with size " << node->Size << " || " << node->File << ":" << node->Line << std::endl;
					}
					else if (node->IsArray)
					{
						fprintf(g_allocationFile.fd, "%s : %-10s : %-50.50s : new[] with size %llu\n", strDate, "DEBUG", caller, node->Size);
						//g_allocationFile << strDate << " : " << std::left << std::setw(7) << "DEBUG" << " : " << std::setw(50) << caller << " : new[] with size " << node->Size << std::endl;
					}
					else
					{
						fprintf(g_allocationFile.fd, "%s : %-10s : %-50.50s : new with size %llu\n", strDate, "DEBUG", caller, node->Size);
						//g_allocationFile << strDate << " : " << std::left << std::setw(7) << "DEBUG" << " : " << std::setw(50) << caller << " : new with size " << node->Size << std::endl;
					}

					std::free(strDate);
				}
			}

			void PrintMemoryDeallocated(const char* caller, MemoryNode* node, const char* file, unsigned int line)
			{
				if (g_doLogMemory)
				{
					if (nullptr == g_allocationFile.fd)
					{
						char* filePath = GetFileRelativeToApp(g_allocationFile.Name);

						OpenFile(g_allocationFile.fd, filePath, "a");
						std::free(filePath);
					}

					char* strDate = MemoryManager::GetCurrentDate();

					if (NULL != file && node->IsArray)
					{
						fprintf(g_allocationFile.fd, "%s : %-10s : %-50.50s : delete[] with size %llu || %s:%u\n", strDate, "DEBUG", caller, node->Size, file, line);
						//g_allocationFile << strDate << " : " << std::left << std::setw(7) << "DEBUG" << " : " << std::setw(50) << caller << " : delete[] with size " << node->Size << " || " << file << ":" << line << std::endl;
					}
					else if (NULL != file)
					{
						fprintf(g_allocationFile.fd, "%s : %-10s : %-50.50s : delete with size %llu || %s:%u\n", strDate, "DEBUG", caller, node->Size, file, line);
						//g_allocationFile << strDate << " : " << std::left << std::setw(7) << "DEBUG" << " : " << std::setw(50) << caller << " : delete with size " << node->Size << " || " << file << ":" << line << std::endl;
					}
					else if (node->IsArray)
					{
						fprintf(g_allocationFile.fd, "%s : %-10s : %-50.50s : delete[] with size %llu\n", strDate, "DEBUG", caller, node->Size);
						//g_allocationFile << strDate << " : " << std::left << std::setw(7) << "DEBUG" << " : " << std::setw(50) << caller << " : delete[] with size " << node->Size << std::endl;
					}
					else
					{
						fprintf(g_allocationFile.fd, "%s : %-10s : %-50.50s : delete with size %llu\n", strDate, "DEBUG", caller, node->Size);
						//g_allocationFile << strDate << " : " << std::left << std::setw(7) << "DEBUG" << " : " << std::setw(50) << caller << " : delete with size " << node->Size << std::endl;
					}

					std::free(strDate);
				}
			}

		}
		MemoryManager* MemoryManager::sm_instance = NULL;

		MemoryManager& MemoryManager::GetInstance()
		{
			//static DebugMemoryManager instance;

			//return instance;
			if (NULL == sm_instance)
			{
				sm_instance = new (std::malloc(sizeof(MemoryManager))) MemoryManager();
				atexit(MemoryManager::FreeManager);
			}

			return *sm_instance;
		}

		char* MemoryManager::GetCurrentDate()
		{
			char* str = static_cast<char*>(std::malloc(20 * sizeof(char)));
			time_t t = time(NULL);
			tm* tt;

			str[0] = '\0';
#ifdef COMPILER_MSVC
			tm tmpTt;

			localtime_s(&tmpTt, &t);
			tt = &tmpTt;
#else
			tt = localtime(&t);
#endif
			if (nullptr == tt)
			{
				throw std::string("impossible de récupérer l'heure locale");
			}

			strftime(str, 20, "%d/%m/%Y %H:%M:%S", tt);
			return str;
		}

		void MemoryManager::FreeManager()
		{
			if (NULL != sm_instance)
			{
				sm_instance->~MemoryManager();
				std::free(sm_instance);
				sm_instance = NULL;
			}
		}

		MemoryManager::MemoryManager()
		{
			s_memory = static_cast<MemoryNode*>(std::malloc(sizeof(MemoryNode)));

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

			OpenFile(fd, fileName, "w");
			if (nullptr != fd)
			{
				fputs("=========================================\n", fd);
				fputs("=               Memory Debug            =\n", fd);
				fputs("=========================================\n", fd);
				fclose(fd);
			}

			std::free(fileName);
			fileName = nullptr;

			fileName = GetFileRelativeToApp(g_allocationFile.Name);
			OpenFile(g_allocationFile.fd, fileName, "w");
			if (nullptr != g_allocationFile.fd)
			{
				fputs("=========================================\n", fd);
				fputs("=            Memory Allocations         =\n", fd);
				fputs("=========================================\n", fd);
			}

			std::free(fileName);
			fileName = nullptr;

			fileName = GetFileRelativeToApp(LOG_FILE_FOr_ERRORS);
			OpenFile(fd, fileName, "w");
			if (nullptr != fd)
			{
				fclose(fd);
			}

			std::free(fileName);
			fileName = nullptr;

		}

		MemoryManager::~MemoryManager()
		{
			if (s_memory->Next != s_memory)
			{
				// memory leak detected
				MemoryNode* node = s_memory->Next;
				do
				{
					MemoryNode* next = node->Next;

					PrintMemoryNotFree("DebugMemoryManager::~DebugMemoryManager", m_logFileNameForLeak, node);
					std::free(node);
					node = next;
				} while (node != s_memory);
			}

			std::free(s_memory);
		}

		void* MemoryManager::Allocate(std::size_t size, const char* file, unsigned int line, bool isArray)
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

			MemoryNode* node = static_cast<MemoryNode*>(std::malloc(size + sizeof(MemoryNode)));

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

		void MemoryManager::Deallocate( void* ptr, bool isArray )
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
			std::free(node);
		}

		void MemoryManager::SetNextFree( const char* file, unsigned int line )
		{
			assert(NULL != file);
			m_nextFreeFile = file;
			m_nextFreeLine = line;
		}

	}
}

#ifdef CROISSANT_MEMORY_DEBUG

		void* operator new(std::size_t size)
		{
			return MEMORY_MANAGER.Allocate(size, NULL, 0, false);
		}

		void* operator new[](std::size_t size)
		{
			return MEMORY_MANAGER.Allocate(size, NULL, 0, true);
		}


		void operator delete(void* ptr) throw()
		{
			MEMORY_MANAGER.Deallocate(ptr, false);
		}

		void operator delete[](void* ptr) throw()
		{
			MEMORY_MANAGER.Deallocate(ptr, true);
		}
#endif

