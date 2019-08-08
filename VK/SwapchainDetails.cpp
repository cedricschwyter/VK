/**
    Defines a struct to hold additional information about the swapchain

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         Main.cpp
    @brief        Swapchain details structure definition
*/
#ifndef SWAPCHAIN_DETAILS_CPP
#define SWAPCHAIN_DETAILS_CPP
#include <vulkan/vulkan.h>

#include <vector>

/**
    Holds information about the swapchain
*/
struct SwapchainDetails {

    VkSurfaceCapabilitiesKHR                surfaceCapabilities;
    std::vector< VkSurfaceFormatKHR >        supportedFormats;
    std::vector< VkPresentModeKHR >            presentationModes;

};
#endif  // SWAPCHAIN_DETAILS_CPP
