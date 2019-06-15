/**
    Defines the DepthBuffer class, inheriting BaseBuffer

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         DepthBuffer.cpp
    @brief        Definition of the DepthBuffer class
*/
#pragma once
#include "BaseBuffer.hpp"

class DepthBuffer :
    public BaseBuffer
{
public:

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
    VkImageView         imgView;

    /**
        Evaluates the best depth buffer format to use

        @return     Returns the best depth buffer format to use
    */
    VkFormat enumerateSupportedDepthBufferFormat(void);

};

