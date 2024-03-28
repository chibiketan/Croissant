//
// Created by Grego on 19/03/2024.
//
#include "Graphic/Vulkan/Wrapper/GraphicsPipeline.hpp"

namespace Croissant::Graphic::Vulkan::Wrapper {
    GraphicsPipeline::GraphicsPipeline(Instance const& instance, LogicalDevice const& device, RenderPass const& renderPass, SwapChain const& swapChain, ShaderModule const& vertexShader, ShaderModule const& fragmentShader) :
        m_instance{instance},
        m_device{device},
        m_renderPass{renderPass},
        m_pipelineLayout {nullptr},
        m_pipeline{nullptr}{
//        auto vertShaderCode = readFile("vulkan/shaders/shader.vert.spv");
//        auto fragShaderCode = readFile("vulkan/shaders/shader.frag.spv");
//
//        std::unique_ptr<VkShaderModule, std::function<void (VkShaderModule*)>> vertShaderModule = createShaderModule(vertShaderCode);
//        std::unique_ptr<VkShaderModule, std::function<void (VkShaderModule*)>> fragShaderModule = createShaderModule(fragShaderCode);
//
//
        VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertexShader;
        vertShaderStageInfo.pName = "main";


        VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = fragmentShader;
        fragShaderStageInfo.pName = "main";


        VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};


        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 0;
        vertexInputInfo.pVertexBindingDescriptions = nullptr; // Optionnel
        vertexInputInfo.vertexAttributeDescriptionCount = 0;
        vertexInputInfo.pVertexAttributeDescriptions = nullptr; // Optionnel


        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float) swapChain.getExtent().width;
        viewport.height = (float) swapChain.getExtent().height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;


        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = swapChain.getExtent();


        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;
        rasterizer.depthBiasConstantFactor = 0.0f; // Optionnel
        rasterizer.depthBiasClamp = 0.0f; // Optionnel
        rasterizer.depthBiasSlopeFactor = 0.0f; // Optionnel


        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampling.minSampleShading = 1.0f; // Optionnel
        multisampling.pSampleMask = nullptr; // Optionnel
        multisampling.alphaToCoverageEnable = VK_FALSE; // Optionnel
        multisampling.alphaToOneEnable = VK_FALSE; // Optionnel


        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optionnel
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optionnel
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optionnel
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optionnel
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optionnel
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optionnel
        // Exemple avec du blend des couleurs
        //colorBlendAttachment.blendEnable = VK_TRUE;
        //colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        //colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        //colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        //colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        //colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        //colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;


        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optionnel
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f; // Optionnel
        colorBlending.blendConstants[1] = 0.0f; // Optionnel
        colorBlending.blendConstants[2] = 0.0f; // Optionnel
        colorBlending.blendConstants[3] = 0.0f; // Optionnel

        std::vector<VkDynamicState> dynamicStates = {
                VK_DYNAMIC_STATE_VIEWPORT,
                VK_DYNAMIC_STATE_SCISSOR,
                VK_DYNAMIC_STATE_LINE_WIDTH
        };

        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicState.pDynamicStates = dynamicStates.data();

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;            // Optionnel
        pipelineLayoutInfo.pSetLayouts = nullptr;         // Optionnel
        pipelineLayoutInfo.pushConstantRangeCount = 0;    // Optionnel
        pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optionnel

        if (!this->m_instance.CreatePipelineLayout(this->m_device, pipelineLayoutInfo, this->m_pipelineLayout)) {
            throw std::runtime_error("échec de la création du pipeline layout!");
        }

        // Liaison de tous les éléments de la pipeline ensemble
        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = nullptr; // optionnel
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = &dynamicState; // optionnel
        pipelineInfo.layout = this->m_pipelineLayout;
        pipelineInfo.renderPass = this->m_renderPass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optionnel
        pipelineInfo.basePipelineIndex = -1; // Optionnel

        if (!this->m_instance.CreateGraphicsPipelines(this->m_device,  pipelineInfo, this->m_pipeline)) {
            throw std::runtime_error("échec de la création de la pipeline graphique!");
        }
        Core::WriteTraceDebug("Graphics pipeline créée avec succès");
    }

    GraphicsPipeline::~GraphicsPipeline()
    {
        if (this->m_pipeline) {
            m_instance.DestroyGraphicsPipeline(this->m_device, this->m_pipeline);
            this->m_pipeline = nullptr;
            Core::WriteTraceDebug("Graphics pipeline détruite avec succès");
        }

        if (this->m_pipelineLayout) {
            m_instance.DestroyPipelineLayout(this->m_device, this->m_pipelineLayout);
            this->m_pipelineLayout = nullptr;
            Core::WriteTraceDebug("Pipeline layout détruit avec succès");
        }
    }
}