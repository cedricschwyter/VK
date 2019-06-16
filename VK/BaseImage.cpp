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


VK_STATUS_CODE BaseImage::createImage(
    uint32_t                    width_,
    uint32_t                    height_,
    uint32_t                    mipLevels_,
    VkFormat                    format_,
    VkImageTiling               tiling_,
    VkImageUsageFlags           usage_,
    VkMemoryPropertyFlags       properties_,
    VkSampleCountFlagBits       samples_
    ) {

    VkImageCreateInfo imgCreateInfo         = {};
    imgCreateInfo.sType                     = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imgCreateInfo.imageType                 = VK_IMAGE_TYPE_2D;
    imgCreateInfo.extent.width              = width_;
    imgCreateInfo.extent.height             = height_;
    imgCreateInfo.extent.depth              = 1;
    imgCreateInfo.mipLevels                 = mipLevels_;
    imgCreateInfo.arrayLayers               = 1;
    imgCreateInfo.format                    = format_;
    imgCreateInfo.tiling                    = tiling_;
    imgCreateInfo.initialLayout             = VK_IMAGE_LAYOUT_UNDEFINED;
    imgCreateInfo.usage                     = usage_;
    imgCreateInfo.samples                   = samples_;
    imgCreateInfo.sharingMode               = VK_SHARING_MODE_EXCLUSIVE;
    
    VkResult result = vkCreateImage(
        vk::engine.logicalDevice,
        &imgCreateInfo,
        vk::engine.allocator,
        &img
        );
    ASSERT(result, "Failed to create image from the given parameters", VK_SC_IMAGE_CREATION_ERROR);

    VkMemoryRequirements memReqs;
    vkGetImageMemoryRequirements(vk::engine.logicalDevice, img, &memReqs);

    VkMemoryAllocateInfo memoryAllocInfo    = {};
    memoryAllocInfo.sType                   = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocInfo.allocationSize          = memReqs.size;
    memoryAllocInfo.memoryTypeIndex         = enumerateSuitableMemoryType(memReqs.memoryTypeBits, properties_);

    result = vkAllocateMemory(
        vk::engine.logicalDevice, 
        &memoryAllocInfo, 
        vk::engine.allocator, 
        &imgMem
        );
    ASSERT(result, "Failed to allocate image memory", VK_SC_IMAGE_MEMORY_ALLOCATION_ERROR);

    vkBindImageMemory(
        vk::engine.logicalDevice,
        img,
        imgMem,
        0
        );

    return vk::errorCodeBuffer;

}

BaseImage::~BaseImage() {

    vkDestroyImageView(vk::engine.logicalDevice, imgView, vk::engine.allocator);
    vkDestroyImage(vk::engine.logicalDevice, img, vk::engine.allocator);
    vkFreeMemory(vk::engine.logicalDevice, imgMem, vk::engine.allocator);

}