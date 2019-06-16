/**
    Implements the DepthBuffer class, inheriting BaseBuffer

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         DepthBuffer.cpp
    @brief        Implementation of the DepthBuffer class
*/
#include "DepthBuffer.hpp"
#include "VK.hpp"


DepthBuffer::DepthBuffer() {

    depthFormat     = enumerateSupportedDepthBufferFormat();
    createImage(
        vk::engine.swapchainImageExtent.width, 
        vk::engine.swapchainImageExtent.height, 
        depthFormat, 
        VK_IMAGE_TILING_OPTIMAL, 
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, 
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
        img, 
        imgMem
        );
    imgView         = vk::createImageView(img, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
    vk::imageLayoutTransition(img, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

}

VkFormat DepthBuffer::enumerateSupportedDepthBufferFormat() {

    return vk::enumerateSupportedBufferFormat(
        { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
        );

}

DepthBuffer::~DepthBuffer() {

    vkDestroyImageView(vk::engine.logicalDevice, imgView, vk::engine.allocator);
    vkDestroyImage(vk::engine.logicalDevice, img, vk::engine.allocator);
    vkFreeMemory(vk::engine.logicalDevice, imgMem, vk::engine.allocator);

}