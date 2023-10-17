//
// Created by Grego on 04/08/2023.
//

#include <set>

namespace Croissant::Graphic::Vulkan::Wrapper {
    inline PhysicalDevice::PhysicalDevice(Instance& instance, Wrapper::Surface& surface, VkPhysicalDevice device) :
        m_features{},
        m_memoryProperties{},
        m_properties{},
        m_queueFamillies{},
        m_extensions{},
        m_supportSwap{false},
        m_instance(instance),
        m_surface{surface},
        m_device{device}{
        m_features         = m_instance.GetPhysicalDeviceFeatures(m_device);
        m_memoryProperties = m_instance.GetPhysicalDeviceMemoryProperties(m_device);
        m_properties = m_instance.GetPhysicalDeviceProperties(m_device);
        if (!m_instance.GetPhysicalDeviceQueueFamilyProperties(m_device, m_queueFamillies)) {
            TRACE("Aucune famille de queues pour le device ["<<m_properties.deviceName<<"]");
        }

        if (!instance.GetPhysicalDeviceExtensions(m_device, m_extensions)) {
            TRACE("Impossible de récupérer les extensions pour le device ["<<m_properties.deviceName<<"]");
        }

        for (uint32_t i = 0; i < m_queueFamillies.size(); ++i) {
            auto& queueFamily = m_queueFamillies[i];
            bool presentSupport = false;

            TRACE("\"------++++++++ queue count"<<queueFamily.queueCount);
            TRACE("\"------++++++++ queue flags"<<queueFamily.queueFlags);

            m_instance.GetPhysicalDeviceSurfaceSupport(m_device, i, m_surface, presentSupport);
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                m_graphicalQueueIndex = i;
            }

            if (presentSupport) {
                m_presentQueueIndex = i;
            }

            if (m_presentQueueIndex.has_value() && m_graphicalQueueIndex.has_value()) {
                break;
            }
        }

        m_supportSwap = CheckSwapSupport();

        TRACE("------ Device Name : "<<m_properties.deviceName);
        TRACE("------++++ queue family count : "<<m_queueFamillies.size());
        TRACE("------++++ extension count : "<<m_extensions.size());
        TRACE("------++++ graphical family queue index : "<<m_graphicalQueueIndex.value());
        TRACE("------++++ present family queue index : "<<m_presentQueueIndex.value());
        TRACE("------++++ Swapchain support : "<<m_supportSwap);
        if (m_supportSwap) {
            TRACE("------++++++ Swap min image count : "<<m_surfaceCapabilities->minImageCount);
            TRACE("------++++++ Swap max image count : "<<m_surfaceCapabilities->maxImageCount);
            TRACE("------++++++ Swap max image array layers : "<<m_surfaceCapabilities->maxImageArrayLayers);
            TRACE("------++++++ Swap current extent : { width : "<<m_surfaceCapabilities->currentExtent.width<<", height : "<<m_surfaceCapabilities->currentExtent.height<<"}");
        }
    }

    bool PhysicalDevice::CheckSwapSupport() {
        // Check extension support
        std::set<std::string> requiredExtensions{
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        for (auto const& extensionProperties: m_extensions) {
            requiredExtensions.erase(extensionProperties.extensionName);
        }

        if (!requiredExtensions.empty()) {
            // At least one needed extension is missing
            return false;
        }

        m_surfaceCapabilities = m_instance.GetPhysicalDeviceSurfaceCapabilities(m_device, m_surface);
        m_instance.GetPhysicalDeviceSurfaceFormats(m_device, m_surface, m_surfaceFormats);
        m_instance.GetPhysicalDeviceSurfacePresentModes(m_device, m_surface, m_presentModes);
        m_supportSwap = !m_presentModes.empty() && !m_surfaceFormats.empty();

        return true;
    }

    inline PhysicalDevice::operator VkPhysicalDevice() {
        return m_device;
    }
}