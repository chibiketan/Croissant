#ifndef MEMORYMANAGER_H_
#define MEMORYMANAGER_H_
#pragma once

#ifdef __linux
// compilation linux
#elif _WIN64 || _WIN32
// compilation windows
#include <crtdbg.h>
#endif

#include <cstdlib>
#include "Engine.hpp"

#define MEMORY_MANAGER Croissant::Core::DebugMemoryManager::GetInstance()

namespace Croissant
{
	namespace Core
	{
		class ENGINE_API DebugMemoryManager
		{
		public:
			static DebugMemoryManager& GetInstance();
			void* Allocate(std::size_t size, const char* file, unsigned int line, bool isArray);
			void Deallocate(void* ptr, bool isArray);
			~DebugMemoryManager();
			void SetNextFree(const char* file, unsigned int line);
			// Get newly allocated string with current time (need to free the char*)
			static char* GetCurrentDate();
		private:
			DebugMemoryManager();
			const char* m_logFileNameForLeak;
			const char* m_logFileNameForCommands;
			const char* m_nextFreeFile;
			unsigned int m_nextFreeLine;

			static DebugMemoryManager* sm_instance;
			static void FreeManager();
		};
	}
}

#ifdef CROISSANT_MEMORY_DEBUG

//inline void* _cdecl operator new(std::size_t size)
inline void* operator new(std::size_t size)
{
	return MEMORY_MANAGER.Allocate(size, NULL, 0, false);
}

inline void* operator new(std::size_t size, const char* file, unsigned int line)
{
	return MEMORY_MANAGER.Allocate(size, file, line, false);
}

inline void* operator new[](std::size_t size)
{
	return MEMORY_MANAGER.Allocate(size, NULL, 0, true);
}

inline void* operator new[](std::size_t size, const char* file, unsigned int line)
{
	return MEMORY_MANAGER.Allocate(size, file, line, true);
}

inline void operator delete(void* ptr) throw()
{
	MEMORY_MANAGER.Deallocate(ptr, false);
}

inline void operator delete[](void* ptr) throw()
{
	MEMORY_MANAGER.Deallocate(ptr, true);
}

inline void operator delete(void* ptr, const char* file, unsigned int line)
{
	MEMORY_MANAGER.SetNextFree(file, line);
	MEMORY_MANAGER.Deallocate(ptr, false);
	//delete ptr;
}

inline void operator delete[](void* ptr, const char* file, unsigned int line)
{
	MEMORY_MANAGER.SetNextFree(file, line);
	MEMORY_MANAGER.Deallocate(ptr, true);
	//delete[] ptr;
}

#define CROISSANT_NEW new(__FILE__, __LINE__)
#define CROISSANT_DELETE(obj) MEMORY_MANAGER.SetNextFree(__FILE__, __LINE__); delete obj;
#define CROISSANT_DELETE_SAFE(obj) MEMORY_MANAGER.SetNextFree(__FILE__, __LINE__); delete obj; obj = nullptr;
#define CROISSANT_DELETE_ARRAY(obj) MEMORY_MANAGER.SetNextFree(__FILE__, __LINE__); delete[] obj;
#define CROISSANT_DELETE_ARRAY_SAFE(obj) MEMORY_MANAGER.SetNextFree(__FILE__, __LINE__); delete[] obj; obj = nullptr;

#else
#define CROISSANT_NEW new
#define CROISSANT_DELETE(obj) delete obj;
#define CROISSANT_DELETE_SAFE(obj) delete obj; obj = nullptr;
#define CROISSANT_DELETE_ARRAY(obj) delete[] obj;
#define CROISSANT_DELETE_ARRAY_SAFE(obj) delete[] obj; obj = nullptr;
#endif

#endif
