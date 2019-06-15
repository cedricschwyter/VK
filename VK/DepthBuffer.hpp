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
    DepthBuffer() = default;

private:

    VkImage             img;
    VkDeviceMemory      imgMem;
    VkImageView         imgView;

};

