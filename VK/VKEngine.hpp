/**
	Declares the VKEngine class

	@author		D3PSI
	@version	0.0.1 02.12.2019

	@file		VKEngine.hpp
	@brief		Declaration of the VKEngine class
*/

#pragma once
#include "Version.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <conio.h>
#include <map>

#include "VK_STATUS_CODE.hpp"
#include "Logger.hpp"
#include "ASSERT.cpp"
#include "QueueFamily.cpp"

class VKEngine {
public:

	const unsigned int					WIDTH					= 1280;
	const unsigned int					HEIGHT					= 720;
	const char*							TITLE					= "VK by D3PSI";

	/**
		Initializes VKEngine and loads dependencies

		@return		Returns VK_SC_SUCCESS on success
	*/
	VK_STATUS_CODE init(void);

private:

	VkResult								result;
	GLFWwindow*								window;
	VkAllocationCallbacks*					allocator			= nullptr;
	VkInstance								instance;
	const std::vector< const char* >		validationLayers	= {
	
		"VK_LAYER_LUNARG_standard_validation"
	
	};
#ifdef VK_DEVELOPMENT
	const bool								validationLayersEnabled			= true;
#else
	const bool								validationLayersEnabled			= false;
#endif
	VkDebugUtilsMessengerEXT				validationLayerDebugMessenger;
	VkPhysicalDevice						physicalDevice					= VK_NULL_HANDLE;
	VkDevice								logicalDevice;
	VkQueue									graphicsQueue;
	VkSurfaceKHR							surface;

	/**
		Initializes the logger

		@return		Returns LOGGER_SC_SUCCESS on success
		@return		Returns LOGGER_SC_UNKNOWN_ERROR on error
	*/
	LOGGER_STATUS_CODE initLogger(void);

	/**
		Initializes the windowing library

		@return		Returns VK_SC_SUCCESS on success
	*/
	VK_STATUS_CODE initWindow(void);

	/**
		Initializes the Vulkan API

		@return		Returns VK_SC_SUCCESS on success
	*/
	VK_STATUS_CODE initVulkan(void);

	/**
		Contains the main loop

		@return		Returns VK_SC_SUCCESS on success
	*/
	VK_STATUS_CODE loop(void);

	/**
		Handles cleaning of resources on exit

		@return		Returns VK_SC_SUCCESS on success
	*/
	VK_STATUS_CODE clean(void);

	/**
		Creates the VkInstance class member

		@return		Returns VK_SC_SUCCESS on success
	*/
	VK_STATUS_CODE createInstance(void);

	/**
		Checks whether validation layers are supported

		@return		Returns true if validation layers are supported
		@return		Returns false if validation layers are not supported
	*/
	bool validationLayersSupported(void);

	/**
		Get the required extensions for Vulkan to activate

		@return		Returns an std::vector< const char* > with extension names
	*/
	std::vector< const char* > queryRequiredExtensions(void);

	/**
		Message callback function for Vulkan validation layers so they can actually display their messages

		@param		messageSeverity_	Message severity level (internal)
		@param		messageType_		Message type (internal)
		@param		pCallbackData_		Actual message (internal)
		@param		pUserData_			User specified data

		@return		Returns VK_FALSE

	*/
	static VKAPI_ATTR VkBool32 VKAPI_CALL validationLayerDebugMessageCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT			messageSeverity_,
		VkDebugUtilsMessageTypeFlagsEXT					messageType_,
		const VkDebugUtilsMessengerCallbackDataEXT*		pCallbackData_,
		void*											pUserData_
		);

	/**
		Creates the VkDebugUtilsMessengerEXT for validation layer messages

		@return		Returns VK_SC_SUCCESS on success
		@return		Returns VK_SC_DEBUG_UTILS_MESSENGER_CREATION_ERROR on error
	*/
	VK_STATUS_CODE debugUtilsMessenger(void);

	/**
		Enumerates and selects the best physical device (GPU)

		@return		Returns VK_SC_SUCCESS on success
		@return		Returns VK_SC_PHYSICAL_DEVICE_ERROR on error
	*/
	VK_STATUS_CODE selectBestPhysicalDevice(void);

	/**
		Rates GPU for the operations that are about to be performed

		@param		device_		The VkPhysicalDevice to be rated

		@return		Returns an integer value as score
	*/
	int evaluateDeviceSuitabilityScore(VkPhysicalDevice device_);

	/**
		Prints information about GPU !!!NOT YET FULLY IMPLEMENTED!!!

		@param		device_		The VkPhysicalDevice to print information about

		@return		Returns VK_SC_SUCCESS on success
	*/
	VK_STATUS_CODE printPhysicalDevicePropertiesAndFeatures(VkPhysicalDevice device_);

	/**
		Finds queue families that are suitable for the operations that are about to be performed on them
	
		@param		device_		A valid VkPhysicalDevice handle whose queue families are to be tested

		@return		Returns a QueueFamily struct
	*/
	QueueFamily findSuitableQueueFamilies(VkPhysicalDevice device_);

	/**
		Creates a VkDevice handle from a VkPhysicalDevice (class member)

		@return		Returns VK_SC_SUCCESS on success
		@return		Returns VK_SC_LOGICAL_DEVICE_ERROR on error
	*/
	VK_STATUS_CODE createLogicalDeviceFromPhysicalDevice(void);

	/**
		Creates a VkSurfaceKHR handle via GLFW

		@return		Returns VK_SC_SUCCESS on success
	*/
	VK_STATUS_CODE createSurfaceGLFW(void);

};
