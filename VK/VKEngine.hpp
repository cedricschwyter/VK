/**
    Declares the VKEngine class

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         VKEngine.hpp
    @brief        Declaration of the VKEngine class
*/
#pragma once
#include "Version.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <thread>
#include <algorithm>
#include <fstream>
#include <string>
#if defined WIN_64 || defined WIN_32
#include <conio.h>
#endif
#include <chrono>

#include "VK_STATUS_CODE.hpp"
#include "Logger.hpp"
#include "QueueFamily.cpp"
#include "LoadingScreen.hpp"
#include "SwapchainDetails.cpp"
#include "VertFragShaderStages.hpp"
#include "GraphicsPipeline.hpp"
#include "BaseVertex.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "UniformBuffer.hpp"
#include "MVPBufferObject.cpp"
#include "TextureImage.hpp"
#include "BaseCamera.hpp"
#include "FPSCamera.hpp"
#include "CenterCamera.hpp"
#include "DepthBuffer.hpp"
#include "MSAARenderImage.hpp"

class VKEngine {
public:

    VkPhysicalDevice                        physicalDevice;
    VkDevice                                logicalDevice;
    VkAllocationCallbacks*                  allocator;
    std::vector< VkImage >                  swapchainImages;
    VkFormat                                swapchainImageFormat;
    VkExtent2D                              swapchainImageExtent;
    VkCommandPool                           standardCommandPool;
    VkQueue                                 graphicsQueue                        = VK_NULL_HANDLE;
    BaseCamera*                             camera;
    VkSampleCountFlagBits                   maxMSAASamples                          = VK_SAMPLE_COUNT_1_BIT;

    /**
        Initializes VKEngine and loads dependencies

        @return        Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE init(void);

    /**
        Finds queue families that are suitable for the operations that are about to be performed on them

        @param         device_        A valid VkPhysicalDevice handle whose queue families are to be tested

        @return        Returns a QueueFamily struct
    */
    QueueFamily findSuitableQueueFamily(VkPhysicalDevice device_);

private:

    GLFWwindow*                             window;
    GLFWmonitor*                            monitor;
    VkInstance                              instance                             = VK_NULL_HANDLE;
    const std::vector< const char* >        validationLayers                     = {
    
        "VK_LAYER_LUNARG_standard_validation"
    
    };
#ifdef VK_DEVELOPMENT
    const bool                              validationLayersEnabled              = true;
#else
    const bool                              validationLayersEnabled              = false;
#endif
    VkDebugUtilsMessengerEXT                validationLayerDebugMessenger        = VK_NULL_HANDLE;
    VkQueue                                 presentationQueue                    = VK_NULL_HANDLE;
    VkSurfaceKHR                            surface                              = VK_NULL_HANDLE;
    const std::vector< const char* >        requiredExtensions                   = {
    
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    
    };
    LoadingScreen*                          loadingScreen                        = nullptr;
    VkSwapchainKHR                          swapchain                            = VK_NULL_HANDLE;
    std::vector< VkImageView >              swapchainImageViews;
    std::vector< VkFramebuffer >            swapchainFramebuffers;
    VkRenderPass                            renderPass;
    GraphicsPipeline                        pipeline;
    std::vector< VkCommandBuffer >          standardCommandBuffers;
    std::vector< VkSemaphore >              swapchainImageAvailableSemaphores;
    std::vector< VkSemaphore >              renderingCompletedSemaphores;
    std::vector< VkFence >                  inFlightFences;
    size_t                                  currentSwapchainImage                = 0;
    bool                                    hasFramebufferBeenResized            = false;
    BaseBuffer*                             vertexBuffer;
    BaseBuffer*                             indexBuffer;
    BaseBuffer*                             mvpBuffer;
    BaseImage*                              depthBuffer;
    BaseImage*                              msaaBufferImage;
    BaseImage*                              image;
    bool                                    initialized                          = false;

    /**
        Initializes the logger

        @return        Returns LOGGER_SC_SUCCESS on success
        @return        Returns LOGGER_SC_UNKNOWN_ERROR on error
    */
    LOGGER_STATUS_CODE initLogger(void);

    /**
        Initializes the windowing library

        @return        Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE initWindow(void);

    /**
        Initializes the Vulkan API

        @return        Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE initVulkan(void);

    /**
        Contains the main loop

        @return        Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE loop(void);

    /**
        Handles cleaning of resources on exit

        @return        Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE clean(void);

    /**
        Creates the VkInstance class member

        @return        Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE createInstance(void);

    /**
        Checks whether validation layers are supported

        @return        Returns true if validation layers are supported
        @return        Returns false if validation layers are not supported
    */
    bool validationLayersSupported(void);

    /**
        Get the required extensions for Vulkan to activate

        @return        Returns an std::vector< const char* > with extension names
    */
    std::vector< const char* > queryRequiredExtensions(void);

    /**
        Message callback function for Vulkan validation layers so they can actually display their messages

        @param         messageSeverity_      Message severity level (internal)
        @param         messageType_          Message type (internal)
        @param         pCallbackData_        Actual message (internal)
        @param         pUserData_            User specified data

        @return        Returns VK_FALSE

    */
    static VKAPI_ATTR VkBool32 VKAPI_CALL validationLayerDebugMessageCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT            messageSeverity_,
        VkDebugUtilsMessageTypeFlagsEXT                    messageType_,
        const VkDebugUtilsMessengerCallbackDataEXT*        pCallbackData_,
        void*                                            pUserData_
        );

    /**
        Creates the VkDebugUtilsMessengerEXT for validation layer messages

        @return        Returns VK_SC_SUCCESS on success
        @return        Returns VK_SC_DEBUG_UTILS_MESSENGER_CREATION_ERROR on error
    */
    VK_STATUS_CODE debugUtilsMessenger(void);

    /**
        Enumerates and selects the best physical device (GPU)

        @return        Returns VK_SC_SUCCESS on success
        @return        Returns VK_SC_PHYSICAL_DEVICE_ERROR on error
    */
    VK_STATUS_CODE selectBestPhysicalDevice(void);

    /**
        Rates GPU for the operations that are about to be performed

        @param         device_        The VkPhysicalDevice to be rated

        @return        Returns an integer value as score
    */
    int evaluateDeviceSuitabilityScore(VkPhysicalDevice device_);

    /**
        Prints information about GPU !!!NOT YET FULLY IMPLEMENTED!!!

        @param         device_        The VkPhysicalDevice to print information about

        @return        Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE printPhysicalDevicePropertiesAndFeatures(VkPhysicalDevice device_);

    /**
        Creates a VkDevice handle from a VkPhysicalDevice (class member)

        @return        Returns VK_SC_SUCCESS on success
        @return        Returns VK_SC_LOGICAL_DEVICE_ERROR on error
    */
    VK_STATUS_CODE createLogicalDeviceFromPhysicalDevice(void);

    /**
        Creates a VkSurfaceKHR handle via GLFW

        @return        Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE createSurfaceGLFW(void);

    /**
        Checks whether the device supports the swapchain extension (should be implicit if it supports Vulkan, but double checking never hurts, right?)
        
        @param         device_        The VkPhysicalDevice handle that should be tested

        @return        Returns true if the device supports the VK_KHR_swapchain extension
        @return        Returns false if the device does not support the VK_KHR_swapchain extensions
    */
    bool checkDeviceSwapchainExtensionSupport(VkPhysicalDevice device_);

    /**
        Initializes the loading screen
    */
    void initLoadingScreen(void);

    /**
        Queries the specified device's swap chain capabilities and details

        @param         device_        The device whose details are to be enumerated

        @return        Returns a SwapchainDetails struct containing the enumerated information
    */
    SwapchainDetails querySwapchainDetails(VkPhysicalDevice device_);

    /**
        Determines the best swapchain-format to use (in the entire application)

        @param         availableFormats_        A reference to an std::vector containing formats to choose from

        @return        Returns the preferred VkSurfaceFormatKHR
    */
    VkSurfaceFormatKHR evaluateBestSwapchainSurfaceFormat(const std::vector< VkSurfaceFormatKHR >& availableFormats_);

    /**
        Determines the best swapchain-present-mode to use

        @param         availablePresentModes_        A reference to an std::vector containing present modes to choose from

        @return        Returns the preferred VkPresentModeKHR
    */
    VkPresentModeKHR evaluateBestSwapchainSurfacePresentMode(const std::vector< VkPresentModeKHR >& availablePresentModes_);

    /**
        Determines the swapchain extent from a set of given surface capabilities

        @param         capabilities_        The supported swapchain-extents of the GPU can be fount in a VkSurfaceCapabilitiesKHR struct

        @return        Returns a VkExtent2D which should be specified in VkSwapchainCreateInfoKHR
    */
    VkExtent2D evaluateSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities_); 

    /**
        Creates a VkSwapchain handle

        @return        Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE createSwapchain(void);

    /**
        Creates an array of VkImageViews for the VkImages in the swapchain

        @return        Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE createSwapchainImageViews(void);

    /**
        Creates the necessary graphics pipelines

        @return        Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE createGraphicsPipelines(void);
    
    /**
        Creates the necessary render pass(es)

        @return        Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE createRenderPasses(void);

    /**
        Creates and allocates a framebuffer for every VkImageView in the swapchain

        @return        Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE allocateSwapchainFramebuffers(void);

    /**
        Creates and allocates command pools for later use

        @return        Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE allocateCommandPools(void);

    /**
        Creates, allocates and recordes command buffers for later submission onto the queues

        @return        Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE allocateCommandBuffers(void);

    /**
        Displays the swapchain image, that is up next to be displayed

        @return        Return VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE showNextSwapchainImage(void);

    /**
        Creates and initializes sync-objects such as semaphores and fences

        @return        Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE initializeSynchronizationObjects(void);

    /**
        Recreates the swapchain on window resize and/or other events

        @return        Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE recreateSwapchain(void);

    /**
        Cleans resources from the swapchain

        @return        Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE cleanSwapchain(void);

    /**
        Framebuffer resize callback function for GLFW

        @param         window_        A pointer to a GLFWwindow
        @param         width_        The new width of the GLFWwindow
        @param         height_        The new height of the GLFWwindow
    */
    static void framebufferResizeCallback(GLFWwindow* window_, int width_, int height_);

    /**
        Allocates the necessary buffers (vertex data, index data, etc.)

        @return        Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE allocateNecessaryBuffers(void);

    /**
        Creates uniform buffers

        @return        Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE allocateUniformBuffers(void);

    /**
        Updates the uniform buffers in the shaders

        @return        Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE updateUniformBuffers();

    /**
        Loads and generates necessary textures

        @return        Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE createTextureImages(void);

    /**
        Creates a camera object

        @return        Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE createCamera(void);

    /**
        Checks for keyboard and mouse input
    */
    void processKeyboardInput(void);

    /**
        Callback function for GLFW on mouse events

        @param         window_     A pointer to a focused GLFWwindow
        @param         xPos_       The x-component of the mouse's position on screen
        @param         yPos_       The y-component of the mouse's position on screen
    */
    static void mouseMoveCallback(GLFWwindow* window_, double xPos_, double yPos_);

    /**
        Callback function for GLFW on scroll events

        @param         xOff_       The x-component of the scroll wheels offset
        @param         yOff_       The y-component of the scroll wheels offset
    */
    static void mouseScrollCallback(GLFWwindow* window_, double xOff_, double yOff_);

    /**
        Generates some depth resources and applies them to the rendered images

        @return     Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE allocateDepthBuffer(void);

    /**
        Evaluates the maximum number of samples to use

        @return     Returns the maximum number of samples as VkSampleCountFlagBits
    */
    VkSampleCountFlagBits enumerateMaximumMultisamplingSampleCount(void);

    /**
        Generates some multisampling resources

        @return     Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE allocateMSAABufferedImage(void);

};
