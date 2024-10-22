/**
    Declares the Core namespace with all its functions

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         Core.hpp
    @brief        Declaration of the Core namespace
*/
#ifndef VK_ENGINE_HPP
#define VK_ENGINE_HPP
#include "Version.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <thread>
#include <condition_variable>
#include <algorithm>
#include <queue>
#include <mutex>
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
#include "Model.hpp"
#include "Descriptor.hpp"
#include "DescriptorSet.hpp"
#include "ModelInfo.cpp"
#include "Queue.cpp"
#include "AssetLoader.hpp"
#include "LightData.cpp"

namespace vk {

    /**
        Holds the core of the Vulkan render engine 
    */
    namespace core {

        extern VkPhysicalDevice                                 physicalDevice;
        extern VkDevice                                         logicalDevice;
        extern VkAllocationCallbacks*                           allocator;
        extern std::vector< VkImage >                           swapchainImages;
        extern VkFormat                                         swapchainImageFormat;
        extern VkExtent2D                                       swapchainImageExtent;
        extern BaseCamera*                                      camera;
        extern VkSampleCountFlagBits                            MSAASampleCount;
        extern std::queue< ModelInfo >                          modelLoadingQueue;
        extern std::mutex                                       modelLoadingQueueMutex;
        extern std::condition_variable                          modelLoadingQueueCondVar;
        extern bool                                             finished;
        extern bool                                             notified;
        extern std::condition_variable                          assetsLoadedCondVar;
        extern std::mutex                                       assetsLoadedMutex;
        extern bool                                             assetsLoaded;
        extern std::thread                                      assetThread;
        extern bool                                             readyToRun;
        extern GLFWwindow*                                      window;
        extern GLFWmonitor*                                     monitor;
        extern VkInstance                                       instance;
        extern const std::vector< const char* >                 validationLayers;
        extern const bool                                       validationLayersEnabled;
        extern VkDebugUtilsMessengerEXT                         validationLayerDebugMessenger;
        extern VkQueue                                          presentationQueue;
        extern VkSurfaceKHR                                     surface;
        extern const std::vector< const char* >                 requiredExtensions;
        extern LoadingScreen*                                   loadingScreen;
        extern VkSwapchainKHR                                   swapchain;
        extern std::vector< VkImageView >                       swapchainImageViews;
        extern std::vector< VkFramebuffer >                     swapchainFramebuffers;
        extern VkRenderPass                                     renderPass;
        extern GraphicsPipeline                                 standardPipeline;
        extern std::vector< Descriptor >                        standardDescriptors;
        extern DescriptorSetLayout*                             standardDescriptorLayout;
        extern std::vector< VkCommandBuffer >                   standardCommandBuffers;
        extern std::vector< VkSemaphore >                       swapchainImageAvailableSemaphores;
        extern std::vector< VkSemaphore >                       renderingCompletedSemaphores;
        extern std::vector< VkFence >                           inFlightFences;
        extern size_t                                           currentSwapchainImage;
        extern bool                                             hasFramebufferBeenResized;
        extern BaseBuffer*                                      vpBuffer;  
        extern Descriptor                                       vpDescriptor;
        extern BaseBuffer*                                      lightDataBuffer;
        extern Descriptor                                       lightDataDescriptor;
        extern VkPolygonMode                                    polygonMode;
        extern BaseImage*                                       depthBuffer;
#ifndef VK_MULTISAMPLING_NONE
        extern BaseImage*                                       msaaBufferImage;
#endif
        extern TextureImage*                                    noImageSubstituent;
        extern Descriptor                                       noImageSubstituentDescriptor;
        extern Descriptor                                       diffuseSampler1Descriptor;
        extern Descriptor                                       diffuseSampler2Descriptor;
        extern bool                                             initialized;
        extern std::vector< Model* >                            models;
        extern bool                                             firstTimeRecreation;
        extern std::vector< std::vector< DescriptorSet* > >     descriptorSets;
        
        extern std::vector< std::thread* >                      modelLoadingQueueThreads;
        extern std::vector< AssetLoader* >                      assetLoaders;
        extern uint32_t                                         maxThreads;
        
        extern std::vector< std::thread* >                      renderThreads;

        /**
            Pre-runs before init()
        */
        void preInit(void);

        /**
            Initializes VKCore and loads dependencies

            @return     Returns VK_SC_SUCCESS on success
        */
        VK_STATUS_CODE init(void);

        /**
            Runs VKCore

            @return     Returns VK_SC_SUCCESS on success
        */
        VK_STATUS_CODE run(void);

        /**
            Initializes the logger

            @return        Returns LOGGER_SC_SUCCESS on success
            @return        Returns LOGGER_SC_UNKNOWN_ERROR on error
        */
        LOGGER_STATUS_CODE initLogger(void);

        /**
            Initializes the loading screen
        */
        void initLoadingScreen(void);

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
            Adds a model to the model loading queue

            @param      path_                   The path to the model
            @param      modelMatrixFunc_        The function pointer to calculate the models model matrix

            @return     Returns OGL_SC_SUCCESS on success
        */
        VK_STATUS_CODE push(const char* path_, glm::mat4 (*modelMatrixFunc_)());

        /**
            Adds a model to the model loading queue

            @param      info_       A model info struct
        */
        VK_STATUS_CODE push(ModelInfo info_);

        /**
            Finds queue families that are suitable for the operations that are about to be performed on them

            @param         device_        A valid VkPhysicalDevice handle whose queue families are to be tested

            @return        Returns a QueueFamily struct
        */
        QueueFamily findSuitableQueueFamily(VkPhysicalDevice device_);

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
            VkDebugUtilsMessageSeverityFlagBitsEXT              messageSeverity_,
            VkDebugUtilsMessageTypeFlagsEXT                     messageType_,
            const VkDebugUtilsMessengerCallbackDataEXT*         pCallbackData_,
            void*                                               pUserData_
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
            Creates, and allocates command buffers for later submission onto the queues

            @return        Returns VK_SC_SUCCESS on success
        */
        VK_STATUS_CODE allocateCommandBuffers(void);

        /*
            Records a command buffer for rendering

            @param      imageIndex_     The current swapchain image index

            @return     Returns VK_SC_SUCCESS on success
        */
        VK_STATUS_CODE recordCommandBuffer(uint32_t imageIndex_);

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

        /**
            Loads all necessary models and vertex data such as texture coordinates and actual textures

            @return     Returns VK_SC_SUCCESS on success
        */
        VK_STATUS_CODE loadModelsAndVertexData(void);

        /**
            Executes commands for multithreaded image presentation
        */
        void multithreadedNextSwapchainImage(void);

        /**
            Recreates the graphics pipelines

            @return     Returns VK_SC_SUCCESS on success
        */
        VK_STATUS_CODE recreateGraphicsPipelines(void);

    }

}
#endif  // VK_ENGINE_HPPmake
