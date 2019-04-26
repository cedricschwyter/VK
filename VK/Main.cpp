/**
	Implements the main entry point for the VKEngine

	@author		D3PSI
	@version	0.0.1 02.12.2019

	@file		Main.cpp
	@brief		Implementation of the Vulkan-part for my Vulkan vs. OpenGL comparison (Maturaarbeit)
*/

#include <iostream>

#include "VKEngine.hpp"
#include "VK_STATUS_CODE.hpp"
#include "VK.hpp"

namespace vk {

	VKEngine							engine;
	const unsigned int					WIDTH		= 1280;
	const unsigned int					HEIGHT		= 720;
	const char*							TITLE		= "VK by D3PSI";

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

}

/**
	Entry point for the application
*/
int main() {

	return vk::init();

}