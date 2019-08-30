/**
    Implements the BaseImage class

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         BaseImage.cpp
    @brief        Implementation of the BaseImage class
*/
#include "BaseImage.hpp"
#include "VK.hpp"
#include "ASSERT.cpp"


BaseImage::~BaseImage() {

    vkDestroyImageView(VKCore::logicalDevice, imgView, VKCore::allocator);
    vkDestroyImage(VKCore::logicalDevice, img, VKCore::allocator);
    vkFreeMemory(VKCore::logicalDevice, imgMem, VKCore::allocator);

}