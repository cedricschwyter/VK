/**
	Declares the VKEngine class

	@author		D3PSI
	@version	0.0.1 02.12.2019

	@file		VKEngine.hpp
	@brief		Declaration of the VKEngine class
*/

#pragma once
#include "VK_STATUS_CODE.hpp"

class VKEngine {
public:

	/**
		Initializes the application and loads dependencies

		@return		VK_STATUS_CODE	Returns VK_SC_SUCCESS on success
	*/
	VK_STATUS_CODE init(void);

};

