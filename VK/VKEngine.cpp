/**
	Implements the VKEngine class

	@author		D3PSI
	@version	0.0.1 02.12.2019

	@file		VKEngine.cpp
	@brief		Implementation of the VKEngine class
*/

#include "VKEngine.hpp"


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

	createInstance();

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

	VkApplicationInfo applicationInfo				= {};
	applicationInfo.sType							= VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.pApplicationName				= TITLE;
	applicationInfo.applicationVersion				= VK_MAKE_VERSION(1, 0, 0);
	applicationInfo.pEngineName						= "D3PSI's VKEngine";
	applicationInfo.engineVersion					= VK_MAKE_VERSION(1, 0, 0);
	applicationInfo.apiVersion						= VK_API_VERSION_1_0;

	logger::log(EVENT_LOG, "Querying available extensions...");
	uint32_t extCount								= 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
	
	std::vector< VkExtensionProperties > extensions(extCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extCount, extensions.data());
	std::string exts = "Available extensions:\n";
	for (const auto& ext : extensions) {
	
		std::string extName = ext.extensionName;
		exts += "\t\t\t\t\t\t\t" + extName + "\n";

	}
	logger::log(EVENT_LOG, exts.c_str());

	uint32_t glfwExtCount							= 0;
	const char** glfwExt;

	logger::log(EVENT_LOG, "Querying GLFW-extensions...");
	glfwExt = glfwGetRequiredInstanceExtensions(&glfwExtCount);
	logger::log(EVENT_LOG, "Successfully enabled required GLFW-extensions");

	VkInstanceCreateInfo instanceCreateInfo			= {};
	instanceCreateInfo.sType						= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pApplicationInfo				= &applicationInfo;
	instanceCreateInfo.enabledExtensionCount		= glfwExtCount;
	instanceCreateInfo.ppEnabledExtensionNames		= glfwExt;
	instanceCreateInfo.enabledLayerCount			= 0;	// TODO: Enable validation layers!

	logger::log(EVENT_LOG, "Creating VkInstance...");
	result = vkCreateInstance(&instanceCreateInfo, allocator, &instance);
	ASSERT(result, "Failed to create VkInstance!", VK_SC_INSTANCE_CREATON_ERROR);

	return VK_SC_SUCCESS;

}
