/**
    Implements the LOG_TYPE enumeration

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         LOG_TYPE.hpp
    @brief        Implementation of the LOG_TYPE enumeration

*/
#ifndef LOG_TYPE_CPP
#define LOG_TYPE_CPP

/**
 * Enumeration to differenciate between the different types of .log-files and streams
 */
typedef enum LOG_TYPE {

    ERROR_LOG,
    START_LOG,
    EVENT_LOG

} LOG_TYPE;
#endif  // LOG_TYPE_CPP
