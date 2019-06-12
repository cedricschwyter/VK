/**
    Prototypes the Logger namespace

    @author        D3PSI
    @version    0.0.1 02.12.2019

    @file        Logger.hpp
    @brief        Prototype of the Logger namespace
*/
#pragma once
#include <mutex>

#include "Version.hpp"
#include "LOGGER_STATUS_CODE.cpp"
#include "LOG_TYPE.cpp"

/**
    Prototypes the logger namespace
*/
namespace logger {

    /**
        Initializes logger and creates necessary directories

        @return        Returns LOGGER_SC_SUCCESS on success
        @return        Returns LOGGER_SC_UNKNOWN_ERROR on error
    */
    LOGGER_STATUS_CODE init(void);

    /**
        Writes to specified log and prints message to console

        @param        log_    Specify log-file to write to
        @param        msg_    The actual text to write and print

        @return        Returns LOGGER_SC_SUCCESS on success
    */
    LOGGER_STATUS_CODE log(LOG_TYPE log_, const char* msg_);

    /**
        Writes to specified log and prints message to console

        @param        log_    Specify log-file to write to
        @param        msg_    The actual text to write and print

        @return        Returns LOGGER_SC_SUCCESS on success
    */
    LOGGER_STATUS_CODE log(LOG_TYPE log_, std::string msg_);

}