//
// Created by Grego on 15/08/2023.
//
#include "Graphic/Vulkan/Wrapper/LogicalDevice.hpp"

namespace Croissant::Graphic::Vulkan::Wrapper {

    LogicalDevice::LogicalDevice( Instance &instance, PhysicalDevice &device) :
        m_instance{instance},
        m_device{device}
    {
        // TODO continue
//        auto deviceIndices = findQueueFamilies(this->m_physicalDevice);
//        float queuePriority = 1.0f;
//
//        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};
//        std::set<uint32_t> uniqueQueueFamilies = { deviceIndices.presentFamily.value(), deviceIndices.graphicsFamily.value() };
//
//        for (auto queueFamily : uniqueQueueFamilies) {
//            VkDeviceQueueCreateInfo queueCreateInfo{};
//
//            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
//            queueCreateInfo.queueFamilyIndex = queueFamily;
//            queueCreateInfo.queueCount = 1;
//            queueCreateInfo.pQueuePriorities = &queuePriority;
//            queueCreateInfos.push_back(queueCreateInfo);
//        }
//
//        VkPhysicalDeviceFeatures physicalDeviceFeatures{};
//        VkDeviceCreateInfo  deviceCreateInfo{};
//
//        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
//        deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
//        deviceCreateInfo.queueCreateInfoCount = queueCreateInfos.size();
//        deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;
//
//        std::vector<char const*> neededLayers{};
//        if (enableValidationLayer) {
//            neededLayers.emplace_back("VK_LAYER_KHRONOS_validation");
//        }
//
//        deviceCreateInfo.ppEnabledLayerNames = neededLayers.data();
//        deviceCreateInfo.enabledLayerCount = neededLayers.size();
//        // request extensions
//        auto const requiredExtensions = getDeviceExtensions();
//
//        deviceCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();
//        deviceCreateInfo.enabledExtensionCount = requiredExtensions.size();
//
//        if (VK_SUCCESS != vkCreateDevice(this->m_physicalDevice, &deviceCreateInfo, nullptr, this->m_device.get())) {
//            throw std::runtime_error("Unable to create logical device");
//        }
//
//        // get queues references
//        vkGetDeviceQueue(*this->m_device, deviceIndices.graphicsFamily.value(), 0, &this->m_graphicsQueue);
//        vkGetDeviceQueue(*this->m_device, deviceIndices.presentFamily.value(), 0, &this->m_presentQueue);
    }



}
