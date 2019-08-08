/**
    Defines the BaseImage class

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         BaseImage.hpp
    @brief        Implementation of the BaseImage class
*/
#ifndef BASE_IMAGE_HPP
#define BASE_IMAGE_HPP
#include <vulkan/vulkan.h>

#include "VK_STATUS_CODE.hpp"
#include "BaseBuffer.hpp"

class BaseImage : 
    public BaseBuffer
{
public:

    VkFormat            imgFormat;
    VkImageView         imgView;

    /**
        Default constructor
    */
    BaseImage(void) = default;
    
    /**
        Default destructor
    */
    ~BaseImage(void);

protected:

    VkImage             img;
    VkDeviceMemory      imgMem;


};
#endif  // BASE_IMAGE_HPP
