#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>

int main(int argc, char** argv)
{
    // initialisation de la bibliothèque GLFW
    glfwInit();

    // Création de la fenêtre
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);

    // Récupération du nombre d'extention supportées
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::cout << extensionCount << " extensions supported" << std::endl;

    // glm::mat4 matrix;
    // glm::vec4 vec;
    // auto test = matrix * vec;

    // Création de l'instance Vulkan
    // informations sur l'application
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan test";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // récupération des extensions nécessaires pour fonctionner
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);


    // Informations sur l'instance Vulkan à créer
    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.enabledLayerCount = 0;

    // Création de l'instance Vulkan
    VkInstance instanceVulkan;
    VkResult creationResult = vkCreateInstance(&createInfo, nullptr, &instanceVulkan);

    if (creationResult != VK_SUCCESS) {
        std::cout << "failed to create instance!" << std::endl;
        throw std::runtime_error("failed to create instance!");
    }

    // Boucle de la fenêtre
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    // Suppression de l'instance Vulkan
    vkDestroyInstance(instanceVulkan, nullptr);
    // Suppression de la fenêtre
    glfwDestroyWindow(window);

    // Arrêt propre de GLFW
    glfwTerminate();

    return 0;
}