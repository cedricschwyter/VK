/**
    Implements the ImageObject class

    @author		D3PSI
    @version	0.0.1 02.12.2019

    @file		ImageObject.cpp
    @brief		Implementation of the ImageObject class
*/
#include "ImageObject.hpp"

#include <stb_image.h>

#include "VK.hpp"

ImageObject::ImageObject(const char* path_) {

    stbi_uc* pix = stbi_load(
        "res/textures/application/vulkan.png",
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
    imageCreateInfo.extent.width            = static_cast< uint32_t >(w);
    imageCreateInfo.extent.height           = static_cast< uint32_t >(h);
    imageCreateInfo.extent.depth            = 1;                            // As I am only using 2D-textures...
    imageCreateInfo.mipLevels               = 1;                            // No mipmapping yet
    imageCreateInfo.arrayLayers             = 1;
    imageCreateInfo.format                  = VK_FORMAT_R8G8B8A8_UNORM;     // Should be supported in 99% of cases...
    imageCreateInfo.tiling                  = VK_IMAGE_TILING_OPTIMAL;
    imageCreateInfo.initialLayout           = VK_IMAGE_LAYOUT_UNDEFINED;
    imageCreateInfo.usage                   = VK_SHARING_MODE_EXCLUSIVE;
    imageCreateInfo.samples                 = VK_SAMPLE_COUNT_1_BIT;        // No multisampling yet
   
    vk::engine.result = vkCreateImage(
        vk::engine.logicalDevice, 
        &imageCreateInfo, 
        vk::engine.allocator, 
        &img
        );
    ASSERT(vk::engine.result, "Failed to create image", VK_SC_TEXTURE_IMAGE_CREATION_ERROR);

    VkMemoryRequirements memoryRequirements;
    vkGetImageMemoryRequirements(vk::engine.logicalDevice, img, &memoryRequirements);

    VkMemoryAllocateInfo memoryAllocateInfo             = {};
    memoryAllocateInfo.sType                            = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.allocationSize                   = memoryRequirements.size;
    memoryAllocateInfo.memoryTypeIndex                  = enumerateSuitableMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    vk::engine.result = vkAllocateMemory(
        vk::engine.logicalDevice,
        &memoryAllocateInfo,
        vk::engine.allocator,
        &mem
        );
    ASSERT(vk::engine.result, "Failed to allocate buffer memory", VK_SC_BUFFER_ALLOCATION_ERROR);

    bind();

}

VK_STATUS_CODE ImageObject::bind() {

    vk::engine.result = vkBindImageMemory(
        vk::engine.logicalDevice,
        img,
        mem,
        0
        );
    ASSERT(vk::engine.result, "Failed to bind buffer memory", VK_SC_BUFFER_BINDING_ERROR);

    return VK_SC_SUCCESS;

}

ImageObject::~ImageObject() {



}
