//
// Created by Grego on 20/08/2023.
//
#pragma once

#ifndef ENGINE_SWAPCHAIN_HPP_INC
#define ENGINE_SWAPCHAIN_HPP_INC

#include "Engine.hpp"
#include "Graphic/Vulkan/Wrapper/Instance.hpp"
#include "Graphic/Vulkan/Wrapper/PhysicalDevice.hpp"
#include "Graphic/Vulkan/Wrapper/LogicalDevice.hpp"

namespace Croissant::Graphic::Vulkan::Wrapper
{
    class SwapChain
    {
    CLASS_NO_COPY_MOVE(SwapChain)
    public:
        SwapChain(Instance& instance, PhysicalDevice& device, LogicalDevice& lDevice, Window& window, Surface& surface);
        ~SwapChain();

    private:
        Instance&               m_instance;
        PhysicalDevice&         m_device;
        LogicalDevice&          m_lDevice;
        Window&                 m_window;
        Surface&                m_surface;
        VkSwapchainKHR          m_swapchain;
        std::vector<VkImage>    m_swapImages;
        VkExtent2D              m_extent;
        VkFormat                m_format;
    };
}

#endif //ENGINE_SWAPCHAIN_HPP_INC
