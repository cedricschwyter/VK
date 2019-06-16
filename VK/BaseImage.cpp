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

    vkDestroyImageView(vk::engine.logicalDevice, imgView, vk::engine.allocator);
    vkDestroyImage(vk::engine.logicalDevice, img, vk::engine.allocator);
    vkFreeMemory(vk::engine.logicalDevice, imgMem, vk::engine.allocator);

}