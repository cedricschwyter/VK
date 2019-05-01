/**
	Implements the VKEngine class

	@author		D3PSI
	@version	0.0.1 02.12.2019

	@file		VKEngine.cpp
	@brief		Implementation of the VKEngine class
*/

#include "VKEngine.hpp"
#include "VK.hpp"


VK_STATUS_CODE VKEngine::init() {

	ASSERT(initLogger(), "Logger initialization error", LOGGER_SC_UNKNOWN_ERROR);
	logger::log(START_LOG, "Initializing...");
	logger::log(EVENT_LOG, "Initializing loading screen...");
	initLoadingScreen();
	ASSERT(initWindow(), "Window initialization error", VK_SC_WINDOW_ERROR);
	ASSERT(initVulkan(), "Vulkan initialization error", VK_SC_VULKAN_ERROR);
	ASSERT(loop(), "Vulkan runtime error", VK_SC_VULKAN_RUNTIME_ERROR);
	ASSERT(clean(), "Application cleanup error", VK_SC_CLEANUP_ERROR);
	logger::log(START_LOG, "Shutting down...");

	return VK_SC_SUCCESS;

}

LOGGER_STATUS_CODE VKEngine::initLogger() {

	return logger::init();

}

VK_STATUS_CODE VKEngine::initWindow() {

	logger::log(EVENT_LOG, "Initializing window...");
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	window = glfwCreateWindow(
		vk::WIDTH,
		vk::HEIGHT,
		vk::TITLE,
		nullptr,
		nullptr
		);
	logger::log(EVENT_LOG, "Successfully initialized window");

	return VK_SC_SUCCESS;

}

VK_STATUS_CODE VKEngine::initVulkan() {

	allocator = nullptr;

	ASSERT(createInstance(), "Instance creation error", VK_SC_INSTANCE_CREATON_ERROR);
	ASSERT(debugUtilsMessenger(), "Debug utils messenger creation error", VK_SC_DEBUG_UTILS_MESSENGER_CREATION_ERROR);
	ASSERT(createSurfaceGLFW(), "Surface creation error", VK_SC_SURFACE_CREATION_ERROR);
	ASSERT(selectBestPhysicalDevice(), "Failed to find a suitable GPU that supports Vulkan", VK_SC_PHYSICAL_DEVICE_CREATION_ERROR);
	ASSERT(createLogicalDeviceFromPhysicalDevice(), "Failed to create a logical device from the selected physical device", VK_SC_LOGICAL_DEVICE_CREATION_ERROR);
	ASSERT(createSwapchain(), "Failed to create a swapchain with the given parameters", VK_SC_SWAPCHAIN_CREATION_ERROR);
	ASSERT(createSwapchainImageViews(), "Failed to create swapchain image views", VK_SC_SWAPCHAIN_IMAGE_VIEWS_CREATION_ERROR);
	ASSERT(createGraphicsPipelines(), "Failed to create graphics pipelines", VK_SC_GRAPHICS_PIPELINE_CREATION_ERROR);

	glfwShowWindow(window);
	glfwFocusWindow(window);

	loadingScreen->closeMutex.lock();
	loadingScreen->close = true;
	loadingScreen->closeMutex.unlock();

	return VK_SC_SUCCESS;

}

VK_STATUS_CODE VKEngine::loop() {

	logger::log(EVENT_LOG, "Entering application loop...");

	while (!glfwWindowShouldClose(window)) {
	
		glfwPollEvents();
	
	}

	logger::log(EVENT_LOG, "Terminating...");

	return VK_SC_SUCCESS;

}

VK_STATUS_CODE VKEngine::clean() {

	for (auto imageView : swapchainImageViews) {

		vkDestroyImageView(logicalDevice, imageView, allocator);

	}

	vkDestroySwapchainKHR(logicalDevice, swapchain, allocator);
	logger::log(EVENT_LOG, "Successfully destroyed swapchain");

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

#if defined VK_DEVELOPMENT
	std::cout << "\n\nPress any key to continue...";
	logger::log(EVENT_LOG, "Exit code: " + _getch());
#endif
	return VK_SC_SUCCESS;

}

VK_STATUS_CODE VKEngine::createInstance() {

	logger::log(EVENT_LOG, "Requesting validation layers...");
	if (validationLayersEnabled && !validationLayersSupported()) {
	
		logger::log(ERROR_LOG, "Validation layers are not available");

	}
	logger::log(EVENT_LOG, "Successfully enabled validation layers");

	auto extensions									= queryRequiredExtensions();

	VkApplicationInfo applicationInfo				= {};
	applicationInfo.sType							= VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.pApplicationName				= vk::TITLE;
	applicationInfo.applicationVersion				= VK_MAKE_VERSION(1, 0, 0);
	applicationInfo.pEngineName						= vk::TITLE;
	applicationInfo.engineVersion					= VK_MAKE_VERSION(1, 0, 0);
	applicationInfo.apiVersion						= VK_API_VERSION_1_0;

	VkInstanceCreateInfo instanceCreateInfo			= {};
	instanceCreateInfo.sType						= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pApplicationInfo				= &applicationInfo;
	instanceCreateInfo.enabledExtensionCount		= static_cast< uint32_t >(extensions.size());
	instanceCreateInfo.ppEnabledExtensionNames		= extensions.data();
	
	if (validationLayersEnabled) {
	
		instanceCreateInfo.enabledLayerCount		= static_cast< uint32_t >(validationLayers.size());
		instanceCreateInfo.ppEnabledLayerNames		= validationLayers.data();

	}
	else {
	
		instanceCreateInfo.enabledLayerCount		= 0;

	}

	logger::log(EVENT_LOG, "Creating VkInstance...");
	result = vkCreateInstance(&instanceCreateInfo, allocator, &instance);
	ASSERT(result, "Failed to create VkInstance!", VK_SC_INSTANCE_CREATON_ERROR);

	return VK_SC_SUCCESS;

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
	VkDebugUtilsMessageSeverityFlagBitsEXT			messageSeverity_,
	VkDebugUtilsMessageTypeFlagsEXT					messageType_,
	const VkDebugUtilsMessengerCallbackDataEXT*		pCallbackData_,
	void*											pUserData_
	) {

	std::string header = "Validation Layer:	";
	std::string message(pCallbackData_->pMessage);

	logger::log(EVENT_LOG, header + message);

	if (messageSeverity_ > VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
	
		logger::log(ERROR_LOG, header + message);

	}

	return VK_FALSE;

}

VK_STATUS_CODE VKEngine::debugUtilsMessenger() {

	if (!validationLayersEnabled) return VK_SC_SUCCESS;

	logger::log(EVENT_LOG, "Creating debug utils messenger...");

	VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo		= {};
	debugUtilsMessengerCreateInfo.sType										= VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	debugUtilsMessengerCreateInfo.messageSeverity							= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
																			| VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
																			| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
																			| VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
	debugUtilsMessengerCreateInfo.messageType								= VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
																			| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
																			| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	debugUtilsMessengerCreateInfo.pfnUserCallback							= validationLayerDebugMessageCallback;
	debugUtilsMessengerCreateInfo.pUserData									= nullptr;

	result = vk::createDebugUtilsMessenger(
		instance,
		&debugUtilsMessengerCreateInfo,
		allocator,
		&validationLayerDebugMessenger);

	ASSERT(result, "Debug utils messenger creation error", VK_SC_DEBUG_UTILS_MESSENGER_CREATION_ERROR);

	logger::log(EVENT_LOG, "Successfully created debug utils messenger");

	return VK_SC_SUCCESS;

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

	if (possibleGPUs.rbegin()->first > 0) {		// Is the first possibility even suitable?

		logger::log(EVENT_LOG, "Suitable GPU found: ");
		printPhysicalDevicePropertiesAndFeatures(possibleGPUs.rbegin()->second);
		physicalDevice = possibleGPUs.rbegin()->second;

		return VK_SC_SUCCESS;

	}
	else {
	
		logger::log(ERROR_LOG, "Failed to find a suitable GPU that supports Vulkan");
		return VK_SC_PHYSICAL_DEVICE_VULKAN_SUPPORT_ERROR;

	}

}

int VKEngine::evaluateDeviceSuitabilityScore(VkPhysicalDevice device_) {

	QueueFamily				families				= findSuitableQueueFamilies(device_);
	SwapchainDetails		swapchainDetails		= querySwapchainDetails(device_);

	VkPhysicalDeviceProperties physicalDeviceProperties;
	vkGetPhysicalDeviceProperties(device_, &physicalDeviceProperties);

	VkPhysicalDeviceFeatures physicalDeviceFeatures;
	vkGetPhysicalDeviceFeatures(device_, &physicalDeviceFeatures);

	int score = 0;

	if (physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {

		score += 1000;

	}

	if (!families.isComplete() 
		|| !checkDeviceSwapchainExtensionSupport(device_) 
		|| swapchainDetails.supportedFormats.empty() 
		|| swapchainDetails.presentationModes.empty()
		) {		// absolutely necessary features needed to run application on that GPU
	
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
	std::string info = std::string("GPU information:	") + physicalDeviceProperties.deviceName;

	logger::log(EVENT_LOG, info);

	return VK_SC_SUCCESS;

}

QueueFamily VKEngine::findSuitableQueueFamilies(VkPhysicalDevice device_) {

	QueueFamily families;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device_, &queueFamilyCount, nullptr);

	std::vector< VkQueueFamilyProperties > queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device_, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& qF : queueFamilies) {
	
		if (qF.queueCount > 0 && (qF.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {		// Does the queue family have at least one queue and does it support graphics-operations?
		
			families.graphicsFamilyIndex = i;

		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(
			device_,
			i, 
			surface,
			&presentSupport
			);

		if (qF.queueCount > 0 && presentSupport) {		// Also a presentation queue family is needed to actually display to the surface

			families.presentationFamilyIndex = i;
		
		}


		if (families.isComplete()) {
		
			break;

		}

		i++;

	}

	return families;

}

VK_STATUS_CODE VKEngine::createLogicalDeviceFromPhysicalDevice() {

	logger::log(EVENT_LOG, "Creating logical device...");
	QueueFamily families = findSuitableQueueFamilies(physicalDevice);

	std::vector< VkDeviceQueueCreateInfo > deviceQueueCreateInfos;
	std::set< uint32_t > uniqueQueueFamilies = { families.graphicsFamilyIndex.value(), families.presentationFamilyIndex.value() };

	float queuePriority = 1.0f;
	for (uint32_t qF : uniqueQueueFamilies) {

		VkDeviceQueueCreateInfo deviceQueueCreateInfo		= {};
		deviceQueueCreateInfo.sType							= VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		deviceQueueCreateInfo.queueFamilyIndex				= qF;
		deviceQueueCreateInfo.queueCount					= 1;
		deviceQueueCreateInfo.pQueuePriorities				= &queuePriority;

		deviceQueueCreateInfos.push_back(deviceQueueCreateInfo);

	}

	VkPhysicalDeviceFeatures physicalDeviceFeatures			= {};		// No features are necessary at the moment so this struct is just initialized to VK_FALSE (0)
	
	VkDeviceCreateInfo deviceCreateInfo						= {};
	deviceCreateInfo.sType									= VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount					= static_cast< uint32_t >(deviceQueueCreateInfos.size());
	deviceCreateInfo.pQueueCreateInfos						= deviceQueueCreateInfos.data();
	deviceCreateInfo.enabledExtensionCount					= static_cast< uint32_t >(requiredExtensions.size());
	deviceCreateInfo.ppEnabledExtensionNames				= requiredExtensions.data();
	deviceCreateInfo.pEnabledFeatures						= &physicalDeviceFeatures;

	if (validationLayersEnabled) {

		deviceCreateInfo.enabledLayerCount = static_cast< uint32_t >(validationLayers.size());
		deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();

	}
	else {

		deviceCreateInfo.enabledLayerCount = 0;

	}

	result = vkCreateDevice(
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
		families.graphicsFamilyIndex.value(), 
		0, 
		&graphicsQueue
		);
	logger::log(EVENT_LOG, "Successfully retrieved queue handle for graphics queue");

	logger::log(EVENT_LOG, "Retrieving queue handle for presentation queue...");
	vkGetDeviceQueue(
		logicalDevice,
		families.presentationFamilyIndex.value(),
		0,
		&presentationQueue
	);
	logger::log(EVENT_LOG, "Successfully retrieved queue handle for presentation queue");

	return VK_SC_SUCCESS;

}

VK_STATUS_CODE VKEngine::createSurfaceGLFW() {

	ASSERT(glfwCreateWindowSurface(
		instance, 
		window, 
		allocator, 
		&surface
		), "GLFW surface creation error", VK_SC_SURFACE_CREATION_ERROR);

	return VK_SC_SUCCESS;

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
	
	logger::log(extensions.empty() ? EVENT_LOG : ERROR_LOG, extensions.empty() ? "Device supports necessary extensions" : "Device does not support necessary extensions");

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

	if (availableFormats_.size() == 1 && availableFormats_[0].format == VK_FORMAT_UNDEFINED) {		// If Vulkan only returns one entry and its format is set to VK_FORMAT_UNDEFINED it means that Vulkan has no preferred format so we can choose freely

		return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };

	}

	for (const auto& format : availableFormats_) {

		if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
		
			return format;
		
		}

	}

	return availableFormats_[0];		// If there are no better choices, just return the first one in the array, usually it is good enough

}

VkPresentModeKHR VKEngine::evaluateBestSwapchainSurfacePresentMode(const std::vector< VkPresentModeKHR >& availablePresentModes_) {

	for (const auto& presMode : availablePresentModes_) {
	
		if (presMode == VK_PRESENT_MODE_MAILBOX_KHR) {		// Prefer VK_PRESENT_MODE_MAILBOX_KHR over VK_PRESENT_MODE_FIFO_KHR because of tearing issues
		
			return presMode;
		
		}
	
	}

	return VK_PRESENT_MODE_FIFO_KHR;		// If no better mode is available, return VK_PRESENT_MODE_FIFO_KHR as its implied to be supported if the GPU support Vulkan

}

VkExtent2D VKEngine::evaluateSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities_) {

	if (capabilities_.currentExtent.width != std::numeric_limits<uint32_t>::max()) {

		return capabilities_.currentExtent;

	}
	else {
		VkExtent2D preferredExtent		= { vk::WIDTH, vk::HEIGHT };

		preferredExtent.width			= std::clamp(preferredExtent.width, capabilities_.minImageExtent.width, capabilities_.maxImageExtent.width);
		preferredExtent.height			= std::clamp(preferredExtent.height, capabilities_.minImageExtent.height, capabilities_.maxImageExtent.height);

		return preferredExtent;
	}

}

VK_STATUS_CODE VKEngine::createSwapchain() {

	logger::log(EVENT_LOG, "Creating swapchain...");
	SwapchainDetails		details						= querySwapchainDetails(physicalDevice);

	VkSurfaceFormatKHR		surfaceFormat				= evaluateBestSwapchainSurfaceFormat(details.supportedFormats);
	VkPresentModeKHR		presentMode					= evaluateBestSwapchainSurfacePresentMode(details.presentationModes);
	VkExtent2D				extent						= evaluateSwapchainExtent(details.surfaceCapabilities);

	swapchainImageFormat								= surfaceFormat.format;
	swapchainImageExtent								= extent;

	uint32_t				swapchainImageCount			= details.surfaceCapabilities.minImageCount + 1;
	if (details.surfaceCapabilities.maxImageCount > 0 && swapchainImageCount > details.surfaceCapabilities.maxImageCount) {
	
		swapchainImageCount = details.surfaceCapabilities.maxImageCount;
	
	}

	VkSwapchainCreateInfoKHR swapchainCreateInfo		= {};
	swapchainCreateInfo.sType							= VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainCreateInfo.surface							= surface;
	swapchainCreateInfo.minImageCount					= swapchainImageCount;
	swapchainCreateInfo.imageFormat						= surfaceFormat.format;
	swapchainCreateInfo.imageColorSpace					= surfaceFormat.colorSpace;
	swapchainCreateInfo.imageExtent						= extent;
	swapchainCreateInfo.imageArrayLayers				= 1;										// Amount of layers in an image, always 1, unless doing stereoscopic stuff
	swapchainCreateInfo.imageUsage						= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;		// Render directly to swapchain

	QueueFamily families								= findSuitableQueueFamilies(physicalDevice);
	uint32_t queueFamilyIndices[]						= { families.graphicsFamilyIndex.value(), families.presentationFamilyIndex.value() };

	if (families.graphicsFamilyIndex != families.presentationFamilyIndex) {		// If presentation queue and graphics queue are in the same queue family, exclusive ownership is not necessary

		swapchainCreateInfo.imageSharingMode			= VK_SHARING_MODE_CONCURRENT;
		swapchainCreateInfo.queueFamilyIndexCount		= 2;
		swapchainCreateInfo.pQueueFamilyIndices			= queueFamilyIndices;

	}
	else {																		// else it is

		swapchainCreateInfo.imageSharingMode			= VK_SHARING_MODE_EXCLUSIVE;

	}
	
	swapchainCreateInfo.preTransform					= details.surfaceCapabilities.currentTransform;		// Specify current image transform as no transform is necessary
	swapchainCreateInfo.compositeAlpha					= VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;				// Window should not be blended with other windows behind it, no thanks
	swapchainCreateInfo.clipped							= true;												// Do not render pixels that are outside the clip space as this is just performance loss
	swapchainCreateInfo.presentMode						= presentMode;
	swapchainCreateInfo.oldSwapchain					= VK_NULL_HANDLE;

	result = vkCreateSwapchainKHR(
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

	return VK_SC_SUCCESS;

}

VK_STATUS_CODE VKEngine::createSwapchainImageViews() {

	swapchainImageViews.resize(swapchainImages.size());

	for (size_t i = 0; i < swapchainImages.size(); i++) {
	
		VkImageViewCreateInfo imageViewCreateInfo				= {};
		imageViewCreateInfo.sType								= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.image								= swapchainImages[i];
		imageViewCreateInfo.viewType							= VK_IMAGE_VIEW_TYPE_2D;			// 2D textures will be used
		imageViewCreateInfo.format								= swapchainImageFormat;
		imageViewCreateInfo.components.r						= VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.g						= VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.b						= VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.a						= VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.subresourceRange.aspectMask			= VK_IMAGE_ASPECT_COLOR_BIT;		// use as color target
		imageViewCreateInfo.subresourceRange.baseMipLevel		= 0;
		imageViewCreateInfo.subresourceRange.levelCount			= 1;
		imageViewCreateInfo.subresourceRange.baseArrayLayer		= 0;
		imageViewCreateInfo.subresourceRange.layerCount			= 1;

		result = vkCreateImageView(
			logicalDevice,
			&imageViewCreateInfo,
			allocator,
			&swapchainImageViews[i]
			);
		ASSERT(result, "Failed to create VkImageView", VK_SC_SWAPCHAIN_IMAGE_VIEWS_CREATION_ERROR);

	}

	return VK_SC_SUCCESS;

}

VK_STATUS_CODE VKEngine::createGraphicsPipelines() {

	VertFragShaderStages stages("shaders/standard/vert.spv", "shaders/standard/frag.spv");



	stages.destroyModules();

	return VK_SC_SUCCESS;

}