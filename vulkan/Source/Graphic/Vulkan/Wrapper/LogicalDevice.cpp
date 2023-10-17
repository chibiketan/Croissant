//
// Created by Grego on 15/08/2023.
//
#include "Graphic/Vulkan/Wrapper/LogicalDevice.hpp"

namespace Croissant::Graphic::Vulkan::Wrapper {

    LogicalDevice::LogicalDevice( Instance &instance, PhysicalDevice &device) :
        m_instance{instance},
        m_device{device},
        m_logicalDevice{nullptr},
        m_graphicQueue{nullptr},
        m_presentQueue{nullptr}
    {
        float queuePriority = 1.0f;
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};
        std::set<uint32_t> uniqueQueueFamilies = { m_device.m_presentQueueIndex.value(), m_device.m_graphicalQueueIndex.value() };

        for (auto queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{
                    VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                    nullptr,
                    0,
                    queueFamily,
                    1,
                    &queuePriority
            };
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures physicalDeviceFeatures{};
        VkDeviceCreateInfo  deviceCreateInfo{};

        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
        deviceCreateInfo.queueCreateInfoCount = queueCreateInfos.size();
        deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures; // enable all features from device ?

        std::vector<char const*> neededLayers{};
        // TODO ajout d'un paramètre pour gérer ou non le layer de debug
        //if (enableValidationLayer) {
            neededLayers.emplace_back("VK_LAYER_KHRONOS_validation");
        //}

        deviceCreateInfo.ppEnabledLayerNames = neededLayers.data();
        deviceCreateInfo.enabledLayerCount = neededLayers.size();

        // request extensions
        //auto const requiredExtensions = getDeviceExtensions();
        std::vector<char const*> requiredExtensions{
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        deviceCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();
        deviceCreateInfo.enabledExtensionCount = requiredExtensions.size();

        auto result = vkCreateDevice(m_device, &deviceCreateInfo, nullptr, &m_logicalDevice);

        if (VK_SUCCESS != result) {
            throw std::runtime_error("Unable to create logical device");
        }

        // get queues references
        vkGetDeviceQueue(m_logicalDevice, m_device.m_graphicalQueueIndex.value(), 0, &m_graphicQueue);
        vkGetDeviceQueue(m_logicalDevice, m_device.m_presentQueueIndex.value(), 0, &m_presentQueue);
    }

    LogicalDevice::~LogicalDevice() {
        if (nullptr != m_logicalDevice) {
            vkDestroyDevice(m_logicalDevice, nullptr);
            m_logicalDevice = nullptr;
        }
    }


}
