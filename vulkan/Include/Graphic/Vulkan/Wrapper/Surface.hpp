//
// Created by gcompte on 03/08/2023.
//
#pragma once
#ifndef ENGINE_SURFACE_HPP_INC
#define ENGINE_SURFACE_HPP_INC

#  include "Graphic/Vulkan/Utils.hpp"
#  include "Engine.hpp"
#  include "Graphic/Vulkan/Wrapper/Instance.hpp"
#  include "Graphic/Window.hpp"

namespace Croissant::Graphic::Vulkan::Wrapper {
    class Surface {
    CLASS_NO_COPY_MOVE(Surface)
    public:
        ~Surface();
        inline operator VkSurfaceKHR();
        static inline std::unique_ptr<Surface> Create(Instance& instance, Window& window, VkAllocationCallbacks* callbacks = nullptr);

    private:
        Surface(Instance& instance, Window& window, VkAllocationCallbacks* callbacks);

        Instance& m_instance;
        VkSurfaceKHR m_surface;
        VkAllocationCallbacks* m_callbacks;
        Window& m_window;
        mutable VkResult m_lastErrorCode;
    };
}

#endif //ENGINE_SURFACE_HPP_INC

#include "Surface.inl"