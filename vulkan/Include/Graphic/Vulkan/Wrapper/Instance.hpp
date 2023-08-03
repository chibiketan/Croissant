//
// Created by gcompte on 02/08/2023.
//
#pragma once

#ifndef ENGINE_INSTANCE_HPP_INC
#  define ENGINE_INSTANCE_HPP_INC

#  include "Graphic/Vulkan/Utils.hpp"
#  include "Engine.hpp"
#  include <vector>


namespace Croissant::Graphic::Vulkan::Wrapper {
    /**
     * This class represents a Vulkan instance
     */
    class Instance {
    CLASS_NO_COPY_MOVE(Instance)
    public:
        static inline std::unique_ptr<Instance> Create(std::string const& appName, std::string const& engineName, bool enableValidationLayer = true);
        inline VkResult GetLastErrorCode() const;
        static inline std::vector<std::string> GetAvailableLayers();
        ~Instance();
        inline operator VkInstance();
        bool GetPhysicalDevices(std::vector<VkPhysicalDevice>& deviceList);

    private:
        explicit Instance(VkInstanceCreateInfo const& info, VkAllocationCallbacks *callbacks = nullptr);

        VkInstance m_instance;
        VkAllocationCallbacks* m_allocationCallback;
        mutable VkResult m_lastErrorCode;
    };
}

#include "Instance.inl"

#endif //ENGINE_INSTANCE_HPP_INC
