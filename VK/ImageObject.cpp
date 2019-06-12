/**
    Implements the ImageObject class

    @author        D3PSI
    @version    0.0.1 02.12.2019

    @file        ImageObject.cpp
    @brief        Implementation of the ImageObject class
*/
#include "ImageObject.hpp"

#include <stb_image.h>

#include "VK.hpp"

ImageObject::ImageObject(
    const char*                 path_, 
    VkFormat                    format_, 
    VkImageTiling               tiling_, 
    VkImageUsageFlags           usage_,
    VkMemoryPropertyFlags       properties_
    ) {

    stbi_uc* pix = stbi_load(
        path_,
        &w,
        &h,
        &ch,
        STBI_rgb_alpha
        );

    imageSize = w * h * 4;

    if (!pix) {

        logger::log(ERROR_LOG, "Failed to load textures");

    }

    stagingBuffer = new BaseBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    stagingBuffer->fill(pix);

    stbi_image_free(pix);

    VkImageCreateInfo imageCreateInfo       = {};
    imageCreateInfo.sType                   = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCreateInfo.imageType               = VK_IMAGE_TYPE_2D;
    imageCreateInfo.extent.width            = w;
    imageCreateInfo.extent.height           = h;
    imageCreateInfo.extent.depth            = 1;
    imageCreateInfo.mipLevels               = 1;
    imageCreateInfo.arrayLayers             = 1;
    imageCreateInfo.format                  = format_;
    imageCreateInfo.tiling                  = tiling_;
    imageCreateInfo.initialLayout           = VK_IMAGE_LAYOUT_UNDEFINED;
    imageCreateInfo.usage                   = usage_;
    imageCreateInfo.samples                 = VK_SAMPLE_COUNT_1_BIT;
    imageCreateInfo.sharingMode             = VK_SHARING_MODE_EXCLUSIVE;
   
    VkResult result = vkCreateImage(
        vk::engine.logicalDevice, 
        &imageCreateInfo, 
        vk::engine.allocator, 
        &img
        );
    ASSERT(result, "Failed to create image", VK_SC_TEXTURE_IMAGE_CREATION_ERROR);

    VkMemoryRequirements memoryRequirements;
    vkGetImageMemoryRequirements(vk::engine.logicalDevice, img, &memoryRequirements);

    VkMemoryAllocateInfo memoryAllocateInfo             = {};
    memoryAllocateInfo.sType                            = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.allocationSize                   = memoryRequirements.size;
    memoryAllocateInfo.memoryTypeIndex                  = enumerateSuitableMemoryType(memoryRequirements.memoryTypeBits, properties_);

    result = vkAllocateMemory(
        vk::engine.logicalDevice,
        &memoryAllocateInfo,
        vk::engine.allocator,
        &mem
        );
    ASSERT(result, "Failed to allocate buffer memory", VK_SC_BUFFER_ALLOCATION_ERROR);

    bind();

    vk::imageLayoutTransition(
        img,
        VK_FORMAT_R8G8B8A8_UNORM, 
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
        );

    vk::copyBufferToImage(
        stagingBuffer->buf,
        img,
        static_cast< uint32_t >(w),
        static_cast< uint32_t >(h)
        );

    vk::imageLayoutTransition(
        img,
        VK_FORMAT_R8G8B8A8_UNORM, 
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        );

    delete stagingBuffer;

    imgView = vk::createImageView(img, VK_FORMAT_R8G8B8A8_UNORM);

    VkSamplerCreateInfo samplerCreateInfo           = {};
    samplerCreateInfo.sType                         = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerCreateInfo.magFilter                     = VK_FILTER_LINEAR;
    samplerCreateInfo.minFilter                     = VK_FILTER_LINEAR;
    samplerCreateInfo.addressModeU                  = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerCreateInfo.addressModeV                  = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerCreateInfo.addressModeW                  = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerCreateInfo.anisotropyEnable              = VK_TRUE;
    samplerCreateInfo.maxAnisotropy                 = 16;
    samplerCreateInfo.borderColor                   = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerCreateInfo.unnormalizedCoordinates       = VK_FALSE;
    samplerCreateInfo.compareEnable                 = VK_FALSE;
    samplerCreateInfo.compareOp                     = VK_COMPARE_OP_ALWAYS;
    samplerCreateInfo.mipmapMode                    = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerCreateInfo.mipLodBias                    = 0.0f;
    samplerCreateInfo.minLod                        = 0.0f;
    samplerCreateInfo.maxLod                        = 0.0f;

    result = vkCreateSampler(
        vk::engine.logicalDevice,
        &samplerCreateInfo,
        vk::engine.allocator,
        &imgSampler
        );
    ASSERT(result, "Failed to create sampler", VK_SC_SAMPLER_CREATION_ERROR);

}

VK_STATUS_CODE ImageObject::bind() {

    VkResult result = vkBindImageMemory(
        vk::engine.logicalDevice,
        img,
        mem,
        0
        );
    ASSERT(result, "Failed to bind buffer memory", VK_SC_BUFFER_BINDING_ERROR);

    return VK_SC_SUCCESS;

}

ImageObject::~ImageObject() {

    vkDestroySampler(vk::engine.logicalDevice, imgSampler, vk::engine.allocator);
    vkDestroyImageView(vk::engine.logicalDevice, imgView, vk::engine.allocator);
    vkDestroyImage(vk::engine.logicalDevice, img, vk::engine.allocator);

}
