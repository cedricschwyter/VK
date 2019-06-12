/**
    Implements the LOGGER_STATUS_CODE enumeration

    @author        D3PSI
    @version    0.0.1 02.12.2019

    @file        LOGGER_STATUS_CODE.hpp
    @brief        Implementation of the LOGGER_STATUS_CODE enumeration

*/
#pragma once

/**
    Status code enumeration for easier exception-backtracing
*/
typedef enum LOGGER_STATUS_CODE {

    LOGGER_SC_DIRECTORY_CREATION_ERROR        = -2,
    LOGGER_SC_UNKNOWN_ERROR                   = -1,
    LOGGER_SC_SUCCESS                         = 0

} LOGGER_STATUS_CODE;