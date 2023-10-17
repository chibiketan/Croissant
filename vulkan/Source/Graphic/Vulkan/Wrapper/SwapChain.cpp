//
// Created by Grego on 20/08/2023.
//
#include "Graphic/Vulkan/Wrapper/SwapChain.hpp"

namespace Croissant::Graphic::Vulkan::Wrapper
{
    namespace internal
    {
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR> formats)
        {
            auto result = std::find_if(formats.begin(), formats.end(), [] (VkSurfaceFormatKHR format) {
                    return format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
                });

            if (result == formats.end()) {
                // not found, we use the first one
                return formats[0];
            }
            return *result;
        }

        VkExtent2D chooseSwapExtent(VkSurfaceCapabilitiesKHR capabilities, Window& window)
        {
            if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
                return capabilities.currentExtent;
            }

            VkExtent2D actualExtent = { static_cast<uint32_t>(window.Width()), static_cast<uint32_t>(window.Height()) };

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }

        VkPresentModeKHR chooseSwapPresentMode(std::vector<VkPresentModeKHR> const& availablePresentModes) {
            for (auto const& availablePresentMode : availablePresentModes) {
                if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                    return availablePresentMode;
                }
            }

            // toujours présent d'après la spec
            return VK_PRESENT_MODE_FIFO_KHR;
        }

    }

    SwapChain::SwapChain(Instance &instance,
                         PhysicalDevice &device, LogicalDevice& lDevice, Window& window, Surface& surface) :
                         m_instance{instance},
                         m_device{device},
                         m_lDevice{lDevice},
                         m_window{window},
                         m_surface{surface},
                         m_swapchain{nullptr}
    {
        auto imageCount = std::clamp(m_device.m_surfaceCapabilities->minImageCount + 1, 0u, m_device.m_surfaceCapabilities->maxImageCount);
        uint32_t queueFamilyIndices[] = {m_device.m_graphicalQueueIndex.value(), m_device.m_presentQueueIndex.value()};
        auto surfaceFormat = internal::chooseSwapSurfaceFormat(m_device.m_surfaceFormats);
        auto extent = internal::chooseSwapExtent(m_device.m_surfaceCapabilities.value(), m_window);
        auto presentMode = internal::chooseSwapPresentMode(m_device.m_presentModes);

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = m_surface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        if (m_device.m_graphicalQueueIndex != m_device.m_presentQueueIndex) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        } else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0; // Optionnel
            createInfo.pQueueFamilyIndices = nullptr; // Optionnel
        }
        if (m_device.m_surfaceCapabilities)
        {
            createInfo.preTransform = m_device.m_surfaceCapabilities->currentTransform;
        }
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(m_lDevice, &createInfo, nullptr, &m_swapchain) != VK_SUCCESS) {
            throw std::runtime_error("Swapchain creation failed");
        }



        uint32_t swapimageCount;

        vkGetSwapchainImagesKHR(m_lDevice, m_swapchain, &swapimageCount, nullptr);
        this->m_swapImages.resize(swapimageCount);
        vkGetSwapchainImagesKHR(m_lDevice, m_swapchain, &swapimageCount, this->m_swapImages.data());
        this->m_extent = extent;
        this->m_format = surfaceFormat.format;
    }

    SwapChain::~SwapChain()
    {
        vkDestroySwapchainKHR(m_lDevice, m_swapchain, nullptr);
    }
}