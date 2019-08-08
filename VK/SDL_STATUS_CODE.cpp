/**
    Implements the SDL_STATUS_CODE enumeration

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         SDL_STATUS_CODE.hpp
    @brief        Implementation of the SDL_STATUS_CODE enumeration

*/
#ifndef SDL_STATUS_CODE_CPP
#define SDL_STATUS_CODE_CPP

/**
    Status code enumeration for easier exception-backtracing
*/
typedef enum SDL_STATUS_CODE {

    SDL_SC_UNKNOWN_ERROR        = -1,
    SDL_SC_SUCCESS              = 0

} SDL_STATUS_CODE;
#endif  // SDL_STATUS_CODE_CPP
