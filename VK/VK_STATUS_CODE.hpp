/**
	Implements the VK_STATUS_CODE enumeration

	@author		D3PSI
	@version	0.0.1 02.12.2019

	@file		VK_STATUS_CODE.hpp
	@brief		Implementation of the VK_STATUS_CODE enumeration

*/
#pragma once

/**
	Status code enumeration for easier exception-backtracing
*/
typedef enum VK_STATUS_CODE {

	VK_SC_CLEANUP_ERROR				= -6,
	VK_SC_VULKAN_RUNTIME_ERROR		= -5,
	VK_SC_VULKAN_ERROR				= -4,
	VK_SC_WINDOW_ERROR				= -3,
	VK_SC_LOGGER_ERROR				= -2,
	VK_SC_UKNOWN_ERROR				= -1,
	VK_SC_SUCCESS					= 0

} VK_STATUS_CODE;