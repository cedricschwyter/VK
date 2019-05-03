/**
	Sets build-options by defining macros

	@author		D3PSI
	@version	0.0.1 02.12.2019

	@file		Version.hpp
	@brief		Set build-options by defining macros
*/
#pragma once

// Edit configuration here
#define VK_DEVELOPMENT		// enable this for verbose output
//#define VK_RELEASE			// enable this for performance

// Default values
#if !defined VK_DEVELOPMENT && !defined VK_RELEASE
	#define VK_DEVELOPMENT
#endif
