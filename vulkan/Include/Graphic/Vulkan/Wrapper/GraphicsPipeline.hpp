//
// Created by Grego on 19/03/2024.
//

#pragma once

#ifndef ENGINE_GRAPHICSPIPELINE_HPP_INC
#  define ENGINE_GRAPHICSPIPELINE_HPP_INC

#  include "Graphic/Vulkan/Utils.hpp"
#  include "Graphic/Vulkan/Wrapper/Instance.hpp"
#  include "Graphic/Vulkan/Wrapper/ShaderModule.hpp"
#  include "Graphic/Vulkan/Wrapper/SwapChain.hpp"
#  include "Graphic/Vulkan/Wrapper/RenderPass.hpp"

namespace Croissant::Graphic::Vulkan::Wrapper {
    class GraphicsPipeline {
    CLASS_NO_COPY_MOVE(GraphicsPipeline);
    public:
        GraphicsPipeline(Instance const& instance, LogicalDevice const& device, RenderPass const& renderPass, SwapChain const& swapChain, ShaderModule const& vertexShader, ShaderModule const& fragmentShader);
        ~GraphicsPipeline();

    private:

        Instance const& m_instance;
        LogicalDevice const& m_device;
        RenderPass const& m_renderPass;
        VkPipelineLayout m_pipelineLayout;
        VkPipeline m_pipeline;
    };
}

#endif //ENGINE_GRAPHICSPIPELINE_HPP_INC
