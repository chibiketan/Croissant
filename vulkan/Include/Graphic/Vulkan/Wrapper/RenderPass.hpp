//
// Created by Grego on 16/02/2024.
//

#pragma once

#ifndef ENGINE_RENDERPASS_HPP_INC
#  define ENGINE_RENDERPASS_HPP_INC
#  include "Graphic/Vulkan/Wrapper/Instance.hpp"
#  include "Graphic/Vulkan/Wrapper/LogicalDevice.hpp"
#  include "Graphic/Vulkan/Wrapper/SwapChain.hpp"

namespace Croissant::Graphic::Vulkan::Wrapper {
    class RenderPass {
    CLASS_NO_COPY_MOVE(RenderPass);
    public:
        RenderPass(Instance const& instance, LogicalDevice const& device, SwapChain const& swapChain);
        ~RenderPass();

    private:
        Instance const&         m_instance;
        LogicalDevice const&    m_device;
        VkRenderPass            m_renderPass;
    };
}


#endif //ENGINE_RENDERPASS_HPP_INC
