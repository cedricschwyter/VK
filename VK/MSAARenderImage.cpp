/**
    Defines the MSAARenderImage class

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         MSAARenderImage.cpp
    @brief        Definition of the MSAARenderImage class
*/
#include "MSAARenderImage.hpp"
#include "VK.hpp"


MSAARenderImage::MSAARenderImage() {

    imgFormat = vk::engine->swapchainImageFormat;
    
    vk::createImage(
        vk::engine->swapchainImageExtent.width,
        vk::engine->swapchainImageExtent.height,
        1,
        imgFormat,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        vk::engine->maxMSAASamples,
        img,
        imgMem
        );
    
    imgView = vk::createImageView(
        img, 
        imgFormat, 
        VK_IMAGE_ASPECT_COLOR_BIT, 
        1
        );
    
    vk::imageLayoutTransition(
        img, 
        imgFormat, 
        VK_IMAGE_LAYOUT_UNDEFINED, 
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 
        1
        );

}

MSAARenderImage::~MSAARenderImage() {

    

}