//
// Created by Grego on 19/02/2024.
//

#include "Graphic/Vulkan/Wrapper/ShaderModule.hpp"

namespace Croissant::Graphic::Vulkan::Wrapper {

    ShaderModule::ShaderModule(const Croissant::Graphic::Vulkan::Wrapper::Instance &instance,
                               const Croissant::Graphic::Vulkan::Wrapper::LogicalDevice &device,
                               std::vector<char> const& code) :
       m_instance { instance },
       m_device { device },
       m_shaderModule { nullptr }
    {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if (!m_instance.CreateShaderModule(m_device, createInfo, m_shaderModule)) {
            throw std::runtime_error("échec de la création d'un module shader!");
        }

        Core::WriteTraceDebug("création d'un module shader!");
    }

    ShaderModule::~ShaderModule()
    {
        if (nullptr != this->m_shaderModule) {
            Core::WriteTraceDebug("destruction d'un module shader!");
            m_instance.DestroyShaderModule(m_device, m_shaderModule);
            this->m_shaderModule = nullptr;
        }
    }

    ShaderModule::operator VkShaderModule() const
    {
        return this->m_shaderModule;
    }

}