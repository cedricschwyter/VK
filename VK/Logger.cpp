/**
	Implements the Logger namespace

	@author		D3PSI
	@version	0.0.1 02.12.2019

	@file		Logger.cpp
	@brief		Implementation of the Logger namespace
*/
#pragma once
#include <direct.h>

#include "Logger.hpp"

namespace logger {

	const char*		LOG_DIR				= "logs";

	const char*		ERROR_LOG_PATH		= "logs/error.log";
	const char*		START_LOG_PATH		= "logs/start.log";
	const char*		EVENT_LOG_PATH		= "logs/event.log";

	LOGGER_STATUS_CODE init() {
	
		_mkdir(LOG_DIR);

		return LOGGER_SC_SUCCESS;

	}

	LOGGER_STATUS_CODE log(LOG_TYPE log_, const char* msg_) {
	


	}

}