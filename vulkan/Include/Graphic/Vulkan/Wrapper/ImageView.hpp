//
// Created by Grego on 15/02/2024.
//

#pragma once

#ifndef ENGINE_IMAGEVIEW_HPP_INC
#  define ENGINE_IMAGEVIEW_HPP_INC

#  include "Engine.hpp"
#  include "Graphic/Vulkan/Utils.hpp"
#  include "Graphic/Vulkan/Wrapper/SwapChain.hpp"

namespace Croissant::Graphic::Vulkan::Wrapper {
    class ImageView {
        CLASS_NO_COPY_MOVE(ImageView);
    public:
        ImageView(Instance const& instance, VkImage const& image, SwapChain const &swapChain, LogicalDevice const& device);
        ~ImageView();
        inline operator VkImageView() const;

    private:
        Instance const& m_instance;
        LogicalDevice const& m_device;
        VkImageView m_imageView;
    };
}

#include "ImageView.inl"

#endif //ENGINE_IMAGEVIEW_HPP_INC
