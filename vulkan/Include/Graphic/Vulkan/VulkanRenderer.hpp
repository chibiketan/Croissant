//
// Created by gcompte on 02/08/2023.
//
#pragma once

#ifndef ENGINE_VULKANRENDERER_HPP_INC
#  define ENGINE_VULKANRENDERER_HPP_INC

#  include "Engine.hpp"
#  include "Graphic/Vulkan/Utils.hpp"
#  include "Graphic/Vulkan/Wrapper/Instance.hpp"
#  include "Graphic/Vulkan/Wrapper/Surface.hpp"
#  include "Graphic/Vulkan/Wrapper/PhysicalDevice.hpp"
#  include <list>
#  include "Graphic/Vulkan/Wrapper/LogicalDevice.hpp"
#  include "Graphic/Vulkan/Wrapper/SwapChain.hpp"
#  include "Graphic/Vulkan/Wrapper/RenderPass.hpp"

namespace Croissant::Graphic::Vulkan {
    class VulkanRenderer {
    CLASS_NO_COPY_MOVE(VulkanRenderer)
    public:
        explicit VulkanRenderer(Window& window);
        ~VulkanRenderer() = default;

    private:
        void LoadPhysicalDevices();
        Wrapper::PhysicalDevice& PickPhysicalDevice();
        bool IsDeviceSuitable(Wrapper::PhysicalDevice& device);

        std::unique_ptr<Wrapper::Instance> m_instance;
        std::unique_ptr<Wrapper::Surface> m_surface;
        std::list<Wrapper::PhysicalDevice> m_physicalDevices;
        Wrapper::PhysicalDevice*    m_currentDevice;
        std::unique_ptr<Wrapper::LogicalDevice> m_logicalDevice;
        std::unique_ptr<Wrapper::SwapChain> m_swapChain;
        std::unique_ptr<Wrapper::RenderPass> m_renderPass;
    };
}

#endif //ENGINE_VULKANRENDERER_HPP_INC
