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

    vkDestroyImageView(vk::core::logicalDevice, imgView, vk::core::allocator);
    vkDestroyImage(vk::core::logicalDevice, img, vk::core::allocator);
    vkFreeMemory(vk::core::logicalDevice, imgMem, vk::core::allocator);

}