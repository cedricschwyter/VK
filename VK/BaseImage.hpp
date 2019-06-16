/**
    Defines the BaseImage class

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         BaseImage.hpp
    @brief        Implementation of the BaseImage class
*/
#pragma once
#include <vulkan/vulkan.h>

#include "VK_STATUS_CODE.hpp"
#include "BaseBuffer.hpp"

class BaseImage : 
    public BaseBuffer
{
public:

    /**
        Default constructor
    */
    BaseImage(void) = default;

    /**
        Default destructor
    */
    ~BaseImage(void) = default;

protected:

    /**
        Creates a VkImage handle

        @param      width_              The width of the image
        @param      height_             The height of the image
        @param      format_             The image format
        @param      tiling_             The image tiling flags
        @param      usage_              The image usage flags
        @param      properties_         Image memory property flags
        @param      image_              The handle where the image will be stored
        @param      imageMemory_        The handle where the image's device memory will be stored

        @return     Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE createImage(
        uint32_t                    width_,
        uint32_t                    height_,
        VkFormat                    format_,
        VkImageTiling               tiling_,
        VkImageUsageFlags           usage_,
        VkMemoryPropertyFlags       properties_,
        VkImage&                    image_,
        VkDeviceMemory&             imageMemory_
        );

};

