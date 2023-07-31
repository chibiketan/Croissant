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
#include <fstream>
#include <filesystem>

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
        , m_swapChainFramebuffers{}
        , m_pipelineLayout{ new VkPipelineLayout {nullptr}, [&](VkPipelineLayout* item) { this->releasePipelineLayout(*item); }}
        , m_renderPass{ new VkRenderPass {nullptr}, [&](VkRenderPass* item) { this->releaseRenderPass(*item); }}
        , m_graphicsPipeline{ new VkPipeline {nullptr}, [&](VkPipeline* item) { this->releasePipeline(*item); }}
        , m_commandPool{ new VkCommandPool {nullptr}, [&](VkCommandPool* item) { this->releaseCommandPool(*item); }}
        , m_commandBuffers{}
        , m_imageAvailableSemaphore{ new VkSemaphore{nullptr}, [&](VkSemaphore* item) { this->releaseSemaphore(*item); }}
        , m_renderFinishedSemaphore{ new VkSemaphore{nullptr}, [&](VkSemaphore* item) { this->releaseSemaphore(*item); }}
        , m_inFlightFence{ new VkFence{nullptr}, [&](VkFence* item) { this->releaseFence(*item); }}
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
        this->createRenderPass();
        this->createGraphicsPipeline();
        this->createFramebuffers();
        this->createCommandPool();
        this->createCommandBuffer();
        this->createSyncObjects();
    }

    void run()
    {
        while(!glfwWindowShouldClose(m_window.get())) {
            glfwPollEvents();
            this->drawFrame();
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
    std::vector<std::unique_ptr<VkFramebuffer, std::function<void (VkFramebuffer*)>>> m_swapChainFramebuffers;
    std::unique_ptr<VkPipelineLayout, std::function<void (VkPipelineLayout*)>>  m_pipelineLayout;
    std::unique_ptr<VkRenderPass, std::function<void (VkRenderPass*)>> m_renderPass;
    std::unique_ptr<VkPipeline, std::function<void (VkPipeline*)>> m_graphicsPipeline;
    std::unique_ptr<VkCommandPool, std::function<void (VkCommandPool*)>> m_commandPool;
    std::vector<VkCommandBuffer> m_commandBuffers;
    std::unique_ptr<VkSemaphore, std::function<void (VkSemaphore*)>> m_imageAvailableSemaphore;
    std::unique_ptr<VkSemaphore, std::function<void (VkSemaphore*)>> m_renderFinishedSemaphore;
    std::unique_ptr<VkFence, std::function<void (VkFence*)>> m_inFlightFence;
#ifdef CROISSANT_VULKAN_VALIDATION_LAYER
    constexpr static bool enableValidationLayer = true;
#else
    constexpr static bool enableValidationLayer = false;
#endif

    static std::vector<char> readFile(const std::string& filename) {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            throw std::runtime_error(std::string {"échec de l'ouverture du fichier "} + filename + "!");
        }

        size_t fileSize = (size_t) file.tellg();
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();

        return buffer;
    }

    void createGraphicsPipeline() {
        auto vertShaderCode = readFile("vulkan/shaders/shader.vert.spv");
        auto fragShaderCode = readFile("vulkan/shaders/shader.frag.spv");

        std::unique_ptr<VkShaderModule, std::function<void (VkShaderModule*)>> vertShaderModule = createShaderModule(vertShaderCode);
        std::unique_ptr<VkShaderModule, std::function<void (VkShaderModule*)>> fragShaderModule = createShaderModule(fragShaderCode);


        VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = *vertShaderModule;
        vertShaderStageInfo.pName = "main";


        VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = *fragShaderModule;
        fragShaderStageInfo.pName = "main";


        VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};


        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 0;
        vertexInputInfo.pVertexBindingDescriptions = nullptr; // Optionnel
        vertexInputInfo.vertexAttributeDescriptionCount = 0;
        vertexInputInfo.pVertexAttributeDescriptions = nullptr; // Optionnel


        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float) this->m_swapChainExtent.width;
        viewport.height = (float) this->m_swapChainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;


        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = this->m_swapChainExtent;


        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;
        rasterizer.depthBiasConstantFactor = 0.0f; // Optionnel
        rasterizer.depthBiasClamp = 0.0f; // Optionnel
        rasterizer.depthBiasSlopeFactor = 0.0f; // Optionnel


        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampling.minSampleShading = 1.0f; // Optionnel
        multisampling.pSampleMask = nullptr; // Optionnel
        multisampling.alphaToCoverageEnable = VK_FALSE; // Optionnel
        multisampling.alphaToOneEnable = VK_FALSE; // Optionnel


        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optionnel
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optionnel
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optionnel
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optionnel
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optionnel
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optionnel
        // Exemple avec du blend des couleurs
        //colorBlendAttachment.blendEnable = VK_TRUE;
        //colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        //colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        //colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        //colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        //colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        //colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;


        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optionnel
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f; // Optionnel
        colorBlending.blendConstants[1] = 0.0f; // Optionnel
        colorBlending.blendConstants[2] = 0.0f; // Optionnel
        colorBlending.blendConstants[3] = 0.0f; // Optionnel


        std::vector<VkDynamicState> dynamicStates = {
                VK_DYNAMIC_STATE_VIEWPORT,
                VK_DYNAMIC_STATE_SCISSOR,
                VK_DYNAMIC_STATE_LINE_WIDTH
        };

        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicState.pDynamicStates = dynamicStates.data();

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;            // Optionnel
        pipelineLayoutInfo.pSetLayouts = nullptr;         // Optionnel
        pipelineLayoutInfo.pushConstantRangeCount = 0;    // Optionnel
        pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optionnel

        if (vkCreatePipelineLayout(*this->m_device, &pipelineLayoutInfo, nullptr, this->m_pipelineLayout.get()) != VK_SUCCESS) {
            throw std::runtime_error("échec de la création du pipeline layout!");
        }

        // Liaison de tous les éléments de la pipeline ensemble
        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = nullptr; // optionnel
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = &dynamicState; // optionnel
        pipelineInfo.layout = *this->m_pipelineLayout;
        pipelineInfo.renderPass = *this->m_renderPass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optionnel
        pipelineInfo.basePipelineIndex = -1; // Optionnel

        if (vkCreateGraphicsPipelines(*this->m_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, this->m_graphicsPipeline.get()) != VK_SUCCESS) {
            throw std::runtime_error("échec de la création de la pipeline graphique!");
        }
    }

    void createCommandPool() {
        auto queueFamilyIndices = findQueueFamilies(this->m_physicalDevice);

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; // optionnel

        if (vkCreateCommandPool(*this->m_device, &poolInfo, nullptr, this->m_commandPool.get()) != VK_SUCCESS) {
            throw std::runtime_error("Echec de la création d'une command pool");
        }
    }

    void createCommandBuffer() {
        this->m_commandBuffers.resize(this->m_swapChainFramebuffers.size());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = *this->m_commandPool;
        allocInfo.level = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t)this->m_commandBuffers.size();

        if (vkAllocateCommandBuffers(*this->m_device, &allocInfo, this->m_commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("Echec de l'allocation de command buffers !");
        }
    }

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0; // optional
        beginInfo.pInheritanceInfo = nullptr; // optional

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording comand buffer");
        }

        // starting a render pass
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = *this->m_renderPass;
        renderPassInfo.framebuffer = *this->m_swapChainFramebuffers[imageIndex];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = this->m_swapChainExtent;

        VkClearValue clearColor{{{0.0f, 0.0f, 0.0f, 1.0f}}};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;



        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        // Basic drawing command
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *this->m_graphicsPipeline);
        // define viewport and scissor
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(this->m_swapChainExtent.width);
        viewport.height = static_cast<float>(this->m_swapChainExtent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = this->m_swapChainExtent;
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        // draw triangle
        vkCmdDraw(commandBuffer, 3, 1, 0, 0);

        // ending render pass
        vkCmdEndRenderPass(commandBuffer);

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer");
        }
    }

    void createSyncObjects() {
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        if (vkCreateSemaphore(*this->m_device, &semaphoreInfo, nullptr, this->m_imageAvailableSemaphore.get()) != VK_SUCCESS
            || vkCreateSemaphore(*this->m_device, &semaphoreInfo, nullptr, this->m_renderFinishedSemaphore.get()) != VK_SUCCESS
            || vkCreateFence(*this->m_device, &fenceInfo, nullptr, this->m_inFlightFence.get()) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create semaphore ou fence.");
        }
    }

    void drawFrame() {
        vkWaitForFences(*this->m_device, 1, this->m_inFlightFence.get(), VK_TRUE, std::numeric_limits<uint64_t>::max());
        vkResetFences(*this->m_device, 1, this->m_inFlightFence.get());
        uint32_t imageIndex;
        vkAcquireNextImageKHR(*this->m_device, *this->m_swapChain, std::numeric_limits<uint64_t>::max(), *this->m_imageAvailableSemaphore, nullptr, &imageIndex);
        auto commandBuffer = this->m_commandBuffers.at(imageIndex);
        vkResetCommandBuffer(commandBuffer, 0);
        this->recordCommandBuffer(commandBuffer, imageIndex);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {*this->m_imageAvailableSemaphore};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        VkSemaphore signalSemaphores[] = {*this->m_renderFinishedSemaphore};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(this->m_graphicsQueue, 1, &submitInfo, *this->m_inFlightFence) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer !");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {*this->m_swapChain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr; // Optionnal

        vkQueuePresentKHR(this->m_graphicsQueue, &presentInfo);
    }

    void releaseShaderModule(VkShaderModule shaderModule) {
        if (nullptr == shaderModule) {
            return;
        }

        std::cout << "Release shader module" << std::endl;
        vkDestroyShaderModule(*this->m_device, shaderModule, nullptr);
    }

    void releasePipelineLayout(VkPipelineLayout pipelineLayout) {
        if (nullptr == pipelineLayout) {
            return;
        }

        std::cout << "Release pipeline layout" << std::endl;
        vkDestroyPipelineLayout(*this->m_device, pipelineLayout, nullptr);
    }

    void releaseRenderPass(VkRenderPass renderPass) {
        if (nullptr == renderPass)  {
            return;
        }

        std::cout << "Release render pass" << std::endl;
        vkDestroyRenderPass(*this->m_device, renderPass, nullptr);
    }

    void releasePipeline(VkPipeline pipeline) {
        if (nullptr == pipeline) {
            return;
        }

        std::cout << "Release pipeline" << std::endl;
        vkDestroyPipeline(*this->m_device, pipeline, nullptr);
    }

    void releaseCommandPool(VkCommandPool commandPool) {
        if (nullptr == commandPool) {
            return;
        }

        std::cout << "Release command pool" << std::endl;
        vkDestroyCommandPool(*this->m_device, commandPool, nullptr);
    }

    void releaseSemaphore(VkSemaphore semaphore) {
        if (nullptr == semaphore) {
            return;
        }

        std::cout << "Release semaphore" << std::endl;
        vkDestroySemaphore(*this->m_device, semaphore, nullptr);
    }

    void releaseFence(VkFence fence) {
        if (nullptr == fence) {
            return;
        }

        std::cout << "Release fence" << std::endl;
        vkDestroyFence(*this->m_device, fence, nullptr);
    }

    std::unique_ptr<VkShaderModule, std::function<void (VkShaderModule*)>> createShaderModule(const std::vector<char>& code) {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        std::unique_ptr<VkShaderModule, std::function<void (VkShaderModule*)>> shaderModule { new VkShaderModule{nullptr}, [&](VkShaderModule* item) { this->releaseShaderModule(*item); } };
        if (vkCreateShaderModule(*this->m_device, &createInfo, nullptr, shaderModule.get()) != VK_SUCCESS) {
            throw std::runtime_error("échec de la création d'un module shader!");
        }

        return shaderModule;
    }

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

    void createRenderPass() {
        VkAttachmentDescription colorAttachment{};

        colorAttachment.format = m_swapChainImageFormat;
        colorAttachment.samples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VkImageLayout::VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;


        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;


        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        // add subpass
        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;


        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        if (vkCreateRenderPass(*this->m_device, &renderPassInfo, nullptr, this->m_renderPass.get()) != VK_SUCCESS) {
            throw std::runtime_error("échec de la création de la render pass!");
        }
    }

    void createFramebuffers() {
        this->m_swapChainFramebuffers.resize(this->m_swapChainImageViews.size());

        for (size_t i = 0; i < this->m_swapChainImageViews.size(); ++i) {
            this->m_swapChainFramebuffers[i] = std::unique_ptr<VkFramebuffer, std::function<void(VkFramebuffer*)>>{
                new VkFramebuffer{nullptr}, [&](VkFramebuffer* item) { this->releaseFramebuffer(*item); }
            };

            VkImageView attachments[] = {
                    *this->m_swapChainImageViews[i]
            };
            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = *this->m_renderPass;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = this->m_swapChainExtent.width;
            framebufferInfo.height = this->m_swapChainExtent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(*this->m_device, &framebufferInfo, nullptr, this->m_swapChainFramebuffers[i].get()) != VK_SUCCESS) {
                throw std::runtime_error("Echec de la création d'un framebuffer");
            }
        }
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

    void releaseFramebuffer(VkFramebuffer framebuffer) {
        if (nullptr == framebuffer) {
            return;
        }

        std::cout << "Release framebuffer" << std::endl;
        vkDestroyFramebuffer(*this->m_device, framebuffer, nullptr);
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