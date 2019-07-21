/**
    Defines the TEXTURE_TYPE struct

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         TEXTURE_TYPE.cpp
    @brief        Definition of the TEXTURE_TYPE struct
*/
#pragma once


typedef enum TEXTURE_TYPE {

    TT_HEIGHT       = 4,
    TT_NORMAL       = 3,
    TT_SPECULAR     = 2,
    TT_DIFFUSE      = 1,
    TT_STANDARD     = 0

} TEXTURE_TYPE;