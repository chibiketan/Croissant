#define GLFW_INCLUDE_VULKAN
#define VK_NO_PROTOTYPES
#include "defines.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <utility>
#include <vector>
#include <set>
#include <string>
#include <memory>
#include <functional>
#include <optional>
#include <volk.h>
#include <limits>

struct GlfwWindowDestructor {
    void operator()(GLFWwindow* window) {
        std::cout << "Release Window" << std::endl;
        glfwDestroyWindow(window);
    }
};

struct VulkanInstanceDestructor {
    void operator()(VkInstance* instance) {
        if (*instance != nullptr) {
            std::cout << "Release vulkan instance" << std::endl;
            vkDestroyInstance(*instance, nullptr);
        }
    }
};

struct VulkanDeviceDestructor {
    void operator()(VkDevice* device) {
        if (*device != nullptr) {
            std::cout << "Release Device" << std::endl;
            vkDestroyDevice(*device, nullptr);
        }
    }
};

class TestVulkan
{
public:

    TestVulkan(int width, int height, std::string title)
        : m_width(width), m_height(height), m_title(std::move(title))
        , m_window(glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr))
        , m_instanceVulkan(new VkInstance(nullptr))
        , m_debugMessenger(new VkDebugUtilsMessengerEXT(nullptr), [&](VkDebugUtilsMessengerEXT* item) { this->releaseDebugUtilsMessenger(*item); })
        , m_surface{ new VkSurfaceKHR{nullptr}, [&](VkSurfaceKHR* item) { this->releaseSurface(*item); }}
        , m_physicalDevice{nullptr}
        , m_device{new VkDevice(nullptr)}
        , m_swapChain{ new VkSwapchainKHR{nullptr}, [&](VkSwapchainKHR* item) { this->releaseSwapchain(*item); }}
        , m_graphicsQueue{nullptr}
        , m_presentQueue{nullptr}
        , m_swapChainImageFormat{}
        , m_swapChainExtent{}
        , m_swapChainImages{}
        , m_swapChainImageViews{}
    {

    }

    ~TestVulkan() noexcept(true)
    {
        cleanup();
    }

    void initialize()
    {
        this->initVulkan();
        this->setupDebugLayer();
        this->createSurface();
        this->pickPhysicalDevice();
        this->createLogicalDevice();
        this->createSwapChain();
        this->createImageViews();
    }

    void run()
    {
        while(!glfwWindowShouldClose(m_window.get())) {
            glfwPollEvents();
        }
    }


private:
    int m_width;
    int m_height;
    std::string m_title;
    std::unique_ptr<GLFWwindow, GlfwWindowDestructor> m_window;
    std::unique_ptr<VkInstance, VulkanInstanceDestructor> m_instanceVulkan;
    std::unique_ptr<VkDebugUtilsMessengerEXT, std::function<void (VkDebugUtilsMessengerEXT*)>> m_debugMessenger;
    std::unique_ptr<VkSurfaceKHR, std::function<void (VkSurfaceKHR*)>> m_surface;
    VkPhysicalDevice m_physicalDevice;
    std::unique_ptr<VkDevice, VulkanDeviceDestructor> m_device;
    std::unique_ptr<VkSwapchainKHR, std::function<void (VkSwapchainKHR*)>> m_swapChain;
    VkQueue m_graphicsQueue;
    VkQueue m_presentQueue;
    VkFormat m_swapChainImageFormat;
    VkExtent2D m_swapChainExtent;
    std::vector<VkImage> m_swapChainImages;
    std::vector<std::unique_ptr<VkImageView, std::function<void (VkImageView*)>>> m_swapChainImageViews;
#ifdef CROISSANT_VULKAN_VALIDATION_LAYER
    constexpr static bool enableValidationLayer = true;
#else
    constexpr static bool enableValidationLayer = false;
#endif

    std::vector<std::string> getInstanceLayers() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        auto layers = std::vector<std::string>(layerCount);
        auto layerPropertiesList = std::vector<VkLayerProperties>(layerCount);

        vkEnumerateInstanceLayerProperties(&layerCount, layerPropertiesList.data());

        std::cout << "Instance layout list :" << std::endl;
        for (auto const& layerProperties : layerPropertiesList) {
            std::cout << "  - " << layerProperties.layerName << "(" << layerProperties.description << ")" << std::endl;
            layers.emplace_back(layerProperties.layerName);
        }

        return layers;
    }

    void initVulkan() {
        // TODO remove ???
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

        // On itère sur la liste des extentions
        auto extentionList = std::vector<VkExtensionProperties>(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extentionList.data());

        std::cout << extensionCount << " extensions supported" << std::endl;
        for (auto const& extension : extentionList) {
            std::cout << "- " << extension.extensionName << std::endl;
        }
        // END TODO remove ???

        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Vulkan test";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        // récupération des extensions nécessaires pour fonctionner (à extraire dans une méthode)
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;

        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        std::vector<char const *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
        std::vector<char const*> neededLayers{};
        if (enableValidationLayer) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            neededLayers.emplace_back("VK_LAYER_KHRONOS_validation");
        }

        auto instanceLayers = getInstanceLayers();

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
        VkInstanceCreateInfo createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledExtensionCount = extensions.size();
        createInfo.ppEnabledExtensionNames = extensions.data();
        createInfo.enabledLayerCount = neededLayers.size();
        if (!neededLayers.empty()) {
            createInfo.ppEnabledLayerNames = neededLayers.data();
        }

        // Création de l'instance Vulkan
        VkResult creationResult = vkCreateInstance(&createInfo, nullptr, m_instanceVulkan.get());

        if (creationResult != VK_SUCCESS) {
            std::cout << "failed to create instance!" << std::endl;
            throw std::runtime_error("failed to create instance!");
        }

        volkLoadInstance(*m_instanceVulkan.get());
    }

    void setupDebugLayer() {
        if (!enableValidationLayer) {
            // nothing to do
            return;
        }

        VkDebugUtilsMessengerCreateInfoEXT createInfo{};

        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
                | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
        createInfo.pUserData = this;

        if (this->createDebugUtilsMesengerEXT(&createInfo, m_debugMessenger.get()) != VK_SUCCESS) {
            throw std::runtime_error("Unable to create debug messenger");
        }
    }

    void createSurface() {
        if (VK_SUCCESS != glfwCreateWindowSurface(*this->m_instanceVulkan, this->m_window.get(), nullptr, this->m_surface.get())) {
            throw std::runtime_error("Unable to create window's surface");
        }
    }

    void cleanup()
    {
    }

    VkResult createDebugUtilsMesengerEXT(VkDebugUtilsMessengerCreateInfoEXT const* createInfo, VkDebugUtilsMessengerEXT* messenger) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(*m_instanceVulkan, "vkCreateDebugUtilsMessengerEXT");
        if (nullptr != func) {
            return func(*m_instanceVulkan, createInfo, nullptr, messenger);
        } else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void releaseDebugUtilsMessenger(VkDebugUtilsMessengerEXT messenger) {
        if (nullptr == messenger || m_instanceVulkan == nullptr) {
            // what to do if m_instanceVulkan is null ?
            // safe guard
            return;
        }

        std::cout << "Release Debug utils messenger" << std::endl;

        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(*m_instanceVulkan, "vkDestroyDebugUtilsMessengerEXT");
        if (nullptr != func) {
            func(*m_instanceVulkan, messenger, nullptr);
        }
    }

    void releaseSurface(VkSurfaceKHR surface) {
        if (nullptr == surface) {
            return;
        }

        std::cout << "Release Surface" << std::endl;

        vkDestroySurfaceKHR(*this->m_instanceVulkan, surface, nullptr);
    }

    void releaseSwapchain(VkSwapchainKHR swapchain) {
        if (nullptr == swapchain) {
            return;
        }

        std::cout << "Release Swapchain" << std::endl;

        vkDestroySwapchainKHR(*this->m_device, swapchain, nullptr);
    }

    void releaseSwapChainImageView(VkImageView imageView) {
        if (nullptr == imageView) {
            return;
        }

        std::cout << "Release Swapchain Image View" << std::endl;

        vkDestroyImageView(*this->m_device, imageView, nullptr);
    }


    bool isDeviceSuitable(VkPhysicalDevice device) {
        VkPhysicalDeviceProperties deviceProperties{};
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        VkPhysicalDeviceFeatures deviceFeatures{};
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
        auto indices = this->findQueueFamilies(device);
        auto extensionsSupported = checkDeviceExtensionSupport(device);
        auto swapChainAdequate = false;

        if (extensionsSupported) {
            auto swapChainSupportDetails = querySwapChainSupport(device);
            swapChainAdequate = !swapChainSupportDetails.presentModes.empty() && !swapChainSupportDetails.formats.empty();
        }

        return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
            && deviceFeatures.geometryShader
            && indices.isComplete() && extensionsSupported
            && swapChainAdequate;
    }

    static bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice) {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> availableExtensions{extensionCount};
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

        auto physicalDeviceExtensions = getDeviceExtensions();
        std::set<std::string> requiredExtensions{physicalDeviceExtensions.begin(), physicalDeviceExtensions.end()};

        for (auto const& extensionProperties: availableExtensions) {
            requiredExtensions.erase(extensionProperties.extensionName);
        }

        return requiredExtensions.empty();
    }

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return this->graphicsFamily.has_value()
                && this->presentFamily.has_value();
        }
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
        // Code servant à trouver la famille de queue "graphique"
        QueueFamilyIndices indices{};
        uint32_t queueFamilyCount;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies{ queueFamilyCount };
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
        int i = 0;
        for (auto const& queueFamily : queueFamilies) {
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, *this->m_surface, &presentSupport);

            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
            }

            if (presentSupport) {
                indices.presentFamily = i;
            }

            if (indices.isComplete()) {
                break;
            }

            ++i;
        }

        return indices;
    }

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physicalDevice) {
        SwapChainSupportDetails details{};

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, *this->m_surface, &details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, *this->m_surface, &formatCount, nullptr);

        if (formatCount > 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, *this->m_surface, &formatCount, details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, *this->m_surface, &presentModeCount, nullptr);

        if (presentModeCount > 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, *this->m_surface, &presentModeCount, details.presentModes.data());
        }

        return details;
    }

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR> const& availableFormats) {
        auto result = std::find_if(availableFormats.begin(), availableFormats.end(), [] (VkSurfaceFormatKHR format) {
            return format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        });

        if (result == availableFormats.end()) {
            // not found, we use the first one
            return availableFormats[0];
        }
        return *result;
    }

    VkPresentModeKHR chooseSwapPresentMode(std::vector<VkPresentModeKHR> const& availablePresentModes) {
        for (auto const& availablePresentMode : availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            }
        }

        // toujours présent d'après la spec
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D chooseSwapExtent(VkSurfaceCapabilitiesKHR const& capabilities) {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return capabilities.currentExtent;
        }

        VkExtent2D actualExtent = { static_cast<uint32_t>(m_width), static_cast<uint32_t>(m_height) };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }

    void createSwapChain() {
        auto swapChainSupport = querySwapChainSupport(m_physicalDevice);

        auto surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        auto presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
        auto extent = chooseSwapExtent(swapChainSupport.capabilities);
        auto imageCount = swapChainSupport.capabilities.minImageCount + 1;

        if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        auto indices = findQueueFamilies(m_physicalDevice);
        uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = *m_surface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        if (indices.graphicsFamily != indices.presentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        } else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0; // Optionnel
            createInfo.pQueueFamilyIndices = nullptr; // Optionnel
        }
        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;
        if (vkCreateSwapchainKHR(*this->m_device, &createInfo, nullptr, this->m_swapChain.get()) != VK_SUCCESS) {
            throw std::runtime_error("Swapchain creation failed");
        }

        vkGetSwapchainImagesKHR(*this->m_device, *this->m_swapChain, &imageCount, nullptr);
        this->m_swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(*this->m_device, *this->m_swapChain, &imageCount, this->m_swapChainImages.data());
        this->m_swapChainExtent = extent;
        this->m_swapChainImageFormat = surfaceFormat.format;
    }

    void createImageViews() {
        this->m_swapChainImageViews.resize(this->m_swapChainImages.size());

        for (size_t i = 0; i < this->m_swapChainImages.size(); ++i) {
            this->m_swapChainImageViews[i] = std::unique_ptr<VkImageView, std::function<void (VkImageView*)>>(new VkImageView{nullptr}, [&](VkImageView* imageView) { this->releaseSwapChainImageView(*imageView); });
            VkImageViewCreateInfo createInfo{};

            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = this->m_swapChainImages[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = this->m_swapChainImageFormat;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;


            if (vkCreateImageView(*this->m_device, &createInfo, nullptr, this->m_swapChainImageViews[i].get()) != VK_SUCCESS) {
                throw std::runtime_error("échec de la création d'une image view!");
            }
        }
    }

    void pickPhysicalDevice() {
        uint32_t deviceCount {0};

        vkEnumeratePhysicalDevices(*m_instanceVulkan, &deviceCount, nullptr);
        if (0 == deviceCount) {
            throw std::runtime_error("No physical device found supporting Vulkan");
        }

        std::vector<VkPhysicalDevice> devices{deviceCount};
        vkEnumeratePhysicalDevices(*m_instanceVulkan, &deviceCount, devices.data());
        for (auto const& device : devices) {
            if (isDeviceSuitable(device)) {
                m_physicalDevice = device;
                break;
            }
        }

        if (nullptr == m_physicalDevice) {
            throw std::runtime_error("No suitable device found.");
        }
    }

    void createLogicalDevice() {
        auto deviceIndices = findQueueFamilies(this->m_physicalDevice);
        float queuePriority = 1.0f;

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};
        std::set<uint32_t> uniqueQueueFamilies = { deviceIndices.presentFamily.value(), deviceIndices.graphicsFamily.value() };

        for (auto queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{};

            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures physicalDeviceFeatures{};
        VkDeviceCreateInfo  deviceCreateInfo{};

        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
        deviceCreateInfo.queueCreateInfoCount = queueCreateInfos.size();
        deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;

        std::vector<char const*> neededLayers{};
        if (enableValidationLayer) {
            neededLayers.emplace_back("VK_LAYER_KHRONOS_validation");
        }

        deviceCreateInfo.ppEnabledLayerNames = neededLayers.data();
        deviceCreateInfo.enabledLayerCount = neededLayers.size();
        // request extensions
        auto const requiredExtensions = getDeviceExtensions();

        deviceCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();
        deviceCreateInfo.enabledExtensionCount = requiredExtensions.size();

        if (VK_SUCCESS != vkCreateDevice(this->m_physicalDevice, &deviceCreateInfo, nullptr, this->m_device.get())) {
            throw std::runtime_error("Unable to create logical device");
        }

        // get queues references
        vkGetDeviceQueue(*this->m_device, deviceIndices.graphicsFamily.value(), 0, &this->m_graphicsQueue);
        vkGetDeviceQueue(*this->m_device, deviceIndices.presentFamily.value(), 0, &this->m_presentQueue);
    }


    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData) {

        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

        return VK_FALSE;
    }

    static std::vector<char const*> getDeviceExtensions() {
        std::vector<char const*> extensions{
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        return extensions;
    }
};

int main(int, char** )
{
    volkInitialize();
    using namespace std::string_literals;
    // initialisation de la bibliothèque GLFW
    glfwInit();

    // Création de la fenêtre
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    try {
        auto app = TestVulkan(800, 600, "Test vulkan window"s);

        app.initialize();
        app.run();
    } catch (std::exception const& e) {
        std::cout << "An exception occured : " << e.what() << std::endl;
    }


    //GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);

    // Récupération du nombre d'extention supportées
//    uint32_t extensionCount = 0;
//    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
//
//    // On itère sur la liste des extentions
//    auto extentionList = std::vector<VkExtensionProperties>(extensionCount);
//    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extentionList.data());
//
//    std::cout << extensionCount << " extensions supported" << std::endl;
//    for (auto const extension : extentionList) {
//        std::cout << "- " << extension.extensionName << std::endl;
//    }

    // glm::mat4 matrix;
    // glm::vec4 vec;
    // auto test = matrix * vec;

    // Création de l'instance Vulkan
    // informations sur l'application
//    VkApplicationInfo appInfo = {};
//    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
//    appInfo.pApplicationName = "Vulkan test";
//    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
//    appInfo.pEngineName = "No Engine";
//    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
//    appInfo.apiVersion = VK_API_VERSION_1_0;
//
//    // récupération des extensions nécessaires pour fonctionner
//    uint32_t glfwExtensionCount = 0;
//    const char** glfwExtensions;
//
//    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
//
//
//    // Informations sur l'instance Vulkan à créer
//    VkInstanceCreateInfo createInfo = {};
//    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
//    createInfo.pApplicationInfo = &appInfo;
//    createInfo.enabledExtensionCount = glfwExtensionCount;
//    createInfo.ppEnabledExtensionNames = glfwExtensions;
//    createInfo.enabledLayerCount = 0;
//
//    // Création de l'instance Vulkan
//    VkInstance m_instanceVulkan;
//    VkResult creationResult = vkCreateInstance(&createInfo, nullptr, &m_instanceVulkan);
//
//    if (creationResult != VK_SUCCESS) {
//        std::cout << "failed to create instance!" << std::endl;
//        throw std::runtime_error("failed to create instance!");
//    }

    // Boucle de la fenêtre
//    while(!glfwWindowShouldClose(window)) {
//        glfwPollEvents();
//    }

    // Suppression de l'instance Vulkan
//    vkDestroyInstance(m_instanceVulkan, nullptr);
//    // Suppression de la fenêtre
//    glfwDestroyWindow(window);

    // Arrêt propre de GLFW
    glfwTerminate();

    return 0;
}