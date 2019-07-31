/**
    Implements the VKEngine class

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         VKEngine.cpp
    @brief        Implementation of the VKEngine class
*/
#include "VKEngine.hpp"
#include "VK.hpp"
#include "ASSERT.cpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


void VKEngine::init(VK_STATUS_CODE* returnCodeAddr_) {

    logger::log(START_LOG, "Initializing...");
    logger::log(EVENT_LOG, "Initializing loading screen...");
    initLoadingScreen();
    ASSERT(initWindow(), "Window initialization error", VK_SC_WINDOW_ERROR);
    ASSERT(initVulkan(), "Vulkan initialization error", VK_SC_VULKAN_ERROR);
    ASSERT(loop(), "Vulkan runtime error", VK_SC_VULKAN_RUNTIME_ERROR);
    ASSERT(clean(), "Application cleanup error", VK_SC_CLEANUP_ERROR);
    logger::log(START_LOG, "Shutting down...");

    VK_STATUS_CODE* returnCode;
    returnCode          = returnCodeAddr_;
    *returnCode         = vk::errorCodeBuffer;

}

LOGGER_STATUS_CODE VKEngine::initLogger() {

    return logger::init();

}

VK_STATUS_CODE VKEngine::initWindow() {

    logger::log(EVENT_LOG, "Initializing window...");
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

#ifdef VK_WINDOW_MODE_FULLSCREEN
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    window = glfwCreateWindow(
        mode->width,
        mode->height,
        vk::TITLE,
        monitor,
        nullptr
        );
#endif
#if defined VK_WINDOW_MODE_WINDOWED
    window = glfwCreateWindow(
        vk::WIDTH,
        vk::HEIGHT,
        vk::TITLE,
        nullptr,
        nullptr
        );

    glfwSetWindowPos(
        window,
        mode->width / 2 - vk::WIDTH / 2,
        mode->height / 2 - vk::HEIGHT / 2
        );
#endif
#if defined VK_WINDOW_MODE_BORDERLESS || defined VK_WINDOW_MODE_UNDEFINED
    window = glfwCreateWindow(
        mode->width,
        mode->height,
        vk::TITLE,
        monitor,
        nullptr
        );
#endif

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    GLFWimage windowIcon[1];
    windowIcon[0].pixels = stbi_load(
        "res/textures/loading_screen/infinity.jpg",
        &windowIcon[0].width,
        &windowIcon[0].height,
        0,
        STBI_rgb_alpha
        );

    glfwSetWindowIcon(window, 1, windowIcon);
    stbi_image_free(windowIcon[0].pixels);
    logger::log(EVENT_LOG, "Successfully initialized window");

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetWindowUserPointer(window, this);
    logger::log(EVENT_LOG, "Successfully set GLFW window user pointer");

    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    logger::log(EVENT_LOG, "Successfully set framebuffer resize callback");

    glfwSetCursorPosCallback(window, mouseMoveCallback);
    logger::log(EVENT_LOG, "Successfully set cursor position callback");

    glfwSetScrollCallback(window, mouseScrollCallback);
    logger::log(EVENT_LOG, "Successfully set mouse scroll callback");

    return vk::errorCodeBuffer;

}

VK_STATUS_CODE VKEngine::initVulkan() {

    allocator = nullptr;


    ASSERT(createInstance(), "Failed to create instance", VK_SC_INSTANCE_CREATON_ERROR);
    ASSERT(debugUtilsMessenger(), "Failed to create debug utils messenger", VK_SC_DEBUG_UTILS_MESSENGER_CREATION_ERROR);
    ASSERT(createSurfaceGLFW(), "Failed to create GLFW surface", VK_SC_SURFACE_CREATION_ERROR);
    ASSERT(selectBestPhysicalDevice(), "Failed to find a suitable GPU that supports Vulkan", VK_SC_PHYSICAL_DEVICE_CREATION_ERROR);
    ASSERT(createLogicalDeviceFromPhysicalDevice(), "Failed to create a logical device from the selected physical device", VK_SC_LOGICAL_DEVICE_CREATION_ERROR);
    ASSERT(createSwapchain(), "Failed to create a swapchain with the given parameters", VK_SC_SWAPCHAIN_CREATION_ERROR);
    ASSERT(createSwapchainImageViews(), "Failed to create swapchain image views", VK_SC_SWAPCHAIN_IMAGE_VIEWS_CREATION_ERROR);
    ASSERT(initializeSynchronizationObjects(), "Failed to initialize sync-objects", VK_SC_SYNCHRONIZATION_OBJECT_INITIALIZATION_ERROR);
    ASSERT(allocateCommandPools(), "Failed to allocate command pools", VK_SC_COMMAND_POOL_ALLOCATION_ERROR);
    ASSERT(allocateMSAABufferedImage(), "Failed to allocate multisampling-buffer", VK_SC_MSAA_BUFFER_CREATION_ERROR);
    ASSERT(allocateDepthBuffer(), "Failed to allocate depth buffer", VK_SC_DEPTH_BUFFER_CREATION_ERROR);
    ASSERT(createRenderPasses(), "Failed to create render passes", VK_SC_RENDER_PASS_CREATION_ERROR);
    ASSERT(allocateUniformBuffers(), "Failed to allocate uniform buffers", VK_SC_UNIFORM_BUFFER_CREATION_ERROR);
    ASSERT(allocateSwapchainFramebuffers(), "Failed to allocate framebuffers", VK_SC_FRAMEBUFFER_ALLOCATION_ERROR);
    ASSERT(createGraphicsPipelines(), "Failed to create graphics pipelines", VK_SC_GRAPHICS_PIPELINE_CREATION_ERROR);
    ASSERT(loadModelsAndVertexData(), "Failed to load models", VK_SC_RESOURCE_LOADING_ERROR);
    ASSERT(allocateCommandBuffers(), "Failed to allocate command buffers", VK_SC_COMMAND_BUFFER_ALLOCATION_ERROR);
    ASSERT(createCamera(), "Failed to create camera", VK_SC_CAMERA_CREATION_ERROR);

    if (!initialized) {

        std::unique_lock< std::mutex > lock(loadingScreen->closeMutex);
        loadingScreen->close = true;
        loadingScreen->closeMutex.unlock();
        glfwShowWindow(window);
        glfwFocusWindow(window);
        initialized = true;

    }

    return vk::errorCodeBuffer;

}

VK_STATUS_CODE VKEngine::loop() {

    logger::log(EVENT_LOG, "Entering application loop...");

    double                    lastTime          = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {

        static double       pastTime            = 0;
        static float        nbFrames            = 0;
        static float        maxfps              = 0;
        double              currentTime         = glfwGetTime();
        double              deltaTime           = currentTime - pastTime;

        pastTime = currentTime;

        nbFrames++;
        float seconds = 10.0f;

        if (currentTime - lastTime >= 1.0 && nbFrames > maxfps) {

            maxfps = nbFrames;

        }

        if (currentTime - lastTime >= seconds) {

            std::string fps                = "Average FPS (last " + std::to_string(seconds) + " seconds):    %f\t";
            std::string frametime          = "Average Frametime (last " + std::to_string(seconds) + " seconds):    %f ms\t";
            std::string maxFPS             = "Max FPS:    %f\n";

            printf(fps.c_str(), double(nbFrames / seconds));
            printf(frametime.c_str(), double((1000.0 * seconds) / nbFrames));
            printf(maxFPS.c_str(), double(maxfps / seconds));
            nbFrames = 0;
            lastTime += seconds;

        }

        glfwPollEvents();
        processKeyboardInput();
        showNextSwapchainImage();
    
    }

    vkDeviceWaitIdle(logicalDevice);

    logger::log(EVENT_LOG, "Terminating...");

    return vk::errorCodeBuffer;

}

VK_STATUS_CODE VKEngine::clean() {

    ASSERT(cleanSwapchain(), "Failed to clean swapchain", VK_SC_SWAPCHAIN_CLEAN_ERROR);

    for (auto model : models) {
    
        delete model;
        logger::log(EVENT_LOG, "Successfully destroyed model");
    
    }
    logger::log(EVENT_LOG, "Successfully destroyed models");

    for (auto descSet : descriptorSets) {
     
        delete descSet;
        logger::log(EVENT_LOG, "Successfully destroyed descriptor set");

    }
    logger::log(EVENT_LOG, "Successfully destroyed descriptor sets");

    delete camera;
    logger::log(EVENT_LOG, "Successfully destroyed camera");

    for (size_t i = 0; i < vk::MAX_IN_FLIGHT_FRAMES; i++) {

        vkDestroySemaphore(logicalDevice, renderingCompletedSemaphores[i], allocator);
        vkDestroySemaphore(logicalDevice, swapchainImageAvailableSemaphores[i], allocator);
        vkDestroyFence(logicalDevice, inFlightFences[i], allocator);

    }
    vkDestroyFence(logicalDevice, vk::graphicsFence, allocator);
    vkDestroyFence(logicalDevice, vk::transferFence, allocator);
    logger::log(EVENT_LOG, "Successfully destroyed sync-objects");

    std::unique_lock< std::mutex > transferLock(vk::transferMutex);
    vkDestroyCommandPool(logicalDevice, vk::transferCommandPool, allocator);
    transferLock.unlock();
    logger::log(EVENT_LOG, "Successfully destroyed command pool");

    std::unique_lock< std::mutex > graphicsLock(vk::graphicsMutex);
    vkDestroyCommandPool(logicalDevice, vk::graphicsCommandPool, allocator);
    graphicsLock.unlock();
    logger::log(EVENT_LOG, "Successfully destroyed command pool");

    vkDestroyDevice(logicalDevice, allocator);
    logger::log(EVENT_LOG, "Successfully destroyed device");

    if (validationLayersEnabled) {
    
        vk::destroyDebugUtilsMessenger(instance, validationLayerDebugMessenger, allocator);
        logger::log(EVENT_LOG, "Successfully destroyed debug utils messenger");

    }

    vkDestroySurfaceKHR(instance, surface, allocator);
    logger::log(EVENT_LOG, "Successfully destroyed surface");

    vkDestroyInstance(instance, allocator);
    logger::log(EVENT_LOG, "Successfully destroyed instance");

    glfwDestroyWindow(window);
    logger::log(EVENT_LOG, "Successfully destroyed window");

    glfwTerminate();
    logger::log(EVENT_LOG, "Successfully terminated GLFW");

    logger::log(EVENT_LOG, "Successfully cleaned allocated resources, shutting down...");

    return vk::errorCodeBuffer;

}

VK_STATUS_CODE VKEngine::createInstance() {

    logger::log(EVENT_LOG, "Requesting validation layers...");
    if (validationLayersEnabled && !validationLayersSupported()) {
    
        logger::log(ERROR_LOG, "Validation layers are not available");

    }
    logger::log(EVENT_LOG, "Successfully enabled validation layers");

    auto extensions                                    = queryRequiredExtensions();

    VkApplicationInfo applicationInfo                  = {};
    applicationInfo.sType                              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pApplicationName                   = vk::TITLE;
    applicationInfo.applicationVersion                 = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.pEngineName                        = vk::TITLE;
    applicationInfo.engineVersion                      = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.apiVersion                         = VK_API_VERSION_1_0;

    VkInstanceCreateInfo instanceCreateInfo            = {};
    instanceCreateInfo.sType                           = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo                = &applicationInfo;
    instanceCreateInfo.enabledExtensionCount           = static_cast< uint32_t >(extensions.size());
    instanceCreateInfo.ppEnabledExtensionNames         = extensions.data();
    
    if (validationLayersEnabled) {
    
        instanceCreateInfo.enabledLayerCount           = static_cast< uint32_t >(validationLayers.size());
        instanceCreateInfo.ppEnabledLayerNames         = validationLayers.data();

    }
    else {
    
        instanceCreateInfo.enabledLayerCount           = 0;

    }

    logger::log(EVENT_LOG, "Creating VkInstance...");
    VkResult result = vkCreateInstance(&instanceCreateInfo, allocator, &instance);
    ASSERT(result, "Failed to create VkInstance!", VK_SC_INSTANCE_CREATON_ERROR);

    return vk::errorCodeBuffer;

}

bool VKEngine::validationLayersSupported() {

    uint32_t validationLayerCount;
    vkEnumerateInstanceLayerProperties(&validationLayerCount, nullptr);

    std::vector< VkLayerProperties > availableValidationLayers(validationLayerCount);
    vkEnumerateInstanceLayerProperties(&validationLayerCount, availableValidationLayers.data());

    // Check if all layers in validationLayers exist in availableValidationLayers
    for (const char* layer : validationLayers) {
    
        bool found = false;

        for (const auto& layerProp : availableValidationLayers) {
        
            if (strcmp(layer, layerProp.layerName) == 0) {
            
                found = true;
                break;

            }

        }

        if (!found) {
        
            return false;

        }

    }

    return true;

}

std::vector< const char* > VKEngine::queryRequiredExtensions() {
    
    logger::log(EVENT_LOG, "Querying available extensions...");
    uint32_t extCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);

    std::vector< VkExtensionProperties > availableExtensions(extCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extCount, availableExtensions.data());

    std::string exts = "Available extensions:";
    for (const auto& ext : availableExtensions) {

        std::string extName = ext.extensionName;
        exts += "\n\t\t\t\t\t\t\t" + extName;

    }
    logger::log(EVENT_LOG, exts.c_str());

    uint32_t glfwExtCount = 0;
    const char** glfwExt;

    logger::log(EVENT_LOG, "Querying GLFW-extensions...");
    glfwExt = glfwGetRequiredInstanceExtensions(&glfwExtCount);
    logger::log(EVENT_LOG, "Successfully enabled required GLFW-extensions");

    std::vector<const char*> extensions(glfwExt, glfwExt + glfwExtCount);

    if (validationLayersEnabled) {

        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    }

    return extensions;

}

VKAPI_ATTR VkBool32 VKAPI_CALL VKEngine::validationLayerDebugMessageCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity_,
    VkDebugUtilsMessageTypeFlagsEXT                  messageType_,
    const VkDebugUtilsMessengerCallbackDataEXT*      pCallbackData_,
    void*                                            pUserData_
    ) {

    std::string header = "Validation Layer:    ";
    std::string message(pCallbackData_->pMessage);

    if (messageSeverity_ > VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
    
        logger::log(ERROR_LOG, header + message);

    }
    else {

        logger::log(EVENT_LOG, header + message);

    }

    return VK_FALSE;

}

VK_STATUS_CODE VKEngine::debugUtilsMessenger() {

    if (!validationLayersEnabled) return vk::errorCodeBuffer;

    logger::log(EVENT_LOG, "Creating debug utils messenger...");

    VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo        = {};
    debugUtilsMessengerCreateInfo.sType                                     = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugUtilsMessengerCreateInfo.messageSeverity                           = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
                                                                            | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                                                                            | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
                                                                            | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
    debugUtilsMessengerCreateInfo.messageType                               = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                                                                            | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                                                                            | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debugUtilsMessengerCreateInfo.pfnUserCallback                           = validationLayerDebugMessageCallback;
    debugUtilsMessengerCreateInfo.pUserData                                 = nullptr;

    VkResult result = vk::createDebugUtilsMessenger(
        instance,
        &debugUtilsMessengerCreateInfo,
        allocator,
        &validationLayerDebugMessenger);

    ASSERT(result, "Debug utils messenger creation error", VK_SC_DEBUG_UTILS_MESSENGER_CREATION_ERROR);

    logger::log(EVENT_LOG, "Successfully created debug utils messenger");

    return vk::errorCodeBuffer;

}

VK_STATUS_CODE VKEngine::selectBestPhysicalDevice() {

    logger::log(EVENT_LOG, "Enumerating GPUs...");
    uint32_t physicalDeviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);

    if (physicalDeviceCount == 0) {
    
        logger::log(ERROR_LOG, "Failed to find a suitable GPU");
        return VK_SC_PHYSICAL_DEVICE_CREATION_ERROR;
    
    }

    std::vector< VkPhysicalDevice > physicalDevices(physicalDeviceCount);
    vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());

    std::multimap< int, VkPhysicalDevice > possibleGPUs;

    for (const auto& physicalDev : physicalDevices) {
    
        printPhysicalDevicePropertiesAndFeatures(physicalDev);

        int rating = evaluateDeviceSuitabilityScore(physicalDev);

        possibleGPUs.insert(std::make_pair(rating, physicalDev));
    
    }

    if (possibleGPUs.rbegin()->first > 0) {        // Is the first possibility even suitable?

        logger::log(EVENT_LOG, "Suitable GPU found: ");
        printPhysicalDevicePropertiesAndFeatures(possibleGPUs.rbegin()->second);
        physicalDevice = possibleGPUs.rbegin()->second;
        MSAASampleCount = enumerateMaximumMultisamplingSampleCount();
        std::string msaaSamplesString;
        if (MSAASampleCount == VK_SAMPLE_COUNT_64_BIT) { msaaSamplesString = "VK_SAMPLE_COUNT_64_BIT"; }
        if (MSAASampleCount == VK_SAMPLE_COUNT_32_BIT) { msaaSamplesString = "VK_SAMPLE_COUNT_32_BIT"; }
        if (MSAASampleCount == VK_SAMPLE_COUNT_16_BIT) { msaaSamplesString = "VK_SAMPLE_COUNT_16_BIT"; }
        if (MSAASampleCount == VK_SAMPLE_COUNT_8_BIT) { msaaSamplesString = "VK_SAMPLE_COUNT_8_BIT"; }
        if (MSAASampleCount == VK_SAMPLE_COUNT_4_BIT) { msaaSamplesString = "VK_SAMPLE_COUNT_4_BIT"; }
        if (MSAASampleCount == VK_SAMPLE_COUNT_2_BIT) { msaaSamplesString = "VK_SAMPLE_COUNT_2_BIT"; }
        if (MSAASampleCount == VK_SAMPLE_COUNT_1_BIT) { msaaSamplesString = "VK_SAMPLE_COUNT_1_BIT"; }
        logger::log(EVENT_LOG, "Enumerated maximum multisampling count: " + msaaSamplesString);

#ifdef VK_MULTISAMPLING_NONE
        MSAASampleCount = VK_SAMPLE_COUNT_1_BIT;
#elif defined VK_MULTISAMPLING_x2
        MSAASampleCount = VK_SAMPLE_COUNT_2_BIT;
#elif defined VK_MULTISAMPLING_x4
        MSAASampleCount = VK_SAMPLE_COUNT_4_BIT;
#elif defined VK_MULTISAMPLING_x8
        MSAASampleCount = VK_SAMPLE_COUNT_8_BIT;
#elif defined VK_MULTISAMPLING_x16
        MSAASampleCount = VK_SAMPLE_COUNT_16_BIT;
#elif defined VK_MULTISAMPLING_x32
        MSAASampleCount = VK_SAMPLE_COUNT_32_BIT;
#elif defined VK_MULTISAMPLING_x64
        MSAASampleCount = VK_SAMPLE_COUNT_64_BIT;
#endif

        return vk::errorCodeBuffer;

    }
    else {
    
        logger::log(ERROR_LOG, "Failed to find a suitable GPU that supports Vulkan");
        return VK_SC_PHYSICAL_DEVICE_VULKAN_SUPPORT_ERROR;

    }

}

int VKEngine::evaluateDeviceSuitabilityScore(VkPhysicalDevice device_) {

    QueueFamily                family                = findSuitableQueueFamily(device_);
    SwapchainDetails           swapchainDetails      = querySwapchainDetails(device_);

    VkPhysicalDeviceProperties physicalDeviceProperties;
    vkGetPhysicalDeviceProperties(device_, &physicalDeviceProperties);

    VkPhysicalDeviceFeatures physicalDeviceFeatures;
    vkGetPhysicalDeviceFeatures(device_, &physicalDeviceFeatures);

    int score = 0;

    if (physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {

        score += 1000;

    }

    if (!family.isComplete() 
        || !checkDeviceSwapchainExtensionSupport(device_) 
        || swapchainDetails.supportedFormats.empty() 
        || swapchainDetails.presentationModes.empty()
        || !physicalDeviceFeatures.samplerAnisotropy
        ) {        // absolutely necessary features needed to run application on that GPU
    
        return 0;

    }

    return score;

}

VK_STATUS_CODE VKEngine::printPhysicalDevicePropertiesAndFeatures(VkPhysicalDevice device_) {

    VkPhysicalDeviceProperties physicalDeviceProperties;
    vkGetPhysicalDeviceProperties(device_, &physicalDeviceProperties);

    VkPhysicalDeviceFeatures physicalDeviceFeatures;
    vkGetPhysicalDeviceFeatures(device_, &physicalDeviceFeatures);

    // TODO: Print info
    std::string info = std::string("GPU information:    ") + physicalDeviceProperties.deviceName;

    logger::log(EVENT_LOG, info);

    return vk::errorCodeBuffer;

}

QueueFamily VKEngine::findSuitableQueueFamily(VkPhysicalDevice device_) {

    QueueFamily family;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device_, &queueFamilyCount, nullptr);

    std::vector< VkQueueFamilyProperties > queueFamily(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device_, &queueFamilyCount, queueFamily.data());

    int i = 0;
    for (const auto& qF : queueFamily) {
    
        if (qF.queueCount > 0 && (qF.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {        // Does the queue family have at least one queue and does it support graphics-operations?
        
            family.graphicsFamilyIndex = i;

        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(
            device_,
            i, 
            surface,
            &presentSupport
            );

        if (qF.queueCount > 0 && presentSupport) {        // Also a presentation queue family is needed to actually display to the surface

            family.presentationFamilyIndex = i;
        
        }

        if (qF.queueCount > 0 && (qF.queueFlags & VK_QUEUE_TRANSFER_BIT) && !(qF.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {        // Transfer queue for memory operations
        
            family.transferFamilyIndex = i;
        
        }


        if (family.isComplete()) {
        
            break;

        }

        i++;

    }

    return family;

}

VK_STATUS_CODE VKEngine::createLogicalDeviceFromPhysicalDevice() {

    logger::log(EVENT_LOG, "Creating logical device...");
    QueueFamily family = findSuitableQueueFamily(physicalDevice);

    std::vector< VkDeviceQueueCreateInfo > deviceQueueCreateInfos;
    std::set< uint32_t > uniqueQueueFamilies = { family.graphicsFamilyIndex.value(), family.presentationFamilyIndex.value(), family.transferFamilyIndex.value() };

    float queuePriority = 1.0f;
    for (uint32_t qF : uniqueQueueFamilies) {

        VkDeviceQueueCreateInfo deviceQueueCreateInfo          = {};
        deviceQueueCreateInfo.sType                            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        deviceQueueCreateInfo.queueFamilyIndex                 = qF;
        deviceQueueCreateInfo.queueCount                       = 1;
        deviceQueueCreateInfo.pQueuePriorities                 = &queuePriority;

        deviceQueueCreateInfos.push_back(deviceQueueCreateInfo);

    }

    VkPhysicalDeviceFeatures physicalDeviceFeatures            = {};
    physicalDeviceFeatures.samplerAnisotropy                   = VK_TRUE;
    physicalDeviceFeatures.fillModeNonSolid                    = VK_TRUE;

    VkDeviceCreateInfo deviceCreateInfo                        = {};
    deviceCreateInfo.sType                                     = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount                      = static_cast< uint32_t >(deviceQueueCreateInfos.size());
    deviceCreateInfo.pQueueCreateInfos                         = deviceQueueCreateInfos.data();
    deviceCreateInfo.enabledExtensionCount                     = static_cast< uint32_t >(requiredExtensions.size());
    deviceCreateInfo.ppEnabledExtensionNames                   = requiredExtensions.data();
    deviceCreateInfo.pEnabledFeatures                          = &physicalDeviceFeatures;

    if (validationLayersEnabled) {

        deviceCreateInfo.enabledLayerCount = static_cast< uint32_t >(validationLayers.size());
        deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();

    }
    else {

        deviceCreateInfo.enabledLayerCount = 0;

    }

    VkResult result = vkCreateDevice(
        physicalDevice, 
        &deviceCreateInfo, 
        allocator, 
        &logicalDevice
        );
    ASSERT(result, "Failed to create a logical device", VK_SC_LOGICAL_DEVICE_CREATION_ERROR);
    logger::log(EVENT_LOG, "Successfully created logical device");

    logger::log(EVENT_LOG, "Retrieving queue handle for graphics queue...");
    vkGetDeviceQueue(
        logicalDevice, 
        family.graphicsFamilyIndex.value(), 
        0, 
        &vk::graphicsQueue
        );
    logger::log(EVENT_LOG, "Successfully retrieved queue handle for graphics queue");

    logger::log(EVENT_LOG, "Retrieving queue handle for presentation queue...");
    vkGetDeviceQueue(
        logicalDevice,
        family.presentationFamilyIndex.value(),
        0,
        &presentationQueue
        );
    logger::log(EVENT_LOG, "Successfully retrieved queue handle for presentation queue");

    logger::log(EVENT_LOG, "Retrieving queue handle for transfer queue...");
    vkGetDeviceQueue(
        logicalDevice,
        family.transferFamilyIndex.value(),
        0,
        &vk::transferQueue
        );
    logger::log(EVENT_LOG, "Successfully retrieved queue handle for transfer queue");

    return vk::errorCodeBuffer;

}

VK_STATUS_CODE VKEngine::createSurfaceGLFW() {

    ASSERT(glfwCreateWindowSurface(
        instance, 
        window, 
        allocator, 
        &surface
        ), "GLFW surface creation error", VK_SC_SURFACE_CREATION_ERROR);

    return vk::errorCodeBuffer;

}

bool VKEngine::checkDeviceSwapchainExtensionSupport(VkPhysicalDevice device_) {

    logger::log(EVENT_LOG, "Checking, whether the device supports the necessary extensions...");

    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(
        device_,
        nullptr,
        &extensionCount,
        nullptr
        );

    std::vector< VkExtensionProperties > availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(
        device_,
        nullptr,
        &extensionCount, 
        availableExtensions.data()
        );

    // Check whether all required extensions are amongst the available ones
    std::set< std::string > extensions(requiredExtensions.begin(), requiredExtensions.end());

    for (const auto& extension : availableExtensions) {

        extensions.erase(extension.extensionName);
    
    }
    
    logger::log(extensions.empty() ? EVENT_LOG : ERROR_LOG, extensions.empty() ? "Device supports necessary extensions" : "Device does not support necessary extensions");        // An if-statement would probably have done better, but I mean, if you can do it in style, why not? ;)

    return extensions.empty();

}

void VKEngine::initLoadingScreen() {

    loadingScreen = new LoadingScreen();

    logger::log(EVENT_LOG, "Starting loading screen thread...");
    std::thread t0([=]() {

        loadingScreen->loop();
        logger::log(EVENT_LOG, "Stopping loading screen thread...");

    });
    t0.detach();

}

SwapchainDetails VKEngine::querySwapchainDetails(VkPhysicalDevice device_) {

    logger::log(EVENT_LOG, "Gathering swapchain details...");
    SwapchainDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device_, surface, &details.surfaceCapabilities);

    uint32_t numFormats;
    vkGetPhysicalDeviceSurfaceFormatsKHR(
        device_, 
        surface, 
        &numFormats, 
        nullptr
        );

    if (numFormats != 0) {
    
        details.supportedFormats.resize(numFormats);
        vkGetPhysicalDeviceSurfaceFormatsKHR(
            device_,
            surface,
            &numFormats,
            details.supportedFormats.data()
            );

    }

    uint32_t numPresentModes;
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        device_,
        surface,
        &numPresentModes,
        nullptr
        );

    if (numPresentModes != 0) {
    
        details.presentationModes.resize(numPresentModes);
        vkGetPhysicalDeviceSurfacePresentModesKHR(
            device_,
            surface,
            &numPresentModes,
            details.presentationModes.data()
            );
    
    }
    logger::log(EVENT_LOG, "Successfully gathered swapchain details");

    return details;

}

VkSurfaceFormatKHR VKEngine::evaluateBestSwapchainSurfaceFormat(const std::vector< VkSurfaceFormatKHR >& availableFormats_) {

    if (availableFormats_.size() == 1 && availableFormats_[0].format == VK_FORMAT_UNDEFINED) {        // If Vulkan only returns one entry and its format is set to VK_FORMAT_UNDEFINED it means that Vulkan has no preferred format so we can choose freely

        return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };

    }

    for (const auto& format : availableFormats_) {

        if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
        
            return format;
        
        }

    }

    return availableFormats_[0];        // If there are no better choices, just return the first one in the array, usually it is good enough

}

VkPresentModeKHR VKEngine::evaluateBestSwapchainSurfacePresentMode(const std::vector< VkPresentModeKHR >& availablePresentModes_) {

    for (const auto& presMode : availablePresentModes_) {

        if (presMode == VK_PRESENT_MODE_MAILBOX_KHR) {        // VK_PRESENT_MODE_MAILBOX_KHR MAY cause tearing issues in borderless window or fullscreen mode

            return presMode;

        }

    }

    return VK_PRESENT_MODE_FIFO_KHR;        // If no better mode is available, return VK_PRESENT_MODE_FIFO_KHR as its implied to be supported if the GPU support Vulkan

}

VkExtent2D VKEngine::evaluateSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities_) {

    if (capabilities_.currentExtent.width != std::numeric_limits< uint32_t >::max()) {

        return capabilities_.currentExtent;

    }
    else {

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        VkExtent2D preferredExtent = {

            static_cast< uint32_t >(width),
            static_cast< uint32_t >(height)

        };

        preferredExtent.width             = std::clamp(preferredExtent.width, capabilities_.minImageExtent.width, capabilities_.maxImageExtent.width);
        preferredExtent.height            = std::clamp(preferredExtent.height, capabilities_.minImageExtent.height, capabilities_.maxImageExtent.height);

        return preferredExtent;
    }

}

VK_STATUS_CODE VKEngine::createSwapchain() {

    logger::log(EVENT_LOG, "Creating swapchain...");
    SwapchainDetails          details                        = querySwapchainDetails(physicalDevice);

    VkSurfaceFormatKHR        surfaceFormat                  = evaluateBestSwapchainSurfaceFormat(details.supportedFormats);
    VkPresentModeKHR          presentMode                    = evaluateBestSwapchainSurfacePresentMode(details.presentationModes);
    VkExtent2D                extent                         = evaluateSwapchainExtent(details.surfaceCapabilities);

    swapchainImageFormat                                     = surfaceFormat.format;
    swapchainImageExtent                                     = extent;

    uint32_t                  swapchainImageCount            = details.surfaceCapabilities.minImageCount + 1;
    if (details.surfaceCapabilities.maxImageCount > 0 && swapchainImageCount > details.surfaceCapabilities.maxImageCount) {
    
        swapchainImageCount = details.surfaceCapabilities.maxImageCount;
    
    }

    VkSwapchainCreateInfoKHR swapchainCreateInfo             = {};
    swapchainCreateInfo.sType                                = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCreateInfo.surface                              = surface;
    swapchainCreateInfo.minImageCount                        = swapchainImageCount;
    swapchainCreateInfo.imageFormat                          = surfaceFormat.format;
    swapchainCreateInfo.imageColorSpace                      = surfaceFormat.colorSpace;
    swapchainCreateInfo.imageExtent                          = extent;
    swapchainCreateInfo.imageArrayLayers                     = 1;                                          // Amount of layers in an image, always 1, unless doing stereoscopic stuff
    swapchainCreateInfo.imageUsage                           = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;        // Render directly to swapchain

    QueueFamily family                                       = findSuitableQueueFamily(physicalDevice);
    uint32_t queueFamilyIndices[]                            = { family.graphicsFamilyIndex.value(), family.presentationFamilyIndex.value() };

    if (family.graphicsFamilyIndex != family.presentationFamilyIndex) {           // If presentation queue and graphics queue are in the same queue family, exclusive ownership is not necessary

        swapchainCreateInfo.imageSharingMode                 = VK_SHARING_MODE_CONCURRENT;
        swapchainCreateInfo.queueFamilyIndexCount            = 2;
        swapchainCreateInfo.pQueueFamilyIndices              = queueFamilyIndices;

    }
    else {                                                                        // else it is

        swapchainCreateInfo.imageSharingMode                 = VK_SHARING_MODE_EXCLUSIVE;

    }
    
    swapchainCreateInfo.preTransform                         = details.surfaceCapabilities.currentTransform;        // Specify current image transform as no transform is necessary
    swapchainCreateInfo.compositeAlpha                       = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;                   // Window should not be blended with other windows behind it, no thanks
    swapchainCreateInfo.clipped                              = true;                                                // Do not render pixels that are outside the clip space as this is just performance loss
    swapchainCreateInfo.presentMode                          = presentMode;
    swapchainCreateInfo.oldSwapchain                         = VK_NULL_HANDLE;

    VkResult result = vkCreateSwapchainKHR(
        logicalDevice,
        &swapchainCreateInfo,
        allocator,
        &swapchain
        );
    ASSERT(result, "Swapchain creation error", VK_SC_SWAPCHAIN_CREATION_ERROR);
    logger::log(EVENT_LOG, "Successfully created swapchain");

    logger::log(EVENT_LOG, "Retrieving the handles for the swapchain images...");
    vkGetSwapchainImagesKHR(
        logicalDevice, 
        swapchain,
        &swapchainImageCount,
        nullptr
        );
    swapchainImages.resize(swapchainImageCount);
    vkGetSwapchainImagesKHR(
        logicalDevice,
        swapchain,
        &swapchainImageCount,
        swapchainImages.data()
        );
    logger::log(EVENT_LOG, "Successfully retrieved the handles for the swapchain images");

    return vk::errorCodeBuffer;

}

VK_STATUS_CODE VKEngine::createSwapchainImageViews() {

    logger::log(EVENT_LOG, "Creating swapchain image views...");

    swapchainImageViews.resize(swapchainImages.size());

    for (size_t i = 0; i < swapchainImages.size(); i++) {
    
        swapchainImageViews[i] = vk::createImageView(
            swapchainImages[i], 
            swapchainImageFormat, 
            VK_IMAGE_ASPECT_COLOR_BIT,
            1
            );

    }

    logger::log(EVENT_LOG, "Successfully created swapchain image views");

    return vk::errorCodeBuffer;

}

VK_STATUS_CODE VKEngine::createGraphicsPipelines() {

    logger::log(EVENT_LOG, "Creating graphics pipeline...");
    
    auto bindingDesc            = BaseVertex::getBindingDescription();
    auto attribDesc             = BaseVertex::getAttributeDescriptions();

    VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo                 = {};
    vertexInputStateCreateInfo.sType                                                = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputStateCreateInfo.vertexBindingDescriptionCount                        = 1;
    vertexInputStateCreateInfo.pVertexBindingDescriptions                           = &bindingDesc;
    vertexInputStateCreateInfo.vertexAttributeDescriptionCount                      = static_cast< uint32_t >(attribDesc.size());
    vertexInputStateCreateInfo.pVertexAttributeDescriptions                         = attribDesc.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo             = {};
    inputAssemblyStateCreateInfo.sType                                              = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyStateCreateInfo.topology                                           = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;        // Draw triangles
    inputAssemblyStateCreateInfo.primitiveRestartEnable                             = VK_FALSE;

    VkViewport viewport                                                             = {};
    viewport.x                                                                      = 0.0f;
    viewport.y                                                                      = 0.0f;
    viewport.width                                                                  = static_cast< float >(swapchainImageExtent.width);
    viewport.height                                                                 = static_cast< float >(swapchainImageExtent.height);
    viewport.minDepth                                                               = 0.0f;
    viewport.maxDepth                                                               = 1.0f;

    VkRect2D scissors                                                               = {};
    scissors.offset                                                                 = {0, 0};
    scissors.extent                                                                 = swapchainImageExtent;

    VkPipelineViewportStateCreateInfo viewportStateCreateInfo                       = {};
    viewportStateCreateInfo.sType                                                   = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportStateCreateInfo.viewportCount                                           = 1;
    viewportStateCreateInfo.pViewports                                              = &viewport;
    viewportStateCreateInfo.scissorCount                                            = 1;
    viewportStateCreateInfo.pScissors                                               = &scissors;

    VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo             = {};
    rasterizationStateCreateInfo.sType                                              = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizationStateCreateInfo.depthClampEnable                                   = VK_FALSE;
    rasterizationStateCreateInfo.rasterizerDiscardEnable                            = VK_FALSE;
    rasterizationStateCreateInfo.polygonMode                                        = polygonMode;
    rasterizationStateCreateInfo.lineWidth                                          = 1.0f;
    rasterizationStateCreateInfo.cullMode                                           = VK_CULL_MODE_BACK_BIT;
    rasterizationStateCreateInfo.frontFace                                          = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizationStateCreateInfo.depthBiasEnable                                    = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo                 = {};
    multisampleStateCreateInfo.sType                                                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleStateCreateInfo.sampleShadingEnable                                  = VK_FALSE;
    multisampleStateCreateInfo.rasterizationSamples                                 = MSAASampleCount;
                                                                                    
    VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo               = {};
    depthStencilStateCreateInfo.sType                                               = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencilStateCreateInfo.depthTestEnable                                     = VK_TRUE;
    depthStencilStateCreateInfo.depthWriteEnable                                    = VK_TRUE;
    depthStencilStateCreateInfo.depthCompareOp                                      = VK_COMPARE_OP_LESS;
    depthStencilStateCreateInfo.depthBoundsTestEnable                               = VK_FALSE;
    depthStencilStateCreateInfo.stencilTestEnable                                   = VK_FALSE;

    VkPipelineColorBlendAttachmentState colorBlendAttachmentState                   = {};
    colorBlendAttachmentState.colorWriteMask                                        = VK_COLOR_COMPONENT_R_BIT
                                                                                    | VK_COLOR_COMPONENT_G_BIT
                                                                                    | VK_COLOR_COMPONENT_B_BIT
                                                                                    | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachmentState.blendEnable                                           = VK_TRUE;        // Implement alpha-blending
    colorBlendAttachmentState.srcColorBlendFactor                                   = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachmentState.dstColorBlendFactor                                   = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachmentState.colorBlendOp                                          = VK_BLEND_OP_ADD;
    colorBlendAttachmentState.srcAlphaBlendFactor                                   = VK_BLEND_FACTOR_ONE;
    colorBlendAttachmentState.dstAlphaBlendFactor                                   = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachmentState.alphaBlendOp                                          = VK_BLEND_OP_ADD;
                                                                                    
    VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo                   = {};
    colorBlendStateCreateInfo.sType                                                 = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendStateCreateInfo.logicOpEnable                                         = VK_FALSE;
    colorBlendStateCreateInfo.attachmentCount                                       = 1;
    colorBlendStateCreateInfo.pAttachments                                          = &colorBlendAttachmentState;
                                                                                    
    std::vector< VkDynamicState > dynamicStates                                     = {
    
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR

    };

    // Dynamic states require GPU features that I do not want to activate just now, so I'll define this struct here but not reference it in the pipeline create info
    VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo                         = {};
    dynamicStateCreateInfo.sType                                                    = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicStateCreateInfo.dynamicStateCount                                        = static_cast< uint32_t >(dynamicStates.size());
    dynamicStateCreateInfo.pDynamicStates                                           = dynamicStates.data();
                             
    /* UNIFORM BINDINGS */    

    VkDescriptorBufferInfo mvpBufferInfo                                            = {};
    mvpBufferInfo.buffer                                                            = mvpBuffer->buf;
    mvpBufferInfo.offset                                                            = 0;
    mvpBufferInfo.range                                                             = sizeof(MVPBufferObject);
                                                                                    
    UniformInfo mvpInfo                                                             = {};
    mvpInfo.binding                                                                 = 0;
    mvpInfo.stageFlags                                                              = VK_SHADER_STAGE_VERTEX_BIT;
    mvpInfo.bufferInfo                                                              = mvpBufferInfo;
    mvpInfo.type                                                                    = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    
    mvpDescriptor                                                                   = Descriptor(mvpInfo);
                                                                                    
    UniformInfo diffuseSamplerInfo                                                  = {};
    diffuseSamplerInfo.binding                                                      = 1;
    diffuseSamplerInfo.stageFlags                                                   = VK_SHADER_STAGE_FRAGMENT_BIT;
    diffuseSamplerInfo.type                                                         = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
               
    diffuseSamplerDescriptor                                                        = Descriptor(diffuseSamplerInfo);

    standardDescriptors.push_back(mvpDescriptor);
    standardDescriptors.push_back(diffuseSamplerDescriptor);

    standardDescriptorLayout = new DescriptorSetLayout(standardDescriptors);

    standardPipeline = GraphicsPipeline(
        "shaders/standard/vert.spv", 
        "shaders/standard/frag.spv",
        &vertexInputStateCreateInfo,
        &inputAssemblyStateCreateInfo,
        &viewportStateCreateInfo,
        &rasterizationStateCreateInfo,
        &multisampleStateCreateInfo,
        &depthStencilStateCreateInfo,
        &colorBlendAttachmentState,
        &colorBlendStateCreateInfo,
        nullptr,                        // Defined, but not referenced
        standardDescriptorLayout,
        renderPass
        );

    logger::log(EVENT_LOG, "Successfully created graphics pipeline");

    return vk::errorCodeBuffer;

}

VK_STATUS_CODE VKEngine::createRenderPasses() {

    logger::log(EVENT_LOG, "Creating render passes...");

    VkAttachmentDescription colorAttachmentDescription          = {};
    colorAttachmentDescription.format                           = swapchainImageFormat;
    colorAttachmentDescription.samples                          = MSAASampleCount; 
    colorAttachmentDescription.loadOp                           = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachmentDescription.storeOp                          = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachmentDescription.stencilLoadOp                    = VK_ATTACHMENT_LOAD_OP_DONT_CARE;                  // No stencil buffering, so nobody cares about stencil operations
    colorAttachmentDescription.stencilStoreOp                   = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachmentDescription.initialLayout                    = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachmentDescription.finalLayout                      = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;         // Render the image to the offscreen render-target
#ifdef VK_MULTISAMPLING_NONE
    colorAttachmentDescription.finalLayout                      = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
#endif
    VkAttachmentReference colorAttachmentReference              = {};
    colorAttachmentReference.attachment                         = 0; 
    colorAttachmentReference.layout                             = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription depthBufferAttachmentDescription    = {};
    depthBufferAttachmentDescription.format                     = depthBuffer->imgFormat;
    depthBufferAttachmentDescription.samples                    = MSAASampleCount;
    depthBufferAttachmentDescription.loadOp                     = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthBufferAttachmentDescription.storeOp                    = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthBufferAttachmentDescription.stencilLoadOp              = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthBufferAttachmentDescription.stencilStoreOp             = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthBufferAttachmentDescription.initialLayout              = VK_IMAGE_LAYOUT_UNDEFINED;
    depthBufferAttachmentDescription.finalLayout                = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthBufferAttachmentReference        = {};
    depthBufferAttachmentReference.attachment                   = 1;
    depthBufferAttachmentReference.layout                       = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
#ifndef VK_MULTISAMPLING_NONE
    VkAttachmentDescription colorAttachmentResolve              = {};
    colorAttachmentResolve.format                               = swapchainImageFormat;
    colorAttachmentResolve.samples                              = VK_SAMPLE_COUNT_1_BIT;     // Sample multisampled color attachment down to present to screen
    colorAttachmentResolve.loadOp                               = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachmentResolve.storeOp                              = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachmentResolve.stencilLoadOp                        = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachmentResolve.stencilStoreOp                       = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachmentResolve.initialLayout                        = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachmentResolve.finalLayout                          = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentResolveRef             = {};
    colorAttachmentResolveRef.attachment                        = 2;
    colorAttachmentResolveRef.layout                            = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
#endif
    VkSubpassDescription subpassDescription                     = {};
    subpassDescription.pipelineBindPoint                        = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription.colorAttachmentCount                     = 1;
    subpassDescription.pColorAttachments                        = &colorAttachmentReference;
    subpassDescription.pDepthStencilAttachment                  = &depthBufferAttachmentReference;
#ifndef VK_MULTISAMPLING_NONE
    subpassDescription.pResolveAttachments                      = &colorAttachmentResolveRef;
#endif
    std::vector< VkAttachmentDescription > attachments          = { colorAttachmentDescription, depthBufferAttachmentDescription, 
#ifndef VK_MULTISAMPLING_NONE 
        colorAttachmentResolve 
#endif
    };
    VkRenderPassCreateInfo renderPassCreateInfo                 = {};
    renderPassCreateInfo.sType                                  = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.attachmentCount                        = static_cast< uint32_t >(attachments.size());
    renderPassCreateInfo.pAttachments                           = attachments.data();
    renderPassCreateInfo.subpassCount                           = 1;
    renderPassCreateInfo.pSubpasses                             = &subpassDescription;

    VkSubpassDependency subpassDependency                       = {};
    subpassDependency.srcSubpass                                = VK_SUBPASS_EXTERNAL;
    subpassDependency.dstSubpass                                = 0;
    subpassDependency.srcStageMask                              = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependency.srcAccessMask                             = 0;
    subpassDependency.dstStageMask                              = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependency.dstAccessMask                             = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT
                                                                | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    renderPassCreateInfo.dependencyCount                        = 1;
    renderPassCreateInfo.pDependencies                          = &subpassDependency;

    VkResult result = vkCreateRenderPass(
                logicalDevice,
                &renderPassCreateInfo, 
                allocator, 
                &renderPass
                );
    ASSERT(result, "Failed to create render pass", VK_SC_RENDER_PASS_CREATION_ERROR);

    logger::log(EVENT_LOG, "Successfully created render passes");

    return vk::errorCodeBuffer;

}

VK_STATUS_CODE VKEngine::allocateSwapchainFramebuffers() {

    logger::log(EVENT_LOG, "Allocating framebuffers...");

    swapchainFramebuffers.resize(swapchainImageViews.size());

    for (size_t i = 0; i < swapchainImageViews.size(); i++) {
    
        logger::log(EVENT_LOG, "Creating framebuffer...");

#ifdef VK_MULTISAMPLING_NONE
        std::vector< VkImageView > attachments = {
            swapchainImageViews[i],
            depthBuffer->imgView  
        };
#endif
    
#ifndef VK_MULTISAMPLING_NONE
        std::vector< VkImageView > attachments = {
            msaaBufferImage->imgView,
            depthBuffer->imgView,
            swapchainImageViews[i]
        };
#endif

        VkFramebufferCreateInfo framebufferCreateInfo          = {};
        framebufferCreateInfo.sType                            = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferCreateInfo.renderPass                       = renderPass;
        framebufferCreateInfo.attachmentCount                  = static_cast< uint32_t >(attachments.size());
        framebufferCreateInfo.pAttachments                     = attachments.data();
        framebufferCreateInfo.width                            = swapchainImageExtent.width;
        framebufferCreateInfo.height                           = swapchainImageExtent.height;
        framebufferCreateInfo.layers                           = 1;

        VkResult result = vkCreateFramebuffer(
            logicalDevice,
            &framebufferCreateInfo,
            allocator,
            &swapchainFramebuffers[i]
            );
        ASSERT(result, "Failed to create framebuffer", VK_SC_FRAMEBUFFER_ALLOCATION_ERROR);

        logger::log(EVENT_LOG, "Successfully allocated framebuffer");

    }

    logger::log(EVENT_LOG, "Successfully allocated framebuffers");

    return vk::errorCodeBuffer;

}

VK_STATUS_CODE VKEngine::allocateCommandPools() {

    logger::log(EVENT_LOG, "Allocating command pool...");

    QueueFamily family = findSuitableQueueFamily(physicalDevice);

    VkCommandPoolCreateInfo commandPoolCreateInfo          = {};
    commandPoolCreateInfo.sType                            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolCreateInfo.queueFamilyIndex                 = family.graphicsFamilyIndex.value();

    std::unique_lock< std::mutex > graphicsLock(vk::graphicsMutex);
    VkResult result = vkCreateCommandPool(
        logicalDevice,
        &commandPoolCreateInfo,
        allocator,
        &vk::graphicsCommandPool
        );
    ASSERT(result, "Failed to create command pool", VK_SC_COMMAND_POOL_ALLOCATION_ERROR);
    graphicsLock.unlock();

    logger::log(EVENT_LOG, "Successfully allocated command pool");

    logger::log(EVENT_LOG, "Allocating command pool...");

    commandPoolCreateInfo                                  = {};
    commandPoolCreateInfo.sType                            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolCreateInfo.queueFamilyIndex                 = family.transferFamilyIndex.value();

    std::unique_lock< std::mutex > transferLock(vk::transferMutex);
    result = vkCreateCommandPool(
        logicalDevice,
        &commandPoolCreateInfo,
        allocator,
        &vk::transferCommandPool
        );
    transferLock.unlock();

    logger::log(EVENT_LOG, "Successfully allocated command pool");

    return vk::errorCodeBuffer;

}

VK_STATUS_CODE VKEngine::allocateCommandBuffers() {

    logger::log(EVENT_LOG, "Allocating command buffers...");

    standardCommandBuffers.resize(swapchainFramebuffers.size());        // For every frame in the swapchain, create a command buffer

    std::scoped_lock< std::mutex > graphicsLock(vk::graphicsMutex);
    VkCommandBufferAllocateInfo commandBufferAllocateInfo          = {};
    commandBufferAllocateInfo.sType                                = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocateInfo.commandPool                          = vk::graphicsCommandPool;
    commandBufferAllocateInfo.level                                = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferAllocateInfo.commandBufferCount                   = static_cast< uint32_t >(standardCommandBuffers.size());

    VkResult result = vkAllocateCommandBuffers(
        logicalDevice,
        &commandBufferAllocateInfo,
        standardCommandBuffers.data()
        );
    ASSERT(result, "Failed to allocate command buffers", VK_SC_COMMAND_BUFFER_ALLOCATION_ERROR);
    logger::log(EVENT_LOG, "Successfully allocated command buffers");

    logger::log(EVENT_LOG, "Recording command buffers...");
    for (size_t i = 0; i < standardCommandBuffers.size(); i++) {
    
        logger::log(EVENT_LOG, "Recording command buffer...");
        VkCommandBufferBeginInfo commandBufferBeginInfo            = {};
        commandBufferBeginInfo.sType                               = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        commandBufferBeginInfo.flags                               = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

        result = vkBeginCommandBuffer(standardCommandBuffers[i], &commandBufferBeginInfo);
        ASSERT(result, "Failed to allocate command buffer", VK_SC_COMMAND_BUFFER_ALLOCATION_ERROR);

        VkRenderPassBeginInfo renderPassBeginInfo                  = {};
        renderPassBeginInfo.sType                                  = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBeginInfo.renderPass                             = renderPass;
        renderPassBeginInfo.framebuffer                            = swapchainFramebuffers[i];
        renderPassBeginInfo.renderArea.offset                      = {0, 0};
        renderPassBeginInfo.renderArea.extent                      = swapchainImageExtent;
        
        std::array< VkClearValue, 2 > clearColorValues             = {}; 
        clearColorValues[0].color                                  = { 122.0f / 255.0f, 122.0f / 255.0f, 122.0f / 255.0f, 1.0f };
        clearColorValues[1].depthStencil                           = {1.0f, 0};

        renderPassBeginInfo.clearValueCount                        = static_cast< uint32_t >(clearColorValues.size());
        renderPassBeginInfo.pClearValues                           = clearColorValues.data();

        vkCmdBeginRenderPass(standardCommandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);        // Rendering commands will be embedded in the primary command buffer

            vkCmdBindPipeline(standardCommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, standardPipeline.pipeline);

                for (Model* model : models) {
                
                    for (Mesh* mesh : model->meshes) {

                        std::vector< Descriptor > descriptors;

                        descriptors.push_back(mvpDescriptor);

                        auto meshDescriptors = mesh->getDescriptors();
                        descriptors.insert(descriptors.end(), meshDescriptors.begin(), meshDescriptors.end());

                        DescriptorSet* descSet = new DescriptorSet(descriptors);
                        descSet->update(descriptors);

                        descSet->bind(standardCommandBuffers, static_cast< uint32_t >(i), standardPipeline);
                        descriptorSets.push_back(descSet);

                        mesh->draw(standardCommandBuffers, static_cast< uint32_t >(i));

                    }
                
                }

        vkCmdEndRenderPass(standardCommandBuffers[i]);

        result = vkEndCommandBuffer(standardCommandBuffers[i]);
        ASSERT(result, "Failed to record command buffer", VK_SC_COMMAND_BUFFER_RECORDING_ERROR);

        logger::log(EVENT_LOG, "Successfully recorded command buffer");

    }

    logger::log(EVENT_LOG, "Successfully recorded command buffers");

    return vk::errorCodeBuffer;

}

VK_STATUS_CODE VKEngine::showNextSwapchainImage() {
    
    vkWaitForFences(
        logicalDevice,
        1,
        &inFlightFences[currentSwapchainImage],
        VK_TRUE,
        std::numeric_limits< uint64_t >::max()
        );
    vkResetFences(logicalDevice, 1, &inFlightFences[currentSwapchainImage]);

    uint32_t swapchainImageIndex;
    VkResult result = vkAcquireNextImageKHR(
        logicalDevice,
        swapchain,
        std::numeric_limits< uint64_t >::max(),                            // numeric limit of 64-bit unsigned interger disables timeout
        swapchainImageAvailableSemaphores[currentSwapchainImage],          // signal this semaphore once operation is complete
        VK_NULL_HANDLE,
        &swapchainImageIndex
        );
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    
        recreateSwapchain();

        return VK_SC_SWAPCHAIN_RECREATED;

    
    }
    ASSERT(result, "Failed to acquire swapchain image", VK_SC_SWAPCHAIN_IMAGE_ACQUIRE_ERROR);

    ASSERT(updateUniformBuffers(), "Failed to update uniform buffers", VK_SC_UNIFORM_BUFFER_UPDATE_ERROR);

    VkSubmitInfo submitInfo                            = {};
    submitInfo.sType                                   = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    std::unique_lock< std::mutex > lock(vk::graphicsMutex);
    VkSemaphore waitSemaphores[]                       = {swapchainImageAvailableSemaphores[currentSwapchainImage]};
    VkPipelineStageFlags waitStages[]                  = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount                      = 1;
    submitInfo.pWaitSemaphores                         = waitSemaphores;
    submitInfo.pWaitDstStageMask                       = waitStages;
    submitInfo.commandBufferCount                      = 1;
    submitInfo.pCommandBuffers                         = &standardCommandBuffers[swapchainImageIndex];
    lock.unlock();

    VkSemaphore signalSemaphores[]                     = {renderingCompletedSemaphores[currentSwapchainImage]};
    submitInfo.signalSemaphoreCount                    = 1;
    submitInfo.pSignalSemaphores                       = signalSemaphores;

    result = vkQueueSubmit(
        vk::graphicsQueue,
        1,
        &submitInfo,
        inFlightFences[currentSwapchainImage]
        );
    ASSERT(result, "Draw buffer submission failed", VK_SC_QUEUE_SUBMISSION_ERROR);

    VkPresentInfoKHR presentationInfo                  = {};
    presentationInfo.sType                             = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentationInfo.waitSemaphoreCount                = 1;
    presentationInfo.pWaitSemaphores                   = signalSemaphores;

    VkSwapchainKHR swapchains[]                        = {swapchain};
    presentationInfo.swapchainCount                    = 1;
    presentationInfo.pSwapchains                       = swapchains;
    presentationInfo.pImageIndices                     = &swapchainImageIndex;

    result = vkQueuePresentKHR(presentationQueue, &presentationInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || hasFramebufferBeenResized) {

        hasFramebufferBeenResized = false;
        recreateSwapchain();

        return VK_SC_SWAPCHAIN_RECREATED;

    }
    ASSERT(result, "Failed to present swapchain image", VK_SC_PRESENTATION_ERROR);

    currentSwapchainImage = (currentSwapchainImage + 1) % vk::MAX_IN_FLIGHT_FRAMES;

    return vk::errorCodeBuffer;

}

VK_STATUS_CODE VKEngine::initializeSynchronizationObjects() {

    logger::log(EVENT_LOG, "Initializing sync-objects...");

    swapchainImageAvailableSemaphores.resize(vk::MAX_IN_FLIGHT_FRAMES);
    renderingCompletedSemaphores.resize(vk::MAX_IN_FLIGHT_FRAMES);
    inFlightFences.resize(vk::MAX_IN_FLIGHT_FRAMES);

    VkSemaphoreCreateInfo semaphoreCreateInfo        = {};
    semaphoreCreateInfo.sType                        = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceCreateInfo                = {};
    fenceCreateInfo.sType                            = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.flags                            = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < vk::MAX_IN_FLIGHT_FRAMES; i++) {

        VkResult result = vkCreateSemaphore(
            logicalDevice,
            &semaphoreCreateInfo,
            allocator,
            &swapchainImageAvailableSemaphores[i]
            );
        ASSERT(result, "Failed to create semaphore", VK_SC_SEMAPHORE_CREATION_ERROR);
        logger::log(EVENT_LOG, "Successfully initialized semaphore");

        result = vkCreateSemaphore(
            logicalDevice,
            &semaphoreCreateInfo,
            allocator,
            &renderingCompletedSemaphores[i]
            );
        ASSERT(result, "Failed to create semaphore", VK_SC_SEMAPHORE_CREATION_ERROR);
        logger::log(EVENT_LOG, "Successfully initialized semaphore");

        result = vkCreateFence(
            logicalDevice,
            &fenceCreateInfo,
            allocator,
            &inFlightFences[i]
            );
        ASSERT(result, "Failed to create fence", VK_SC_FENCE_CREATION_ERROR);
        logger::log(EVENT_LOG, "Successfully initialized fence");

    }

    VkResult result = vkCreateFence(
        logicalDevice,
        &fenceCreateInfo,
        allocator,
        &vk::graphicsFence
        );
    ASSERT(result, "Failed to create fence", VK_SC_FENCE_CREATION_ERROR);
    logger::log(EVENT_LOG, "Successfully initialized graphics fence");

    result = vkCreateFence(
        logicalDevice,
        &fenceCreateInfo,
        allocator,
        &vk::transferFence
        );
    ASSERT(result, "Failed to create fence", VK_SC_FENCE_CREATION_ERROR);
    logger::log(EVENT_LOG, "Successfully initialized transfer fence");

    logger::log(EVENT_LOG, "Successfully initialized sync-objects");

    return vk::errorCodeBuffer;

}

VK_STATUS_CODE VKEngine::recreateSwapchain() {

    if (!firstTimeRecreation) {

        vkDeviceWaitIdle(logicalDevice);

        int width = 0;
        int height = 0;
        while (width == 0 || height == 0) {

            glfwGetFramebufferSize(window, &width, &height);
            glfwWaitEvents();

        }

        cleanSwapchain();

        ASSERT(createSwapchain(), "Failed to create a swapchain with the given parameters", VK_SC_SWAPCHAIN_CREATION_ERROR);
        ASSERT(createSwapchainImageViews(), "Failed to create swapchain image views", VK_SC_SWAPCHAIN_IMAGE_VIEWS_CREATION_ERROR);
        ASSERT(allocateMSAABufferedImage(), "Failed to allocate multisampling-buffer", VK_SC_MSAA_BUFFER_CREATION_ERROR);
        ASSERT(allocateDepthBuffer(), "Failed to allocate depth buffer", VK_SC_DEPTH_BUFFER_CREATION_ERROR);
        ASSERT(createRenderPasses(), "Failed to create render passes", VK_SC_RENDER_PASS_CREATION_ERROR);
        ASSERT(allocateUniformBuffers(), "Failed to allocate uniform buffers", VK_SC_UNIFORM_BUFFER_CREATION_ERROR);
        ASSERT(createGraphicsPipelines(), "Failed to create graphics pipelines", VK_SC_GRAPHICS_PIPELINE_CREATION_ERROR);
        ASSERT(allocateSwapchainFramebuffers(), "Failed to allocate framebuffers", VK_SC_FRAMEBUFFER_ALLOCATION_ERROR);
        ASSERT(allocateCommandBuffers(), "Failed to allocate command buffers", VK_SC_COMMAND_BUFFER_ALLOCATION_ERROR);

        firstTimeRecreation = false;

    }

    return vk::errorCodeBuffer;

}

VK_STATUS_CODE VKEngine::cleanSwapchain() {

    logger::log(EVENT_LOG, "Cleaning swapchain...");

    delete standardDescriptorLayout;
    logger::log(EVENT_LOG, "Successfully destroyed descriptor set layout");

    delete mvpBuffer;
    logger::log(EVENT_LOG, "Successfully destroyed uniform buffers");

    delete depthBuffer;
    logger::log(EVENT_LOG, "Successfully destroyed depth buffer");
#ifndef VK_MULTISAMPLING_NONE
    delete msaaBufferImage;
    logger::log(EVENT_LOG, "Successfully destroyed multisampled color-buffer");
#endif

    logger::log(EVENT_LOG, "Destroying framebuffers...");
    for (auto framebuffer : swapchainFramebuffers) {

        vkDestroyFramebuffer(logicalDevice, framebuffer, allocator);
        logger::log(EVENT_LOG, "Successfully destroyed framebuffer");

    }
    logger::log(EVENT_LOG, "Successfully destroyed framebuffers");

    std::unique_lock< std::mutex > lock(vk::graphicsMutex);
    vkFreeCommandBuffers(
        logicalDevice,
        vk::graphicsCommandPool,
        static_cast< uint32_t >(standardCommandBuffers.size()),
        standardCommandBuffers.data()
        );
    lock.unlock();
    logger::log(EVENT_LOG, "Successfully freed command buffers");

    standardPipeline.destroy();

    vkDestroyRenderPass(logicalDevice, renderPass, allocator);
    logger::log(EVENT_LOG, "Successfully destroyed render pass");

    for (auto imageView : swapchainImageViews) {

        vkDestroyImageView(logicalDevice, imageView, allocator);
        logger::log(EVENT_LOG, "Successfully destroyed image view");

    }

    vkDestroySwapchainKHR(logicalDevice, swapchain, allocator);
    logger::log(EVENT_LOG, "Successfully destroyed swapchain");

    logger::log(EVENT_LOG, "Successfully cleaned swapchain");

    return vk::errorCodeBuffer;

}

void VKEngine::framebufferResizeCallback(GLFWwindow* window_, int width_, int height_) {

    auto vkengine = reinterpret_cast< VKEngine* >(glfwGetWindowUserPointer(window_));
    vkengine->hasFramebufferBeenResized = true;

}

VK_STATUS_CODE VKEngine::allocateUniformBuffers() {

    VkDeviceSize bufferSize = sizeof(MVPBufferObject);

    VkBufferCreateInfo mvpBufferCreateInfo      = {};
    mvpBufferCreateInfo.sType                   = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    mvpBufferCreateInfo.size                    = bufferSize;
    mvpBufferCreateInfo.usage                   = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    mvpBufferCreateInfo.sharingMode             = VK_SHARING_MODE_EXCLUSIVE;

    mvpBuffer = new UniformBuffer(&mvpBufferCreateInfo, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    logger::log(EVENT_LOG, "Successfully created buffers");

    return vk::errorCodeBuffer;

}

VK_STATUS_CODE VKEngine::updateUniformBuffers() {

    static auto            start                    = std::chrono::high_resolution_clock::now();
    auto                   current                  = std::chrono::high_resolution_clock::now();
                          
    float                  delta                    = std::chrono::duration< float, std::chrono::seconds::period >(current - start).count();        // Namespaces are a fricking mess in <chrono>
    
    MVPBufferObject mvp                             = {};

    mvp.model                                       = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    mvp.model                                       = glm::scale(mvp.model, glm::vec3(0.02f));
    mvp.model[1][1]                                 *= -1.0f;
    mvp.view                                        = camera->getViewMatrix();
    mvp.proj                                        = glm::perspective(static_cast< float >(glm::radians(camera->fov)), swapchainImageExtent.width / static_cast< float >(swapchainImageExtent.height), 0.1f, 100.0f);

    mvpBuffer->fill(&mvp);

    return vk::errorCodeBuffer;

}

VK_STATUS_CODE VKEngine::createCamera() {

    camera = new FPSCamera();

    return vk::errorCodeBuffer;

}

void VKEngine::processKeyboardInput() {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {

        glfwSetWindowShouldClose(window, GLFW_TRUE);

    }

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {

        polygonMode = VK_POLYGON_MODE_FILL;
        vkDeviceWaitIdle(logicalDevice);
        std::unique_lock< std::mutex > lock(vk::graphicsMutex);
        vkFreeCommandBuffers(
            logicalDevice,
            vk::graphicsCommandPool,
            static_cast< uint32_t >(standardCommandBuffers.size()),
            standardCommandBuffers.data()
            );
        lock.unlock();
        logger::log(EVENT_LOG, "Successfully freed command buffers");
        delete standardDescriptorLayout;
        standardDescriptors.clear();
        for (auto descSet : descriptorSets) {

            delete descSet;
            logger::log(EVENT_LOG, "Successfully destroyed descriptor set");

        }
        descriptorSets.clear();
        logger::log(EVENT_LOG, "Successfully destroyed descriptor sets");
        standardPipeline.destroy();
        ASSERT(createGraphicsPipelines(), "Failed to create graphics pipelines", VK_SC_GRAPHICS_PIPELINE_CREATION_ERROR);
        ASSERT(allocateCommandBuffers(), "Failed to allocate command buffers", VK_SC_COMMAND_BUFFER_ALLOCATION_ERROR);

    }

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {

        polygonMode = VK_POLYGON_MODE_LINE;
        vkDeviceWaitIdle(logicalDevice);
        std::unique_lock< std::mutex > lock(vk::graphicsMutex);
        vkFreeCommandBuffers(
            logicalDevice,
            vk::graphicsCommandPool,
            static_cast< uint32_t >(standardCommandBuffers.size()),
            standardCommandBuffers.data()
            );
        lock.unlock();
        logger::log(EVENT_LOG, "Successfully freed command buffers");
        delete standardDescriptorLayout;
        standardDescriptors.clear();
        for (auto descSet : descriptorSets) {

            delete descSet;
            logger::log(EVENT_LOG, "Successfully destroyed descriptor set");

        }
        descriptorSets.clear();
        logger::log(EVENT_LOG, "Successfully destroyed descriptor sets");
        standardPipeline.destroy();
        ASSERT(createGraphicsPipelines(), "Failed to create graphics pipelines", VK_SC_GRAPHICS_PIPELINE_CREATION_ERROR);
        ASSERT(allocateCommandBuffers(), "Failed to allocate command buffers", VK_SC_COMMAND_BUFFER_ALLOCATION_ERROR);

    }

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {

        polygonMode = VK_POLYGON_MODE_POINT;
        vkDeviceWaitIdle(logicalDevice);
        std::unique_lock< std::mutex > lock(vk::graphicsMutex);
        vkFreeCommandBuffers(
            logicalDevice,
            vk::graphicsCommandPool,
            static_cast< uint32_t >(standardCommandBuffers.size()),
            standardCommandBuffers.data()
            );
        lock.unlock();
        logger::log(EVENT_LOG, "Successfully freed command buffers");
        delete standardDescriptorLayout;
        standardDescriptors.clear();
        for (auto descSet : descriptorSets) {

            delete descSet;
            logger::log(EVENT_LOG, "Successfully destroyed descriptor set");

        }
        descriptorSets.clear();
        logger::log(EVENT_LOG, "Successfully destroyed descriptor sets");
        standardPipeline.destroy();
        ASSERT(createGraphicsPipelines(), "Failed to create graphics pipelines", VK_SC_GRAPHICS_PIPELINE_CREATION_ERROR);
        ASSERT(allocateCommandBuffers(), "Failed to allocate command buffers", VK_SC_COMMAND_BUFFER_ALLOCATION_ERROR);

    }

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {

        static double start     = glfwGetTime() - 1.0;      // -1.0 prevents bug for first time switch
        double now              = glfwGetTime();
        static bool pressed     = false;

        if (now - start > 0.5) {

            delete camera;
            if (pressed) {

                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                camera = new FPSCamera();
                pressed = false;
            }
            else {

                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                camera = new CenterCamera(ORIGIN, 5.0f);
                pressed = true;

            }
            start = glfwGetTime();

        }

    }
    camera->processKeyboardInput(window);

}

void VKEngine::mouseMoveCallback(GLFWwindow* window_, double xPos_, double yPos_) {

    auto vkengine = reinterpret_cast< VKEngine* >(glfwGetWindowUserPointer(window_));
    vkengine->camera->processMouseMovement(xPos_, yPos_);

}

void VKEngine::mouseScrollCallback(GLFWwindow* window_, double xOff_, double yOff_) {

    auto vkengine = reinterpret_cast< VKEngine* >(glfwGetWindowUserPointer(window_));
    vkengine->camera->processMouseScroll(xOff_, yOff_);

}

VK_STATUS_CODE VKEngine::allocateDepthBuffer() {

    logger::log(EVENT_LOG, "Creating depth resources...");
    
    depthBuffer = new DepthBuffer();

    logger::log(EVENT_LOG, "Successfully created depth resources");

    return vk::errorCodeBuffer;

}

VkSampleCountFlagBits VKEngine::enumerateMaximumMultisamplingSampleCount() {

    VkPhysicalDeviceProperties physicalDeviceProps;
    vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProps);

    VkSampleCountFlags counts = std::min(physicalDeviceProps.limits.framebufferColorSampleCounts, physicalDeviceProps.limits.framebufferDepthSampleCounts);

    if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
    if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
    if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
    if (counts & VK_SAMPLE_COUNT_8_BIT)  { return VK_SAMPLE_COUNT_8_BIT;  }
    if (counts & VK_SAMPLE_COUNT_4_BIT)  { return VK_SAMPLE_COUNT_4_BIT;  }
    if (counts & VK_SAMPLE_COUNT_2_BIT)  { return VK_SAMPLE_COUNT_2_BIT;  }

    return VK_SAMPLE_COUNT_1_BIT;

}

VK_STATUS_CODE VKEngine::allocateMSAABufferedImage() {
#ifndef VK_MULTISAMPLING_NONE
    msaaBufferImage = new MSAARenderImage();
#endif
    return vk::errorCodeBuffer;

}

VK_STATUS_CODE VKEngine::loadModelsAndVertexData() {

    for (auto info : modelLoadingQueue) {
    
        std::thread* t0 = new std::thread([=]() {
            
                Model* model = new Model(info.path, info.pipeline, info.lib);

                std::scoped_lock< std::mutex > lock(modelsPushBackMutex);
                models.push_back(model);
            
            });
        modelLoadingQueueThreads.push_back(t0);
    
    }

    for (auto thread : modelLoadingQueueThreads) {
    
        thread->join();
        delete thread;
    
    }

    return vk::errorCodeBuffer;

}


VK_STATUS_CODE VKEngine::push(const char* path_) {

    modelLoadingQueue.push_back({ path_, standardPipeline, VK_STANDARD_MODEL_LOADING_LIB });

    return vk::errorCodeBuffer;

}

VK_STATUS_CODE VKEngine::push(ModelInfo info_) {

    modelLoadingQueue.push_back(info_);

    return vk::errorCodeBuffer;

}

void VKEngine::multithreadedNextSwapchainImage() {

    renderThreads.resize(swapchainImages.size());

    for (uint32_t i = 0; i < renderThreads.size(); i++) {
    
        renderThreads[i] = new std::thread([=]() {
            

            
        });

    
    }

}