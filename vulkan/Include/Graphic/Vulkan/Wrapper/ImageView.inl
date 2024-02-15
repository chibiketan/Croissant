//
// Created by Grego on 15/02/2024.
//

namespace Croissant::Graphic::Vulkan::Wrapper {
    ImageView::operator VkImageView() const
    {
        return m_imageView;
    }
}