/**
	Implements the main entry point for the VKEngine

	@author		D3PSI
	@version	0.0.1 02.12.2019

	@file		Main.cpp
	@brief		Implementation of the Vulkan-part for my Vulkan vs. OpenGL comparison (Maturaarbeit)
*/

#include "Version.hpp"

#if !defined VK_DEVELOPMENT && defined VK_RELEASE
	#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include <iostream>
#define NOMINMAX		// Fixes std::min/std::max/std::clamp error
#include <Windows.h>

#include "VK.hpp"
#include "VKEngine.hpp"
#include "VK_STATUS_CODE.hpp"

namespace vk {

	VKEngine							engine;
	const unsigned int					WIDTH						= 1280;
	const unsigned int					HEIGHT						= 720;
	const char*							TITLE						= "VK by D3PSI";
	const unsigned int					MAX_IN_FLIGHT_FRAMES		= 4;

	VK_STATUS_CODE init() {
	
		try {

			return engine.init();
		
		} 
		catch (const std::exception& e) {

			std::cerr << e.what() << std::endl;

			return VK_SC_UKNOWN_ERROR;

		}
	
	}

	VkResult createDebugUtilsMessenger(
		VkInstance										instance_,
		const VkDebugUtilsMessengerCreateInfoEXT*		pCreateInfo_,
		const VkAllocationCallbacks*					pAllocator_,
		VkDebugUtilsMessengerEXT*						pDebugMessenger_
		) {
	
		logger::log(EVENT_LOG, "Gathering proc-address for 'vkCreateDebugUtilsMessengerEXT'");
		// Get proc-address from Vulkan
		auto function = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance_, "vkCreateDebugUtilsMessengerEXT");
	
		if (function != nullptr) {

			return function(
				instance_, 
				pCreateInfo_, 
				pAllocator_, 
				pDebugMessenger_
				);
		
		}
		else {
		
			return VK_ERROR_EXTENSION_NOT_PRESENT;

		}
	
	}

	VK_STATUS_CODE destroyDebugUtilsMessenger(
		VkInstance							instance_, 
		VkDebugUtilsMessengerEXT			debugMessenger_, 
		const VkAllocationCallbacks*		pAllocator_
		) {

		logger::log(EVENT_LOG, "Gathering proc-address for 'vkDestroyDebugUtilsMessengerEXT'");
		auto function = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance_, "vkDestroyDebugUtilsMessengerEXT");
		if (function != nullptr) {

			function(instance_, debugMessenger_, pAllocator_);
			return VK_SC_SUCCESS;

		}
		else {
		
			return VK_SC_DEBUG_UTILS_MESSENGER_CLEANUP_ERROR;
		
		}
	
	}

	void hideConsole() {

		::ShowWindow(::GetConsoleWindow(), SW_HIDE);
	
	}

	void showConsole() {

		::ShowWindow(::GetConsoleWindow(), SW_SHOW);
	
	}

	bool isConsoleVisible() {

		return ::IsWindowVisible(::GetConsoleWindow()) != FALSE;
	
	}

	const std::vector< char > loadFile(const std::string& filePath_) {
	
		logger::log(EVENT_LOG, "Loading file at '" + filePath_ + "'");

		std::ifstream file(filePath_, std::ios::ate | std::ios::binary);		// Start reading at end of file --> determine the buffer size needed

		if (!file.is_open()) {
		
			logger::log(ERROR_LOG, "Failed to load file at '" + filePath_ + "'");

		}

		size_t bufferSize = (size_t)file.tellg();		// Find read position and thus necessary buffer size
		std::vector< char > buffer(bufferSize);

		file.seekg(0);		// Translate back to the beginning of the file
		file.read(buffer.data(), bufferSize);
		file.close();

		return buffer;
	
	}

}

/**
	Entry point for the application
*/
int main() {

	return vk::init();

}