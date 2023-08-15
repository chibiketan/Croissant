//
// Created by gcompte on 02/08/2023.
//
// This define for VOLK method build only in one cpp file

#define VOLK_IMPLEMENTATION

#include "Graphic/Vulkan/VulkanRenderer.hpp"

using namespace Croissant::Graphic::Vulkan;

VulkanRenderer::VulkanRenderer(Window& window) :
    m_instance{nullptr},
    m_surface{nullptr},
    m_currentDevice{nullptr}
    {
    if (volkGetLoadedInstance() == VK_NULL_HANDLE) {
        volkInitialize();
    }

    this->m_instance = Wrapper::Instance::Create("My super app", "Croissant Engine");
    this->m_surface = Wrapper::Surface::Create(*this->m_instance, window);
    LoadPhysicalDevices();
    m_currentDevice = &PickPhysicalDevice();
    TRACE("Picked device : "<<m_currentDevice->m_properties.deviceName);
}

void VulkanRenderer::LoadPhysicalDevices() {
    std::vector <VkPhysicalDevice> devices;

    if (!m_instance->GetPhysicalDevices(devices)) {
        throw std::runtime_error("Impossible de récupérer les cartes physiques");
    }

    for (VkPhysicalDevice device: devices) {
        m_physicalDevices.emplace_back(*m_instance, *m_surface, device);
    }
}

Wrapper::PhysicalDevice& VulkanRenderer::PickPhysicalDevice() {
    for (auto& device : m_physicalDevices) {
        if (IsDeviceSuitable(device)) {
            return device;
        }
    }

    throw std::runtime_error("No suitable device found.");
}

bool VulkanRenderer::IsDeviceSuitable(Wrapper::PhysicalDevice &device) {
    return device.m_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
           && device.m_features.geometryShader
           && device.m_presentQueueIndex.has_value()
           && device.m_graphicalQueueIndex.has_value()
           && device.m_supportSwap;

}
