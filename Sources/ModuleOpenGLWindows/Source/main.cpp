#include "ModuleOpenGLWindows.hpp"
#include "Core/DebugMemoryManager.hpp"
#include "Module/IModule.hpp"
#include "OpenGL/Window.hpp"
#include "OpenGL/WindowsInputModule.hpp"
#include "OpenGL/WindowsOpenGLModule.hpp"
#include <vector>
#include <memory>

extern "C"
{
	MODULEOPENGLWINDOWS_API Croissant::Module::ModuleList*  CROISSANT_MODULE_ENTRY()
	{
		Croissant::Module::ModuleList* result = CROISSANT_NEW Croissant::Module::ModuleList();
		std::shared_ptr<Croissant::Module::OpenGL::Window> window = std::make_shared<Croissant::Module::OpenGL::Window>();

		result->push_back(std::unique_ptr<Croissant::Module::IModule>(CROISSANT_NEW Croissant::Module::OpenGL::WindowsInputModule(window)));
		result->push_back(std::unique_ptr<Croissant::Module::IModule>(CROISSANT_NEW Croissant::Module::OpenGL::WindowsOpenGLModule(window)));
		return result;
	}

}
