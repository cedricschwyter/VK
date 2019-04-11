/**
	Declares the VKEngine class

	@author		D3PSI
	@version	0.0.1 02.12.2019

	@file		VKEngine.hpp
	@brief		Declaration of the VKEngine class
*/

#pragma once
#include <iostream>

#include "VK_STATUS_CODE.hpp"

class VKEngine {
public:

	/**
		Initializes VKEngine and loads dependencies

		@return		Returns VK_SC_SUCCESS on success
	*/
	VK_STATUS_CODE init(void);

private:

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

};

