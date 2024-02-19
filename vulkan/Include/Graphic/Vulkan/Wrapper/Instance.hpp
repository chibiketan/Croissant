//
// Created by gcompte on 02/08/2023.
//
#pragma once

#ifndef ENGINE_INSTANCE_HPP_INC
#  define ENGINE_INSTANCE_HPP_INC

#  include "Graphic/Vulkan/Utils.hpp"
#  include "Engine.hpp"
#  include <vector>
#  include <optional>

namespace Croissant::Graphic::Vulkan::Wrapper {
    /**
     * This class represents a Vulkan instance
     */
    class Instance {
    CLASS_NO_COPY_MOVE(Instance)
    public:
        static inline std::unique_ptr<Instance> Create(std::string const& appName, std::string const& engineName, bool enableValidationLayer = true);
        inline VkResult GetLastErrorCode() const;
        static inline std::vector<std::string> GetAvailableLayers();
        ~Instance();
        inline operator VkInstance();
        bool GetPhysicalDevices(std::vector<VkPhysicalDevice>& deviceList);
        VkPhysicalDeviceFeatures GetPhysicalDeviceFeatures(VkPhysicalDevice& device);
        VkPhysicalDeviceMemoryProperties GetPhysicalDeviceMemoryProperties(VkPhysicalDevice& device);
        VkPhysicalDeviceProperties GetPhysicalDeviceProperties(VkPhysicalDevice& device);
        bool GetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice& device, std::vector<VkQueueFamilyProperties>& queueList);
        bool GetPhysicalDeviceExtensions(VkPhysicalDevice& device, std::vector<VkExtensionProperties>& extensions);
        bool GetPhysicalDeviceSurfaceSupport(VkPhysicalDevice& device, uint32_t index, VkSurfaceKHR const& surface, bool& presentSupport);
        std::optional<VkSurfaceCapabilitiesKHR> GetPhysicalDeviceSurfaceCapabilities(VkPhysicalDevice& device, VkSurfaceKHR const& surface);
        bool GetPhysicalDeviceSurfaceFormats(VkPhysicalDevice& device, VkSurfaceKHR const& surface, std::vector<VkSurfaceFormatKHR>& formats);
        bool GetPhysicalDeviceSurfacePresentModes(VkPhysicalDevice& device, VkSurfaceKHR const& surface, std::vector<VkPresentModeKHR>& presentModes);
        bool CreateImageView(VkDevice const& device, VkImageViewCreateInfo& info, VkImageView& imageView) const;
        void DestroyImageView(VkDevice const& device, VkImageView const& imageView) const;
        bool CreateRenderPass(VkDevice const& device, VkRenderPassCreateInfo const& info, VkRenderPass& renderPass) const;
        void DestroyRenderPass(VkDevice const& device, VkRenderPass const& renderPass) const;

    private:
        explicit Instance(VkInstanceCreateInfo const& info, VkAllocationCallbacks *callbacks = nullptr);

        VkInstance m_instance;
        VkAllocationCallbacks* m_allocationCallback;
        mutable VkResult m_lastErrorCode;

    };
}

#include "Instance.inl"

#endif //ENGINE_INSTANCE_HPP_INC
