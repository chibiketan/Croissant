//
// Created by gcompte on 02/08/2023.
//

#include "Graphic/Vulkan/Utils.hpp"

namespace Croissant::Graphic::Vulkan::Wrapper {
    inline Instance::operator VkInstance() {
        return this->m_instance;
    }

    inline std::unique_ptr<Instance> Instance::Create(std::string const& appName, std::string const& engineName, bool enableValidationLayer) {
        VkApplicationInfo appInfo = {
                VK_STRUCTURE_TYPE_APPLICATION_INFO,
                nullptr,
                appName.data(),
                VK_MAKE_VERSION(1, 0, 0),
                engineName.data(),
                VK_MAKE_VERSION(1, 0, 0),
                VK_API_VERSION_1_3 // use api < 1.3 ?
        };

        // TODO how to get extensions ?
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

        // On itère sur la liste des extentions
        auto extentionList = std::vector<VkExtensionProperties>(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extentionList.data());

        std::cout << extensionCount << " extensions supported" << std::endl;
        for (auto const& extension : extentionList) {
            TRACE("- " << extension.extensionName);
        }
        std::vector<char const*> extensions{
                VK_KHR_SURFACE_EXTENSION_NAME,
                VK_KHR_WIN32_SURFACE_EXTENSION_NAME
        };
        std::vector<char const*> neededLayers{};
        if (enableValidationLayer) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            neededLayers.emplace_back("VK_LAYER_KHRONOS_validation");
        }

        auto instanceLayers = Instance::GetAvailableLayers();

        for (auto const& neededLayer : neededLayers) {
            using std::operator""s;
            bool found = false;
            std::string layerStr(neededLayer);

            for (auto const& layer : instanceLayers) {
                if (layer == layerStr) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                throw std::runtime_error("A needed layer is missing "s + layerStr);
            }
        }


        // Informations sur l'instance Vulkan à créer
        VkInstanceCreateInfo instanceCreateInfo{
            VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            nullptr,
            0,
            &appInfo,
            static_cast<uint32_t>(neededLayers.size()),
            neededLayers.data(),
            static_cast<uint32_t>(extensions.size()),
            extensions.data()
        };

        return std::unique_ptr<Instance>(new Instance{instanceCreateInfo});
    }

    inline std::vector<std::string> Instance::GetAvailableLayers() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        auto layers = std::vector<std::string>(layerCount);
        auto layerPropertiesList = std::vector<VkLayerProperties>(layerCount);

        vkEnumerateInstanceLayerProperties(&layerCount, layerPropertiesList.data());

        TRACE("Instance layer list :");
        for (auto const& layerProperties : layerPropertiesList) {
            TRACE("  - " << layerProperties.layerName << "(" << layerProperties.description << ")");
            layers.emplace_back(layerProperties.layerName);
        }

        return layers;
    }

    inline VkResult Instance::GetLastErrorCode() const {
        return this->m_lastErrorCode;
    }
}