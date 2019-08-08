/**
    Defines the TextureImage class

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         TextureImage.hpp
    @brief        Definition of the TextureImage class
*/
#ifndef TEXTURE_IMAGE_HPP
#define TEXTURE_IMAGE_HPP
#include <vulkan/vulkan.h>

#include "Logger.hpp"
#include "BaseBuffer.hpp"

class TextureImage : 
    public BaseBuffer
{
public:

    VkImageView         imgView;
    VkSampler           imgSampler;
    uint32_t            mipLevels;

    /**
        Default constructor
    */
    TextureImage(void) = default;

    /**
        Constructor

        @param      path_           (Relative) filepath to image resource
        @param      format_         The image format flags
        @param      tiling_         Image tiling flags
        @param      usage_          Image usage flags
        @param      properties_     Memory properties, defaults to VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    */
    TextureImage(
        const char*                 path_,
        VkFormat                    format_,
        VkImageTiling               tiling_,
        VkImageUsageFlags           usage_,
        VkMemoryPropertyFlags       properties_      = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

    /**
        Binds an image

        @return     Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE bind(void);

    /**
        Default destructor
    */
    ~TextureImage(void);

private:

    int                 w, h, ch;
    VkDeviceSize        imageSize               = 0;
    BaseBuffer*         stagingBuffer;
    VkImage             img;

};
#endif  // TEXTURE_IMAGE_HPP
