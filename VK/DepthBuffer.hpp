/**
    Defines the DepthBuffer class, inheriting BaseBuffer

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         DepthBuffer.cpp
    @brief        Definition of the DepthBuffer class
*/
#ifndef DEPTH_BUFFER_HPP
#define DEPTH_BUFFER_HPP
#include "BaseImage.hpp"

class DepthBuffer :
    public BaseImage
{
public:

    /**
        Default constructor
    */
    DepthBuffer(void);

    /**
        Default destructor
    */
    ~DepthBuffer(void) = default;

private:

    /**
        Evaluates the best depth buffer format to use

        @return     Returns the best depth buffer format to use
    */
    VkFormat enumerateSupportedDepthBufferFormat(void);

};
#endif  // DEPTH_BUFFER_HPP
