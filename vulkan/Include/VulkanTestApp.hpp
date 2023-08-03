//
// Created by gcompte on 02/08/2023.
//

#pragma once

#ifndef ENGINE_VULKANTESTAPP_HPP_INC
#  define ENGINE_VULKANTESTAPP_HPP_INC

#include "Core/Application.hpp"
#include "Graphic/Window.hpp"
#  include "Graphic/Vulkan/VulkanRenderer.hpp"

namespace Croissant::VulkanTest {
    using FrameTime = Croissant::Core::FrameTime;
    using Window = Croissant::Graphic::Window;
    using VulkanRenderer = Croissant::Graphic::Vulkan::VulkanRenderer;
    class VulkanTestApp {
    public:
        explicit VulkanTestApp() = default;

        bool OnInitialize(Croissant::Core::Application<VulkanTestApp>& app);
        bool OnFrame(int32_t&, FrameTime const&, FrameTime const&);
        bool OnShutdown(Croissant::Core::Application<VulkanTestApp>& app);
    private:
        std::unique_ptr<Window> m_window;
        std::unique_ptr<VulkanRenderer> m_renderer;
    };
}

#endif //ENGINE_VULKANTESTAPP_HPP_INC
