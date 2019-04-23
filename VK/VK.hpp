#pragma once
#include <vulkan/vulkan.h>

#include "VKEngine.hpp"
#include "VK_STATUS_CODE.hpp"

/**
	Prototypes a namespace to prevent global naming conflicts
*/
namespace vk {

	extern VKEngine engine;

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

}