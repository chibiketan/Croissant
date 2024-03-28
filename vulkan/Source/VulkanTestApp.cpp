//
// Created by gcompte on 02/08/2023.
//
#include "VulkanTestApp.hpp"
#include "Graphic/WindowEvent.hpp"

bool Croissant::VulkanTest::VulkanTestApp::OnInitialize(Croissant::Core::Application<VulkanTestApp> &) {
    // nothing to do for now
    Core::WriteTraceDebug("Croissant::VulkanTest::VulkanTestApp::OnInitialize called");
    // create window
    this->m_window = std::make_unique<Window>(800, 600, "VulkanTest unique window");
    // create renderer
    this->m_renderer = std::make_unique<VulkanRenderer>(*this->m_window);

    // show window
    this->m_window->Open();
    this->m_window->CenterCursor();
    return true;
}

bool Croissant::VulkanTest::VulkanTestApp::OnFrame(int32_t &, const Croissant::VulkanTest::FrameTime &,
                                                   const Croissant::VulkanTest::FrameTime &) {
    //Core::WriteTraceDebug("Croissant::VulkanTest::VulkanTestApp::OnFrame called");
    // TODO : genereate data / step forward world
    // TODO : render frame
    auto event = this->m_window->PeekEvent();

    // if we close the window, we stop the app.
    if (event->GetType() == Graphic::WindowEventType::CLOSE) {
        return false;
    }

    return true;
}

bool Croissant::VulkanTest::VulkanTestApp::OnShutdown(Croissant::Core::Application<VulkanTestApp> &) {
    // nothing to do for now
    Core::WriteTraceDebug("Croissant::VulkanTest::VulkanTestApp::OnShutdown called");
    // TODO Clean whatever we need to clean
    this->m_window.reset(nullptr);
    return true;
}
