//
// Created by gcompte on 02/08/2023.
//
#include "Graphic/Vulkan/Wrapper/Instance.hpp"

namespace Croissant::Graphic::Vulkan::Wrapper {
    Instance::Instance(const VkInstanceCreateInfo &info, VkAllocationCallbacks *callbacks) :
        m_instance{nullptr},
        m_lastErrorCode{VK_SUCCESS}
    {
        // TODO create instance
        // Création de l'instance Vulkan
        this->m_allocationCallback = callbacks;
        this->m_lastErrorCode = vkCreateInstance(&info, callbacks, &this->m_instance);

        if (this->m_lastErrorCode != VK_SUCCESS) {
            TRACE("failed to create instance!");
            throw std::runtime_error("failed to create instance!");
        }

        volkLoadInstance(this->m_instance);
        // TODO implement debug layer loading
    }

    Instance::~Instance() {
        if (nullptr != this->m_instance) {
            vkDestroyInstance(this->m_instance, this->m_allocationCallback);
            this->m_instance = nullptr;
        }
    }

    bool Instance::GetPhysicalDevices(std::vector<VkPhysicalDevice>& deviceList) {
        uint32_t deviceCount {0};

        m_lastErrorCode = vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
        if (0 == deviceCount) {
            return false;
        }

        deviceList.clear();
        deviceList.resize(deviceCount);
        m_lastErrorCode = vkEnumeratePhysicalDevices(m_instance, &deviceCount, deviceList.data());
        return m_lastErrorCode == VK_SUCCESS;
    }

    VkPhysicalDeviceFeatures Instance::GetPhysicalDeviceFeatures(VkPhysicalDevice& device) {
        VkPhysicalDeviceFeatures features;
        vkGetPhysicalDeviceFeatures(device, &features);

        return features;
    }

    VkPhysicalDeviceMemoryProperties Instance::GetPhysicalDeviceMemoryProperties(VkPhysicalDevice& device) {
        VkPhysicalDeviceMemoryProperties properties;
        vkGetPhysicalDeviceMemoryProperties(device, &properties);

        return properties;
    }

    VkPhysicalDeviceProperties Instance::GetPhysicalDeviceProperties(VkPhysicalDevice& device) {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(device, &properties);

        return properties;
    }

    bool Instance::GetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice& device, std::vector<VkQueueFamilyProperties>& queueList) {
        uint32_t numQueue;

        vkGetPhysicalDeviceQueueFamilyProperties(device, &numQueue, nullptr);
        if (numQueue == 0) {
            return false;
        }

        queueList.resize(numQueue);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &numQueue, queueList.data());
        return true;
    }

    bool Instance::GetPhysicalDeviceExtensions(VkPhysicalDevice &device, std::vector<VkExtensionProperties>& extensions) {
        uint32_t numExtension;

        m_lastErrorCode = vkEnumerateDeviceExtensionProperties(device, nullptr, &numExtension, nullptr);

        if (m_lastErrorCode != VK_SUCCESS) {
            return false;
        }

        extensions.resize(numExtension);
        m_lastErrorCode = vkEnumerateDeviceExtensionProperties(device, nullptr, &numExtension, extensions.data());

        return m_lastErrorCode == VK_SUCCESS;
    }

    bool Instance::GetPhysicalDeviceSurfaceSupport(VkPhysicalDevice& device, uint32_t index, VkSurfaceKHR const& surface,
                                                   bool &presentSupport) {
        VkBool32 support;
        m_lastErrorCode = vkGetPhysicalDeviceSurfaceSupportKHR(device, index, surface, &support);

        if (m_lastErrorCode == VK_SUCCESS) {
            presentSupport = support == VK_TRUE;
        }

        return m_lastErrorCode == VK_SUCCESS;
    }

    std::optional<VkSurfaceCapabilitiesKHR>
    Instance::GetPhysicalDeviceSurfaceCapabilities(VkPhysicalDevice& device, VkSurfaceKHR const& surface) {
        VkSurfaceCapabilitiesKHR capabilities;

        m_lastErrorCode = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &capabilities);
        return m_lastErrorCode == VK_SUCCESS ? std::optional{capabilities} : std::nullopt;
    }

    bool Instance::GetPhysicalDeviceSurfaceFormats(VkPhysicalDevice &device, VkSurfaceKHR const &surface,
                                                   std::vector<VkSurfaceFormatKHR>& formats) {
        uint32_t count;
        m_lastErrorCode = vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, nullptr);

        if (m_lastErrorCode != VK_SUCCESS && m_lastErrorCode != VK_INCOMPLETE) {
            return false;
        }

        formats.resize(count);
        m_lastErrorCode = vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, formats.data());

        return m_lastErrorCode == VK_SUCCESS || m_lastErrorCode == VK_INCOMPLETE;
    }

    bool Instance::GetPhysicalDeviceSurfacePresentModes(VkPhysicalDevice& device, VkSurfaceKHR const& surface,
                                                        std::vector<VkPresentModeKHR>& presentModes) {
        uint32_t count;
        m_lastErrorCode = vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &count, nullptr);

        if (m_lastErrorCode != VK_SUCCESS && m_lastErrorCode != VK_INCOMPLETE) {
            return false;
        }

        presentModes.resize(count);
        m_lastErrorCode = vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &count, presentModes.data());

        return m_lastErrorCode == VK_SUCCESS || m_lastErrorCode == VK_INCOMPLETE;
    }

    bool Instance::CreateImageView(VkDevice const& device, VkImageViewCreateInfo& info, VkImageView& imageView) const
    {
        m_lastErrorCode = vkCreateImageView(device, &info, nullptr, &imageView);

        return m_lastErrorCode == VK_SUCCESS;
    }

    void Instance::DestroyImageView(const VkDevice &device, const VkImageView &imageView) const
    {
        vkDestroyImageView(device, imageView, nullptr);
    }

    bool Instance::CreateRenderPass(const VkDevice& device, const VkRenderPassCreateInfo& info,
                                    VkRenderPass& renderPass) const
    {
        auto error = m_lastErrorCode = vkCreateRenderPass(device, &info, nullptr, &renderPass);

        return error == VK_SUCCESS;
    }

    void Instance::DestroyRenderPass(const VkDevice &device, const VkRenderPass &renderPass) const
    {
        vkDestroyRenderPass(device, renderPass, nullptr);
    }

}