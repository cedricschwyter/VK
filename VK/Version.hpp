/**
    Sets build-options by defining macros

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         Version.hpp
    @brief        Set build-options by defining macros
*/
#ifndef VERSION_HPP
#define VERSION_HPP
#include "Makros.hpp"

// Edit configuration here
#define VK_DEVELOPMENT      // enable this for verbose output
//#define VK_RELEASE          // enable this for performance
//#define VK_RELEASE_CONSOLE    // enbable this for performance with console
//#define VK_NO_LOG             // enable this for full performance

#define VK_WINDOW_MODE_WINDOWED
//#define VK_WINDOW_MODE_FULLSCREEN
//#define VK_WINDOW_MODE_BORDERLESS

//#define WIN_64  
//#define WIN_32
//#define MACOSX
#define LINUX 

//#define VK_STANDARD_MODEL_LOADING_LIB VKEngineModelLoadingLibTINYOBJ
#define VK_STANDARD_MODEL_LOADING_LIB VKEngineModelLoadingLibASSIMP

//#define VK_MULTISAMPLING_NONE
//#define VK_MULTISAMPLING_x2
//#define VK_MULTISAMPLING_x4
//#define VK_MULTISAMPLING_x8
//#define VK_MULTISAMPLING_x16
//#define VK_MULTISAMPLING_x32
//#define VK_MULTISAMPLING_x64

//#define VK_VERTEX_DEDUPLICATION       // Toggle Vertex Deduplication, may be buggy

// Default values

#ifdef VK_NO_LOG
    #define VK_RELEASE
#endif

#if !defined VK_DEVELOPMENT && !defined VK_RELEASE && !defined VK_RELEASE_CONSOLE
    #define VK_DEVELOPMENT
#endif

#if !defined VK_WINDOW_MODE_WINDOWED && !defined VK_WINDOW_MODE_FULLSCREEN && !defined VK_WINDOW_MODE_BORDERLESS
    #define VK_WINDOW_MODE_BORDERLESS
#endif

#ifdef VK_RELEASE_CONSOLE
    #define VK_RELEASE
#endif

#if !defined VK_DEVELOPMENT && !defined VK_RELEASE_CONSOLE && defined VK_RELEASE && (defined WIN_64 || defined WIN_32)
    #pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#ifndef NOMINMAX
    #if defined WIN_64 || defined WIN_32
        #define NOMINMAX        // Fixes std::min/std::max/std::clamp error
        #include <Windows.h>
    #endif
#endif  // NOMINMAX
#endif  // VERSION_HPP
