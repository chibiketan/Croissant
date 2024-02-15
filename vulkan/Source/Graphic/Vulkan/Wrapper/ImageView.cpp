//
// Created by Grego on 15/02/2024.
//

#include "Graphic/Vulkan/Wrapper/ImageView.hpp"

namespace Croissant::Graphic::Vulkan::Wrapper {
    ImageView::ImageView(const Instance &instance, const VkImage &image,
                         const SwapChain &swapChain,
                         const LogicalDevice &device) :
        m_instance { instance },
        m_device { device },
        m_imageView { nullptr }
    {
//        this->m_swapChainImageViews.resize(this->m_swapChainImages.size());
//
//        for (size_t i = 0; i < this->m_swapChainImages.size(); ++i) {
//            this->m_swapChainImageViews[i] = std::unique_ptr<VkImageView, std::function<void (VkImageView*)>>(new VkImageView{nullptr}, [&](VkImageView* imageView) { this->releaseSwapChainImageView(*imageView); });
        VkImageViewCreateInfo createInfo{};

        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = image;
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = swapChain.getFormat();
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        m_instance.CreateImageView(device, createInfo, m_imageView);

//            if (vkCreateImageView(*this->m_device, &createInfo, nullptr, this->m_swapChainImageViews[i].get()) != VK_SUCCESS) {
//                throw std::runtime_error("échec de la création d'une image view!");
//            }
//        }

    }

    ImageView::~ImageView()
    {
        if (nullptr != m_imageView) {
            std::cout << "Destroying ImageView instance" << std::endl;
            m_instance.DestroyImageView(m_device, m_imageView);
            m_imageView = nullptr;
        }
    }
}