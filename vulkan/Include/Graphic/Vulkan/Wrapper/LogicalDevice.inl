//
// Created by Grego on 17/08/2023.
//

namespace Croissant::Graphic::Vulkan::Wrapper
{
    VkQueue LogicalDevice::GetGraphicQueue() const
    {
        return m_graphicQueue;
    }

    VkQueue LogicalDevice::GetPresentQueue() const
    {
        return m_presentQueue;
    }

    LogicalDevice::operator VkDevice() const
    {
        return m_logicalDevice;
    }
}