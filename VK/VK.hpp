/**
	Prototypes the vk namespace

	@author		D3PSI
	@version	0.0.1 02.12.2019

	@file		VK.hpp
	@brief		Prototype of the vk namespace
*/
#pragma once
#include <vulkan/vulkan.h>

#include "VKEngine.hpp"
#include "VK_STATUS_CODE.hpp"

/**
	Prototypes a namespace to prevent global naming conflicts
*/
namespace vk {

	extern VKEngine								engine;
	extern const unsigned int					HEIGHT;
	extern const unsigned int					WIDTH;
	extern const char*							TITLE;
	extern const unsigned int					MAX_IN_FLIGHT_FRAMES;
	extern const std::vector< BaseVertex >		vertices;

	/**
		Handles main initialization of everything

		@return		Returns VK_SC_SUCCESS on success
		@return		Returns VK_SC_UNKNOWN_ERROR on error
	*/
	VK_STATUS_CODE init(void);

	/**
		Helper function to create a VkDebugUtilsMessengerEXT

		@param		instance_				The valid VkInstance handle
		@param		pCreateInfo_			Pointer to messenger create info
		@param		pAllocator_				Pointer to memory allocator
		@param		pDebugMessenger_		The handle in which the messenger will be stored

		@return		Returns VK_SUCCESS on success
		@return		Returns VK_ERROR_EXTENSION NOT PRESENT if the "VK_EXT_debug_utils" extension has not been enabled
	*/
	VkResult createDebugUtilsMessenger(
		VkInstance										instance_,
		const VkDebugUtilsMessengerCreateInfoEXT*		pCreateInfo_,
		const VkAllocationCallbacks*					pAllocator_,
		VkDebugUtilsMessengerEXT*						pDebugMessenger_
	);

	/**
		Helper function to destroy a VkDebugUtilsMessengerEXT

		@param		instance_				The valid VkInstance handle
		@param		debugMessenger_			The debug messenger handle
		@param		pAllocator_				Pointer to memory allocator

		@return		Returns VK_SC_SUCCESS on success
		@return		Returns VK_SC_DEBUG_UTILS_MESSENGER_CLEANUP_ERROR on error
	*/
	VK_STATUS_CODE destroyDebugUtilsMessenger(
		VkInstance							instance_,
		VkDebugUtilsMessengerEXT			debugMessenger_,
		const VkAllocationCallbacks*		pAllocator_
	);

	/**
		Makes the console invisible
	*/
	void hideConsole(void);
	
	/**
		Makes the console visible
	*/
	void showConsole(void);
	
	/**
		Checks whether the console window is visible at the moment

		@return		Returns true if the console window is visible
		@return		Returns false if the console window is not visible
	*/
	bool isConsoleVisible(void);

	/**
		Loads a file from disc and returns its contents in a char array

		@param		filePath_		The (relative) file path to the desired file

		@return		Returns an std::vector< char > containing the binary content of the specified input file
	*/
	const std::vector< char > loadFile(const std::string& filePath_);

}