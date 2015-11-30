#ifndef MEMORYMANAGER_HPP_INC
#define MEMORYMANAGER_HPP_INC

#include "Engine.hpp"

#ifdef CROISSANT_LINUX
// compilation linux
#elif defined(CROISSANT_WINDOWS) && defined(COMPILER_MSVC)
// compilation windows
// uniquement pour le compilateur microsoft
#  include <crtdbg.h>
#endif

#include <cstdlib>

#define MEMORY_MANAGER Croissant::Debug::MemoryManager::GetInstance()

namespace Croissant
{
	namespace Debug
	{
		class ENGINE_API MemoryManager
		{
		public:
			static MemoryManager& GetInstance();
			void* Allocate(std::size_t size, const char* file, unsigned int line, bool isArray);
			void Deallocate(void* ptr, bool isArray);
			~MemoryManager();
			void SetNextFree(const char* file, unsigned int line);
			// Get newly allocated string with current time (need to free the char*)
			static char* GetCurrentDate();
		private:
			MemoryManager();
			const char* m_logFileNameForLeak;
			const char* m_logFileNameForCommands;
			const char* m_nextFreeFile;
			unsigned int m_nextFreeLine;

			static MemoryManager* sm_instance;
			static void FreeManager();
		};
	}
}

#ifdef CROISSANT_MEMORY_DEBUG

//inline void* _cdecl operator new(std::size_t size)
void* operator new(std::size_t size);

inline void* operator new(std::size_t size, const char* file, unsigned int line)
{
	return MEMORY_MANAGER.Allocate(size, file, line, false);
}

void* operator new[](std::size_t size);

inline void* operator new[](std::size_t size, const char* file, unsigned int line)
{
	return MEMORY_MANAGER.Allocate(size, file, line, true);
}

void operator delete(void* ptr) throw();

void operator delete[](void* ptr) throw();

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
