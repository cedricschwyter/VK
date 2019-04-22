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

	glfwDestroyWindow(window);
	glfwTerminate();
	logger::log(EVENT_LOG, "Successfully terminated GLFW");

	logger::log(EVENT_LOG, "Successfully cleaned allocated resources, shutting down...");

	return VK_SC_SUCCESS;

}
