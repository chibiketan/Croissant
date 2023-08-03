//
// Created by gcompte on 03/08/2023.
//
#pragma once

#ifndef ENGINE_PHYSICALDEVICE_HPP_INC
#  define ENGINE_PHYSICALDEVICE_HPP_INC

#include "Graphic/Vulkan/Utils.hpp"

namespace Croissant::Graphic::Vulkan::Wrapper {
    class PhysicalDevice {
        VkPhysicalDevice m_device;
    };
}

#endif //ENGINE_PHYSICALDEVICE_HPP_INC
