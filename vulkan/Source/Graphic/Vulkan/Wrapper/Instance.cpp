//
// Created by gcompte on 02/08/2023.
//
#include "Graphic/Vulkan/Wrapper/Instance.hpp"

namespace Croissant::Graphic::Vulkan::Wrapper {
    Instance::Instance(const VkInstanceCreateInfo &info, VkAllocationCallbacks *callbacks) :
        m_instance{nullptr},
        m_lastErrorCode{VK_SUCCESS}
    {
        // TODO create instance
        // Création de l'instance Vulkan
        this->m_allocationCallback = callbacks;
        this->m_lastErrorCode = vkCreateInstance(&info, callbacks, &this->m_instance);

        if (this->m_lastErrorCode != VK_SUCCESS) {
            TRACE("failed to create instance!");
            throw std::runtime_error("failed to create instance!");
        }

        volkLoadInstance(this->m_instance);
        // TODO implement debug layer loading
    }

    Instance::~Instance() {
        if (nullptr != this->m_instance) {
            vkDestroyInstance(this->m_instance, this->m_allocationCallback);
            this->m_instance = nullptr;
        }
    }

    bool Instance::GetPhysicalDevices(std::vector<VkPhysicalDevice>& deviceList) {
        uint32_t deviceCount {0};

        m_lastErrorCode = vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
        if (0 == deviceCount) {
            return false;
        }

        deviceList.clear();
        deviceList.resize(deviceCount);
        m_lastErrorCode = vkEnumeratePhysicalDevices(m_instance, &deviceCount, deviceList.data());
        return m_lastErrorCode == VK_SUCCESS;
    }
}