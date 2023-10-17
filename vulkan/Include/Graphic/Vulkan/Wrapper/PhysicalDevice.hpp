//
// Created by gcompte on 03/08/2023.
//
#pragma once

#ifndef ENGINE_PHYSICALDEVICE_HPP_INC
#  define ENGINE_PHYSICALDEVICE_HPP_INC

#include "Engine.hpp"
#include "Graphic/Vulkan/Utils.hpp"
#include "Graphic/Vulkan/Wrapper/Surface.hpp"
#include <optional>

namespace Croissant::Graphic::Vulkan::Wrapper {
    class Instance;
    class PhysicalDevice {
    CLASS_NO_COPY_MOVE(PhysicalDevice)
    public:
        inline explicit PhysicalDevice(Instance& instance, Wrapper::Surface& surface, VkPhysicalDevice m_device);
        inline operator VkPhysicalDevice();
        VkPhysicalDeviceFeatures m_features;
        VkPhysicalDeviceMemoryProperties m_memoryProperties;
        VkPhysicalDeviceProperties m_properties;
        std::vector<VkQueueFamilyProperties> m_queueFamillies;
        std::vector<VkExtensionProperties> m_extensions;
        std::optional<std::uint32_t> m_graphicalQueueIndex;
        std::optional<std::uint32_t> m_presentQueueIndex;
        bool m_supportSwap;
        std::optional<VkSurfaceCapabilitiesKHR>    m_surfaceCapabilities;
        std::vector<VkSurfaceFormatKHR> m_surfaceFormats;
        std::vector<VkPresentModeKHR> m_presentModes;

    private:
        inline bool CheckSwapSupport();

        Instance& m_instance;
        Wrapper::Surface& m_surface;
        VkPhysicalDevice m_device;
    };
}

#include "PhysicalDevice.inl"

#endif //ENGINE_PHYSICALDEVICE_HPP_INC
