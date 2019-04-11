/**
	Implements the VKEngine class

	@author		D3PSI
	@version	0.0.1 02.12.2019

	@file		VKEngine.cpp
	@brief		Implementation of the VKEngine class
*/

#include "VKEngine.hpp"


VK_STATUS_CODE VKEngine::init() {

	std::cout << "Hello from VKEngine!" << std::endl;

	initWindow();
	initVulkan();
	loop();
	clean();
	
	return VK_SC_SUCCESS;

}

VK_STATUS_CODE VKEngine::initWindow() {

	return VK_SC_SUCCESS;

}

VK_STATUS_CODE VKEngine::initVulkan() {

	return VK_SC_SUCCESS;

}

VK_STATUS_CODE VKEngine::loop() {

	return VK_SC_SUCCESS;

}

VK_STATUS_CODE VKEngine::clean() {

	return VK_SC_SUCCESS;

}
