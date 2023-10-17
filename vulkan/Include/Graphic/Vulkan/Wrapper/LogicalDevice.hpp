//
// Created by Grego on 15/08/2023.
//
#pragma once

#ifndef ENGINE_LOGICALDEVICE_HPP_INC
#define ENGINE_LOGICALDEVICE_HPP_INC

#include "Engine.hpp"
#include "Graphic/Vulkan/Wrapper/Instance.hpp"
#include "Graphic/Vulkan/Wrapper/PhysicalDevice.hpp"

namespace Croissant::Graphic::Vulkan::Wrapper {
    class LogicalDevice {
        CLASS_NO_COPY_MOVE(LogicalDevice)
    public:
        LogicalDevice(Instance& instance, PhysicalDevice& device);
        ~LogicalDevice();
        inline operator VkDevice();
        inline VkQueue GetGraphicQueue() const;
        inline VkQueue GetPresentQueue() const;

    private:
        Instance&       m_instance;
        PhysicalDevice& m_device;
        VkDevice        m_logicalDevice;
        VkQueue         m_graphicQueue;
        VkQueue         m_presentQueue;
    };
}

#include "Graphic/Vulkan/Wrapper/LogicalDevice.inl"

#endif //ENGINE_LOGICALDEVICE_HPP_INC
