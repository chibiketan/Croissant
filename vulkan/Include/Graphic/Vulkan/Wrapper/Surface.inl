//
// Created by gcompte on 03/08/2023.
//
namespace Croissant::Graphic::Vulkan::Wrapper {
    inline Surface::operator VkSurfaceKHR() {
        return this->m_surface;
    }

    inline std::unique_ptr<Surface> Surface::Create(Instance& instance, Window& window, VkAllocationCallbacks* callbacks) {
        return std::unique_ptr<Surface>{new Surface(instance, window, callbacks)};
    }
}