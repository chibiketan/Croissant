//
// Created by Grego on 16/02/2024.
//

#include "Graphic/Vulkan/Wrapper/RenderPass.hpp"

namespace Croissant::Graphic::Vulkan::Wrapper {
    RenderPass::RenderPass(const Croissant::Graphic::Vulkan::Wrapper::Instance &instance,
                           const Croissant::Graphic::Vulkan::Wrapper::LogicalDevice &device,
                           const Croissant::Graphic::Vulkan::Wrapper::SwapChain &swapChain) :
        m_instance { instance },
        m_device { device },
        m_renderPass { nullptr }
    {
        VkAttachmentDescription colorAttachment{};

        colorAttachment.format = swapChain.getFormat();
        colorAttachment.samples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VkImageLayout::VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;


        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;


        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        // add subpass
        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;


        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        if (!this->m_instance.CreateRenderPass(this->m_device, renderPassInfo, this->m_renderPass)) {
            throw std::runtime_error("échec de la création de la render pass!");
        }
    }

    RenderPass::~RenderPass()
    {
        if (nullptr != m_renderPass) {
            this->m_instance.DestroyRenderPass(this->m_device, this->m_renderPass);
            m_renderPass = nullptr;
        }
    }
}