/**
    Implements the TextureImage class

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         TextureImage.cpp
    @brief        Implementation of the TextureImage class
*/
#include "TextureImage.hpp"

#include <stb_image.h>

#include "VK.hpp"
#include "ASSERT.cpp"


TextureImage::TextureImage(
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

    mipLevels = static_cast< uint32_t >(std::floor(std::log2(std::max(w, h)))) + 1;     // Calculate number of mipmaps by using logarithms

    if (!pix) {

        logger::log(ERROR_LOG, "Failed to load textures");

    }

    VkFormat imageFormat;
    if (ch == 1) {

        imageFormat = VK_FORMAT_R8_UNORM;
    
    }
    else if (ch == 3) {

        // TODO: Reshape data in memory to fit in 4-channel format as 3-channel formats are not supported (mostly) for beauty

        imageFormat = VK_FORMAT_R8G8B8A8_UNORM;
        
    }
    else if (ch == 4) {

        imageFormat = VK_FORMAT_R8G8B8A8_UNORM;
    
    }
    else {
        
        logger::log(ERROR_LOG, "Unsupported image format at " + std::string(path_));
    
    }

    stagingBuffer = new BaseBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    stagingBuffer->fill(pix);

    stbi_image_free(pix);

    vk::createImage(
        w, 
        h, 
        mipLevels, 
        imageFormat, 
        VK_IMAGE_TILING_OPTIMAL, 
        VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, 
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        VK_SAMPLE_COUNT_1_BIT,
        img,
        mem
        );

    vk::imageLayoutTransition(
        img,
        imageFormat, 
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        mipLevels
        );

    vk::copyBufferToImage(
        stagingBuffer->buf,
        img,
        static_cast< uint32_t >(w),
        static_cast< uint32_t >(h)
        );
    
    /*vk::imageLayoutTransition(
        img,
        imageFormat, 
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        mipLevels
        );*/

        vk::generateImageMipmaps(
            img, 
            imageFormat,
            w, 
            h, 
            mipLevels
            );

    delete stagingBuffer;

    imgView = vk::createImageView(img, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT, 1);

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
    samplerCreateInfo.maxLod                        = mipLevels;

    VkResult result = vkCreateSampler(
        vk::engine->logicalDevice,
        &samplerCreateInfo,
        vk::engine->allocator,
        &imgSampler
        );
    ASSERT(result, "Failed to create sampler", VK_SC_SAMPLER_CREATION_ERROR);

    logger::log(EVENT_LOG, "Successfully created sampler");

}

VK_STATUS_CODE TextureImage::bind() {

    VkResult result = vkBindImageMemory(
        vk::engine->logicalDevice,
        img,
        mem,
        0
        );
    ASSERT(result, "Failed to bind buffer memory", VK_SC_BUFFER_BINDING_ERROR);

    return vk::errorCodeBuffer;

}

TextureImage::~TextureImage() {

    vkDestroySampler(vk::engine->logicalDevice, imgSampler, vk::engine->allocator);
    vkDestroyImageView(vk::engine->logicalDevice, imgView, vk::engine->allocator);
    vkDestroyImage(vk::engine->logicalDevice, img, vk::engine->allocator);

}
