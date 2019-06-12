/**
    Sets build-options by defining macros

    @author        D3PSI
    @version    0.0.1 02.12.2019

    @file        Version.hpp
    @brief        Set build-options by defining macros
*/
#pragma once

// Edit configuration here
//#define VK_DEVELOPMENT      // enable this for verbose output
//#define VK_RELEASE          // enable this for performance

#define WIN_64                // Windows 64-bit
//#define WIN_32              // Windows 32-bit
//#define MACOSX              // macOS X 64-bit
//#define LINUX               // Linux 64-bit

// Default values
#if !defined VK_DEVELOPMENT && !defined VK_RELEASE
    #define VK_DEVELOPMENT
#endif

#if !defined WIN_64 && !defined WIN_32 && !defined MACOSX && !defined LINUX
    #define WIN_64
#endif
