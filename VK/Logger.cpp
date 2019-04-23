/**
	Implements the Logger namespace

	@author		D3PSI
	@version	0.0.1 02.12.2019

	@file		Logger.cpp
	@brief		Implementation of the Logger namespace
*/
#pragma once
#include <direct.h>
#include <fstream>
#include <time.h>
#include <iostream>
#include <string>

#include "Logger.hpp"
#include "ConsoleColor.hpp"

namespace logger {

	const char*				LOG_DIR				= "logs";

	const std::string		ERROR_LOG_PATH		= "logs/error.log";
	const std::string		START_LOG_PATH		= "logs/start.log";
	const std::string		EVENT_LOG_PATH		= "logs/event.log";

	LOGGER_STATUS_CODE init() {
	
		_mkdir(LOG_DIR);
		std::ofstream error;
		std::ofstream start;
		std::ofstream event;
		error.open(ERROR_LOG_PATH, std::ios::trunc);
		error.close();
		start.open(START_LOG_PATH, std::ios::app);
		start.close();
		event.open(EVENT_LOG_PATH, std::ios::trunc);
		logger::log(EVENT_LOG, "Successfully initialized Logger");

		return LOGGER_SC_SUCCESS;

	}

	LOGGER_STATUS_CODE log(LOG_TYPE log_, const char* msg_) {
	
		static int countEvent = 0;
		static int countError = 0;
		std::ofstream stream;

		time_t current_time;
		struct tm local_time;

		time(&current_time);
		localtime_s(&local_time, &current_time);

		int Year		= local_time.tm_year + 1900;
		int Month		= local_time.tm_mon + 1;
		int Day			= local_time.tm_mday;

		int Hour		= local_time.tm_hour;
		int Min			= local_time.tm_min;
		int Sec			= local_time.tm_sec;

		switch (log_) {
		case ERROR_LOG:
			if (countError == 0) {

				stream.open(ERROR_LOG_PATH, std::ios::trunc);
				countError++;

				stream << green << Day << white << ":"
					<< green << Month << white << ":"
					<< green << Year << white << "   "
					<< green << Hour << white << ":"
					<< green << Min << white << ":"
					<< green << Sec << white << "		===		"
					<< red << "CRITICAL: "
					<< blue << msg_ << white << std::endl;

				stream.close();

				std::cerr << green << Day << white << ":"
					<< green << Month << white << ":"
					<< green << Year << white << "   "
					<< green << Hour << white << ":"
					<< green << Min << white << ":"
					<< green << Sec << white << "		===		"
					<< red << "CRITICAL: "
					<< blue << msg_ << white << std::endl;


			}
			else {

				stream.open(ERROR_LOG_PATH, std::ios::app);

				stream << green << Day << white << ":"
					<< green << Month << white << ":"
					<< green << Year << white << "   "
					<< green << Hour << white << ":"
					<< green << Min << white << ":"
					<< green << Sec << white << "		===		"
					<< red << "CRITICAL: "
					<< blue << msg_ << white << std::endl;

				std::cerr << green << Day << white << ":"
					<< green << Month << white << ":"
					<< green << Year << white << "   "
					<< green << Hour << white << ":"
					<< green << Min << white << ":"
					<< green << Sec << white << "		===		"
					<< red << "CRITICAL: "
					<< blue << msg_ << white << std::endl;

				stream.close();

			}
		case START_LOG:
			stream.open(START_LOG_PATH, std::ios::app);

			stream << green << Day << white << ":"
				<< green << Month << white << ":"
				<< green << Year << white << "   "
				<< green << Hour << white << ":"
				<< green << Min << white << ":"
				<< green << Sec << white << "		===		"
				<< blue << msg_ << white << std::endl;

			stream.close();

		case EVENT_LOG:
			if (countEvent == 0) {

				stream.open(EVENT_LOG_PATH, std::ios::trunc);
				countEvent++;

				stream << green << Day << white << ":"
					<< green << Month << white << ":"
					<< green << Year << white << "   "
					<< green << Hour << white << ":"
					<< green << Min << white << ":"
					<< green << Sec << white << "		===		"
					<< blue << msg_ << white << std::endl;

				stream.close();

				std::cout << green << Day << white << ":"
					<< green << Month << white << ":"
					<< green << Year << white << "   "
					<< green << Hour << white << ":"
					<< green << Min << white << ":"
					<< green << Sec << white << "		===		"
					<< blue << msg_ << white << std::endl;

			}
			else {

				stream.open(EVENT_LOG_PATH, std::ios::app);

				stream << green << Day << white << ":"
					<< green << Month << white << ":"
					<< green << Year << white << "   "
					<< green << Hour << white << ":"
					<< green << Min << white << ":"
					<< green << Sec << white << "		===		"
					<< blue << msg_ << white << std::endl;

				stream.close();

				std::cout << green << Day << white << ":"
					<< green << Month << white << ":"
					<< green << Year << white << "   "
					<< green << Hour << white << ":"
					<< green << Min << white << ":"
					<< green << Sec << white << "		===		"
					<< blue << msg_ << white << std::endl;

			}
			break;

		default:
			break;

		}

		if (log_ == ERROR_LOG) {

			throw std::runtime_error(msg_);

		}

		return LOGGER_SC_SUCCESS;

	}

	LOGGER_STATUS_CODE log(LOG_TYPE log_, std::string msg_) {
	
		return logger::log(log_, msg_.c_str());

	}

}