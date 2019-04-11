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

/**
	Implements a namespace to prevent global naming conflicts
*/
namespace vk {

	/**
		Handles main initialization of everything

		@return		Returns VK_SC_SUCCESS on success
		@return		Returns VK_SC_UNKNOWN_ERROR on error
	*/
	VK_STATUS_CODE init(void);

}

namespace vk {

	VKEngine engine;

	VK_STATUS_CODE init() {
	
		try {

			return engine.init();
		
		} 
		catch (const std::exception& e) {

			std::cerr << e.what() << std::endl;

			return VK_SC_UKNOWN_ERROR;

		}
	
	}

}

/**
	Entry point for the application
*/
int main() {

	return vk::init();

}