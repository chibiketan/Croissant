//
// Created by Grego on 19/02/2024.
//

#pragma once

#ifndef ENGINE_SHADERMODULE_HPP_INC
#  define ENGINE_SHADERMODULE_HPP_INC
#  include "Graphic/Vulkan/Utils.hpp"
#  include "Graphic/Vulkan/Wrapper/Instance.hpp"
#  include "Graphic/Vulkan/Wrapper/LogicalDevice.hpp"

namespace Croissant::Graphic::Vulkan::Wrapper {
    class ShaderModule {
    CLASS_NO_COPY_MOVE(ShaderModule);
    public:
        ShaderModule(Instance const& instance, LogicalDevice const& device, std::vector<char> const& code);
        ~ShaderModule();
        operator VkShaderModule() const;

    private:
        Instance const& m_instance;
        LogicalDevice const& m_device;
        VkShaderModule m_shaderModule;
    };
}

#endif //ENGINE_SHADERMODULE_HPP_INC
