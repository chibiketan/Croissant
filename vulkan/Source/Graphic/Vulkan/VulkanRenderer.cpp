//
// Created by gcompte on 02/08/2023.
//
// This define for VOLK method build only in one cpp file

#define VOLK_IMPLEMENTATION

#include "Graphic/Vulkan/VulkanRenderer.hpp"

using namespace Croissant::Graphic::Vulkan;

VulkanRenderer::VulkanRenderer(Window& window) :
    m_instance{nullptr},
    m_surface{nullptr}
    {
    if (volkGetLoadedInstance() == VK_NULL_HANDLE) {
        volkInitialize();
    }

    this->m_instance = Wrapper::Instance::Create("My super app", "Croissant Engine");
    this->m_surface = Wrapper::Surface::Create(*this->m_instance, window);
    LoadPhysicalDevices();
}

void VulkanRenderer::LoadPhysicalDevices() {
    std::vector<VkPhysicalDevice> devices;

    if (!m_instance->GetPhysicalDevices(devices)) {
        throw std::runtime_error("Impossible de récupérer les cartes physiques");
    }

    m_physicalDevices.reserve(devices.size());
    for (VkPhysicalDevice device : devices)
    {
        Wrapper::PhysicalDevice deviceInfo;
        vkGetPhysicalDeviceQueueFamilyProperties
        if (!s_instance.GetPhysicalDeviceQueueFamilyProperties(physDevice, &deviceInfo.queueFamilies))
        {
            NazaraWarning("Failed to query physical device queue family properties for " + std::string(deviceInfo.properties.deviceName) + " (0x" + NumberToString(deviceInfo.properties.deviceID, 16) + ')');
            continue;
        }

        deviceInfo.physDevice = physDevice;

        deviceInfo.features         = s_instance.GetPhysicalDeviceFeatures(physDevice);
        deviceInfo.memoryProperties = s_instance.GetPhysicalDeviceMemoryProperties(physDevice);
        deviceInfo.properties       = s_instance.GetPhysicalDeviceProperties(physDevice);

        std::vector<VkExtensionProperties> extensions;
        if (s_instance.GetPhysicalDeviceExtensions(physDevice, &extensions))
        {
            for (auto& extProperty : extensions)
                deviceInfo.extensions.emplace(extProperty.extensionName);
        }
        else
            NazaraWarning("Failed to query physical device extensions for " + std::string(deviceInfo.properties.deviceName) + " (0x" + NumberToString(deviceInfo.properties.deviceID, 16) + ')');

        s_physDevices.emplace_back(std::move(deviceInfo));
}