/**
	Implements the VKEngine class

	@author		D3PSI
	@version	0.0.1 02.12.2019

	@file		VKEngine.cpp
	@brief		Implementation of the VKEngine class
*/

#include "VKEngine.hpp"


VK_STATUS_CODE VKEngine::init() {
	
	if (initLogger()	!= LOGGER_SC_SUCCESS)	return VK_SC_LOGGER_ERROR;
	if (initWindow()	!= VK_SC_SUCCESS)		return VK_SC_WINDOW_ERROR;
	if (initVulkan()	!= VK_SC_SUCCESS)		return VK_SC_VULKAN_ERROR;
	if (loop()			!= VK_SUCCESS)			return VK_SC_VULKAN_RUNTIME_ERROR;
	if (clean()			!= VK_SUCCESS)			return VK_SC_CLEANUP_ERROR;
	
	return VK_SC_SUCCESS;

}

LOGGER_STATUS_CODE VKEngine::initLogger() {

	return logger::init();

}

VK_STATUS_CODE VKEngine::initWindow() {

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

	return VK_SC_SUCCESS;

}

VK_STATUS_CODE VKEngine::initVulkan() {

	return VK_SC_SUCCESS;

}

VK_STATUS_CODE VKEngine::loop() {

	while (!glfwWindowShouldClose(window)) {
	
		glfwPollEvents();
	
	}

	return VK_SC_SUCCESS;

}

VK_STATUS_CODE VKEngine::clean() {

	glfwDestroyWindow(window);
	glfwTerminate();

	return VK_SC_SUCCESS;

}
