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

	window = glfwCreateWindow(
		WIDTH,
		HEIGHT,
		TITLE,
		nullptr,
		nullptr
		);
	logger::log(EVENT_LOG, "Successfully initialized window");

	return VK_SC_SUCCESS;

}

VK_STATUS_CODE VKEngine::initVulkan() {

	ASSERT(createInstance(), "Instance creation error", VK_SC_INSTANCE_CREATON_ERROR);
	ASSERT(debugUtilsMessenger(), "Debug utils messenger creation error", VK_SC_DEBUG_UTILS_MESSENGER_CREATION_ERROR);
	ASSERT(selectBestPhysicalDevice(), "Failed to find a suitable GPU that supports Vulkan", VK_SC_PHYSICAL_DEVICE_ERROR);

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

	if (validationLayersEnabled) {
	
		vk::destroyDebugUtilsMessenger(instance, validationLayerDebugMessenger, allocator);
		logger::log(EVENT_LOG, "Successfully destroyed debug utils messenger");

	}

	vkDestroyInstance(instance, allocator);
	logger::log(EVENT_LOG, "Successfully destroyed instance");

	glfwDestroyWindow(window);
	glfwTerminate();
	logger::log(EVENT_LOG, "Successfully terminated GLFW");

	logger::log(EVENT_LOG, "Successfully cleaned allocated resources, shutting down...");

	std::cout << "\n\nPress any key to continue...";
	_getch();

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
	applicationInfo.pApplicationName				= TITLE;
	applicationInfo.applicationVersion				= VK_MAKE_VERSION(1, 0, 0);
	applicationInfo.pEngineName						= "D3PSI's VKEngine";
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

	if (messageSeverity_ <= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {

		logger::log(EVENT_LOG, header + message);

	}

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
		return VK_SC_PHYSICAL_DEVICE_ERROR;
	
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

		logger::log(EVENT_LOG, "Suitable GPU found");
		physicalDevice = possibleGPUs.rbegin()->second;

		return VK_SC_SUCCESS;

	}
	else {
	
		logger::log(ERROR_LOG, "Failed to find a suitable GPU that supports Vulkan");
		return VK_SC_PHYSICAL_DEVICE_VULKAN_SUPPORT_ERROR;

	}

}

int VKEngine::evaluateDeviceSuitabilityScore(VkPhysicalDevice device_) {

	QueueFamily queue = findSuitableQueueFamilies(device_);

	VkPhysicalDeviceProperties physicalDeviceProperties;
	vkGetPhysicalDeviceProperties(device_, &physicalDeviceProperties);

	VkPhysicalDeviceFeatures physicalDeviceFeatures;
	vkGetPhysicalDeviceFeatures(device_, &physicalDeviceFeatures);

	int score = 0;

	if (physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {

		score += 1000;

	}

	if (!queue.isComplete()) {
	
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

	QueueFamily indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device_, &queueFamilyCount, nullptr);

	std::vector< VkQueueFamilyProperties > queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device_, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& qF : queueFamilies) {
	
		if (qF.queueCount > 0 && (qF.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {		// Does the queue family have at least one queue and does it support graphics-operations?
		
			indices.queueFamily = i;

		}

		if (indices.isComplete()) {
		
			break;

		}

		i++;

	}

	return indices;

}