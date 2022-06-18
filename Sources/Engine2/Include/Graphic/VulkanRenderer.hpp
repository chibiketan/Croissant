#ifndef CROISSANT_ENGINE_GRAPHIC_VULKANRENDERER_HPP_INC
#  define CROISSANT_ENGINE_GRAPHIC_VULKANRENDERER_HPP_INC
#  include "Engine.hpp"
#  include "Exception/CroissantException.hpp"
#  include "vulkan/vulkan.h"

using VulkanRendererException = Croissant::Exception::CroissantException;

namespace Croissant
{
    namespace Graphic
    {
        class ENGINE_API VulkanRenderer
        {
        public:
            VulkanRenderer();
            ~VulkanRenderer();
        protected:
        private:
            VkInstance m_instance;
            std::vector<VkExtensionProperties> m_instanceExtensions{ 0 };

            void CreateInstance();
            void LoadInstanceExtensions();
            bool DoInstanceExtensionExists() const;
        };
    }
}

#endif //CROISSANT_ENGINE_GRAPHIC_VULKANRENDERER_HPP_INC
