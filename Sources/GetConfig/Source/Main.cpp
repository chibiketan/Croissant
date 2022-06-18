#include "Engine.hpp"
#include "Core/LogManager.hpp"
#include "Graphic/Window.hpp"
#include "Graphic/OpenGLRenderer.hpp"

#include <iostream>

#include "vulkan/vulkan.h"

int main(int argc, char** argv)
{
	UNUSED(argc);
	UNUSED(argv);
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    std::vector<VkLayerProperties> layerProperties(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, layerProperties.data());

	uint32_t extensionCount;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    VkExtensionProperties extensionProperties[extensionCount];
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensionProperties);

    std::cout << "============================================================" << std::endl;
    std::cout << "=                                                          =" << std::endl;
    std::cout << "=                   Configuration Vulkan                   =" << std::endl;
    std::cout << "=                                                          =" << std::endl;
    std::cout << "============================================================" << std::endl;
    std::cout << "Layer count : " << std::to_string(layerCount) << std::endl;
    for (const auto layerProperty : layerProperties)
    {
        std::cout << "-  " << layerProperty.layerName << " [" << layerProperty.description << "]" << std::endl;
    }

	std::cout << "Extension count : " << std::to_string(extensionCount) << std::endl;
	for (uint32_t i = 0; i < extensionCount; ++i)
    {
	    std::cout << "-  (" << std::to_string(extensionProperties[i].specVersion) << ") " << extensionProperties[i].extensionName << std::endl;
    }


	return 0;
	Croissant::Core::LogManager::Init();
	{
		Croissant::Graphic::Window win(10U, 10U, "DUMMY");
		Croissant::Graphic::OpenGLRenderer renderer(win);
		auto& info = renderer.GetSystemInfo();

		std::cout << "============================================================" << std::endl;
		std::cout << "=                                                          =" << std::endl;
		std::cout << "=                   Configuration OpenGL                   =" << std::endl;
		std::cout << "=                                                          =" << std::endl;
		std::cout << "============================================================" << std::endl;
		std::cout << "Version : " << std::to_string(info.GetMajorVersion()) << "." << std::to_string(info.GetMinorVersion()) << std::endl;
		std::cout << "Distance de clipping maximale : " << std::to_string(info.GetMaxClipDistance()) << std::endl;
		std::cout << "Taille maximale pour une texture 3D : " << std::to_string(info.GetMax3DTextureSize()) << std::endl;
		std::cout << "Nombre de buffer de dessin maximal : " << std::to_string(info.GetMaxDrawBuffer()) << std::endl;
		std::cout << "Taille maximale pour un tableau d'indices : " << std::to_string(info.GetMaxIndicesArraySize()) << std::endl;
		std::cout << "Taille maximale pour un tableau de vertex : " << std::to_string(info.GetMaxVertexArraySize()) << std::endl;
		std::cout << "Taille maximale pour un buffer de rendu : " << std::to_string(info.GetMaxRenderBufferSize()) << std::endl;
		std::cout << "Nombre d'extension OpenGL support�es : " << std::to_string(info.GetNumExtension()) << std::endl;
	}
	// logManager doit �tre arr�t� une fois que tous ses utilisateurs sont d�truits
	Croissant::Core::LogManager::Shutdown();

	std::cout << std::endl << std::endl << "Appuyer sur une touche pour continuer" << std::endl;
	std::getchar();
	return 0;
}
