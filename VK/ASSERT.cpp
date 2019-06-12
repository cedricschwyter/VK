/**
    Implements the ASSERT function

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         ASSERT.cpp
    @brief        Implementation of the ASSERT function
*/
#pragma once
#include <iostream>

#include "Logger.hpp"

/**
    Checks functions for a returncode other than 0 and throws an error

    @param         val_    Value to be tested
    @param         msg_    Error message that will get logged
    @param         ret_    Return code

    @return        Returns ret_ if val_ is not 0
    @return        Returns val_ if val_ is 0
*/
inline int ASSERT(int val_, const char* msg_, int ret_) {    

    if (val_ != 0) {

        logger::log(ERROR_LOG, msg_);

        return ret_;

    }
    else {
    
        return val_;

    }

}
