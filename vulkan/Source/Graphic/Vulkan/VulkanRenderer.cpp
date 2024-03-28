//
// Created by gcompte on 02/08/2023.
//
// This define for VOLK method build only in one cpp file

#define VOLK_IMPLEMENTATION

#include "Graphic/Vulkan/VulkanRenderer.hpp"
#include "Graphic/Vulkan/Wrapper/ShaderModule.hpp"

// TODO on fait quoi de ça ?
std::vector<char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error(std::string {"échec de l'ouverture du fichier "} + filename + "!");
    }

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}

using namespace Croissant::Graphic::Vulkan;



VulkanRenderer::VulkanRenderer(Window& window) :
    m_instance{nullptr},
    m_surface{nullptr},
    m_currentDevice{nullptr},
    m_logicalDevice{nullptr},
    m_renderPass{nullptr}
    {
    if (volkGetLoadedInstance() == VK_NULL_HANDLE) {
        volkInitialize();
    }

    window.Open();

    this->m_instance = Wrapper::Instance::Create("My super app", "Croissant Engine");
    this->m_surface = Wrapper::Surface::Create(*this->m_instance, window);
    LoadPhysicalDevices();
    m_currentDevice = &PickPhysicalDevice();
    TRACE("Picked device : "<<m_currentDevice->m_properties.deviceName);
    m_logicalDevice = std::make_unique<Wrapper::LogicalDevice>(*m_instance, *m_currentDevice);
    m_swapChain = std::make_unique<Wrapper::SwapChain>(*m_instance, *m_currentDevice, *m_logicalDevice, window, *m_surface);
    this->m_renderPass = std::make_unique<Wrapper::RenderPass>(*this->m_instance, *this->m_logicalDevice, *this->m_swapChain);

    // create shaders
    {
        auto vertShaderCode = readFile("vulkan/shaders/shader.vert.spv");
        auto fragShaderCode = readFile("vulkan/shaders/shader.frag.spv");

        Wrapper::ShaderModule vertShaderModule { *m_instance, *m_logicalDevice, vertShaderCode };
        Wrapper::ShaderModule fragShaderModule { *m_instance, *m_logicalDevice, fragShaderCode };

        //auto m_graphicsPipeline = std::make_unique<Wrapper::GraphicsPipeline>(*m_instance, *m_logicalDevice, *m_renderPass, vertShaderModule, fragShaderModule);
        auto m_graphicsPipeline = Wrapper::GraphicsPipeline {*m_instance, *m_logicalDevice, *m_renderPass, *m_swapChain, vertShaderModule, fragShaderModule};
    }
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
