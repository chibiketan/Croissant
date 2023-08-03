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

namespace Croissant::Graphic::Vulkan {
    class VulkanRenderer {
    CLASS_NO_COPY_MOVE(VulkanRenderer)
    public:
        VulkanRenderer(Window& window);
        ~VulkanRenderer() = default;

    private:
        void LoadPhysicalDevices();

        std::unique_ptr<Wrapper::Instance> m_instance;
        std::unique_ptr<Wrapper::Surface> m_surface;
        std::vector<Wrapper::PhysicalDevice> m_physicalDevices;
    };
}

#endif //ENGINE_VULKANRENDERER_HPP_INC
