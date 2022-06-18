#include "Graphic/VulkanRenderer.hpp"


Croissant::Graphic::VulkanRenderer::VulkanRenderer() {
    CreateInstance();
}

Croissant::Graphic::VulkanRenderer::~VulkanRenderer() {
    if (nullptr == m_instance)
    {
        vkDestroyInstance(m_instance, nullptr);
        m_instance = nullptr;
    }
}

void Croissant::Graphic::VulkanRenderer::CreateInstance() {
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Croissant";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_2;

    VkInstanceCreateInfo instanceCreateInfo{};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo = &appInfo;

    if (vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance) != VK_SUCCESS)
    {
        throw VulkanRendererException { "An error occured while creating Vulkan instance" };
    }

    Core::WriteTraceInfo("Vulkan instance created");


}

