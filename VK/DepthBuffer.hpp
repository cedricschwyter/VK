/**
    Defines the DepthBuffer class, inheriting BaseBuffer

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         DepthBuffer.cpp
    @brief        Definition of the DepthBuffer class
*/
#pragma once
#include "BaseImage.hpp"

class DepthBuffer :
    public BaseImage
{
public:

    VkFormat            depthFormat;
    VkImageView         imgView;

    /**
        Default constructor
    */
    DepthBuffer(void);

    /**
        Default destructor
    */
    ~DepthBuffer(void);

private:

    VkImage             img;
    VkDeviceMemory      imgMem;

    /**
        Evaluates the best depth buffer format to use

        @return     Returns the best depth buffer format to use
    */
    VkFormat enumerateSupportedDepthBufferFormat(void);

};
