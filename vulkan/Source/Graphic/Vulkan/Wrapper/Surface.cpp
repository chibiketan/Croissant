//
// Created by gcompte on 03/08/2023.
//
#include "Graphic/Vulkan/Wrapper/Surface.hpp"

namespace Croissant::Graphic::Vulkan::Wrapper {
    Surface::~Surface() {
        if (nullptr != m_surface) {
            vkDestroySurfaceKHR(m_instance, m_surface, m_callbacks);
            m_surface = nullptr;
        }
    }

    Surface::Surface(Instance& instance, Window& window, VkAllocationCallbacks* callbacks) :
        m_instance{instance},
        m_surface{nullptr},
        m_callbacks{callbacks},
        m_window{window},
        m_lastErrorCode{VK_SUCCESS}
    {
        // TODO create surface
#ifdef CROISSANT_WINDOWS
        VkWin32SurfaceCreateInfoKHR surfaceInfo{
            VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
            nullptr,
            0,
            reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(window.GetSystemHandle(), GWLP_HINSTANCE)),
            window.GetSystemHandle()
        };

        m_lastErrorCode = vkCreateWin32SurfaceKHR((VkInstance)instance, &surfaceInfo, callbacks, &m_surface);
#else
#  error "Plateform not supported"
#endif

        if (this->m_lastErrorCode != VK_SUCCESS) {
            // TODO better message ?
            throw std::runtime_error("Une erreur est survenue lors de la création de la surface");
        }
    }
}