//
// Created by gcompte on 02/08/2023.
//
#pragma once

#ifndef ENGINE_UTILS_HPP_INC
#  define ENGINE_UTILS_HPP_INC

#  include "Engine.hpp"

#  ifdef CROISSANT_WINDOWS
#    define VK_USE_PLATFORM_WIN32_KHR
#  endif

#  include "volk.h"

namespace Croissant::Graphic::Vulkan {

    inline std::string convertErrorCodeToString(VkResult result) {
        switch (result)
        {
            case VK_SUCCESS:
                return "Aucune erreur";
            case VK_ERROR_OUT_OF_HOST_MEMORY:
                return "Plus d'espace mémoire disponible sur la machine";
            case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                return "Plus d'espace mémoire disponible sur la carte graphique";
            case VK_ERROR_INVALID_SHADER_NV:
                return "Une erreur est survenue lors de la compilation du shader. Plus d'informations disponible via l'extention VK_EXT_debug_report";
        }

        return "Erreur inconnue";
    }

}

#endif //ENGINE_UTILS_HPP_INC
