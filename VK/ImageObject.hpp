/**
    Defines the ImageObject class

    @author		D3PSI
    @version	0.0.1 02.12.2019

    @file		ImageObject.hpp
    @brief		Definition of the ImageObject class
*/
#pragma once
#include <vulkan/vulkan.h>

#include "Logger.hpp"
#include "BaseBuffer.hpp"

class ImageObject : 
    public BaseBuffer
{
public:

    /**
        Constructor

        @param      path_       (Relative) filepath to image resource
    */
    ImageObject(const char* path_);

    /**
        Binds an image

        @return     Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE bind(void);

    /**
        Default destructor
    */
    ~ImageObject(void);

private:

    int                 w, h, ch;
    VkDeviceSize        imageSize          = 0;

    BaseBuffer*         stagingBuffer;
    VkImage             img;

};

