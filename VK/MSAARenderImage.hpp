/**
    Implements the MSAARenderImage class

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         MSAARenderImage.cpp
    @brief        Implementation of the MSAARenderImage class
*/
#ifndef MSAA_RENDER_IMAGE_HPP
#define MSAA_RENDER_IMAGE_HPP
#include "BaseImage.hpp"

class MSAARenderImage :
    public BaseImage
{
public:

    /**
        Default constructor
    */
    MSAARenderImage(void);

    /**
        Default destructor
    */
    ~MSAARenderImage(void);

};
#endif  // MSAA_RENDER_IMAGE_HPP
