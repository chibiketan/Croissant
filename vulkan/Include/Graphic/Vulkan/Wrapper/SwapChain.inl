//
// Created by Grego on 15/02/2024.
//

namespace Croissant::Graphic::Vulkan::Wrapper {
    VkFormat SwapChain::getFormat() const
    {
        return this->m_format;
    }

    VkExtent2D SwapChain::getExtent() const
    {
        return this->m_extent;
    }
}
