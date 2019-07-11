/**
    Implements the vk namespace

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         VK.cpp
    @brief        Prototype of the vk namespace
*/
#include "VK.hpp"
#include "ASSERT.cpp"


namespace vk {

    VK_STATUS_CODE                      errorCodeBuffer             = VK_SC_ERROR_CODE_BUFFER_NOT_INITIALIZED;

    VKEngine*                           engine;
    const unsigned int                  WIDTH                       = 1280;
    const unsigned int                  HEIGHT                      = 720;
    const char*                         TITLE                       = "VK by D3PSI";
    const unsigned int                  MAX_IN_FLIGHT_FRAMES        = 4;
    VkQueue                             transferQueue               = VK_NULL_HANDLE;
    VkCommandPool                       transferCommandPool         = VK_NULL_HANDLE;
    const double                        YAW                         = 0.0;
    const double                        PITCH                       = 0.0;
    const double                        ROLL                        = 0.0;
    const double                        SPEED                       = 2.0;
    const double                        SENS                        = 0.1;
    const double                        FOV                         = 45.0;

    const std::vector< BaseVertex >     vertices                    = {

        {{-0.5f, -0.5f,  0.0f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.0f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.0f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.0f}, {0.0f, 1.0f}},

        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}}

    };

    const std::vector< uint32_t >       indices                     = {

        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4

    };

    VK_STATUS_CODE init() {

        try {

            VK_STATUS_CODE* returnAddr;
            std::thread t0(&VKEngine::init, engine, returnAddr);
            t0.join();

            return *returnAddr;

        }
        catch (const std::exception& e) {

            std::cerr << e.what() << std::endl;

            return errorCodeBuffer;

        }

    }

    VkResult createDebugUtilsMessenger(
        VkInstance                                       instance_,
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo_,
        const VkAllocationCallbacks* pAllocator_,
        VkDebugUtilsMessengerEXT* pDebugMessenger_
        ) {

        logger::log(EVENT_LOG, "Gathering proc-address for 'vkCreateDebugUtilsMessengerEXT'");
        // Get proc-address from Vulkan
        auto function = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance_, "vkCreateDebugUtilsMessengerEXT");

        if (function != nullptr) {

            return function(
                instance_,
                pCreateInfo_,
                pAllocator_,
                pDebugMessenger_
                );

        }
        else {

            return VK_ERROR_EXTENSION_NOT_PRESENT;

        }

    }

    VK_STATUS_CODE destroyDebugUtilsMessenger(
        VkInstance                          instance_,
        VkDebugUtilsMessengerEXT            debugMessenger_,
        const VkAllocationCallbacks* pAllocator_
        ) {

        logger::log(EVENT_LOG, "Gathering proc-address for 'vkDestroyDebugUtilsMessengerEXT'");
        auto function = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance_, "vkDestroyDebugUtilsMessengerEXT");
        if (function != nullptr) {

            function(instance_, debugMessenger_, pAllocator_);
            return vk::errorCodeBuffer;

        }
        else {

            return VK_SC_DEBUG_UTILS_MESSENGER_CLEANUP_ERROR;

        }

    }

    void hideConsole() {
#if defined WIN_64 || defined WIN_32
        ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
#endif

    }

    void showConsole() {
#if defined WIN_64 || defined WIN_32
        ::ShowWindow(::GetConsoleWindow(), SW_SHOW);
#endif
    }

    bool isConsoleVisible() {
#if defined WIN_64 || defined WIN_32
        return ::IsWindowVisible(::GetConsoleWindow()) != FALSE;
#else
        return false;
#endif
    }

    const std::vector< char > loadFile(const std::string& filePath_) {

        logger::log(EVENT_LOG, "Loading file at '" + filePath_ + "'");

        std::ifstream file(filePath_, std::ios::ate | std::ios::binary);        // Start reading at end of file --> determine the buffer size needed

        if (!file.is_open()) {

            logger::log(ERROR_LOG, "Failed to load file at '" + filePath_ + "'");

        }

        size_t bufferSize = (size_t)file.tellg();        // Find read position and thus necessary buffer size
        std::vector< char > buffer(bufferSize);

        file.seekg(0);        // Translate back to the beginning of the file
        file.read(buffer.data(), bufferSize);
        file.close();

        return buffer;

    }

    void copyBuffer(VkBuffer srcBuf_, VkBuffer dstBuf_, VkDeviceSize size_) {

        VkCommandBufferAllocateInfo allocateInfo            = {};
        allocateInfo.sType                                  = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocateInfo.level                                  = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocateInfo.commandPool                            = transferCommandPool;
        allocateInfo.commandBufferCount                     = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(engine->logicalDevice, &allocateInfo, &commandBuffer);

        VkCommandBufferBeginInfo commandBufferBeginInfo     = {};
        commandBufferBeginInfo.sType                        = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        commandBufferBeginInfo.flags                        = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);

        VkBufferCopy copy       = {};
        copy.srcOffset          = 0;
        copy.dstOffset          = 0;
        copy.size               = size_;
        vkCmdCopyBuffer(commandBuffer, srcBuf_, dstBuf_, 1, &copy);

        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo             = {};
        submitInfo.sType                    = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount       = 1;
        submitInfo.pCommandBuffers          = &commandBuffer;

        vkQueueSubmit(transferQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(transferQueue);

        vkFreeCommandBuffers(engine->logicalDevice, transferCommandPool, 1, &commandBuffer);

    }

    VkCommandBuffer startCommandBuffer() {

        VkCommandBufferAllocateInfo allocInfo   = {};
        allocInfo.sType                         = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level                         = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool                   = vk::engine->standardCommandPool;
        allocInfo.commandBufferCount            = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(engine->logicalDevice, &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo      = {};
        beginInfo.sType                         = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags                         = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        return commandBuffer;

    }

    void endCommandBuffer(VkCommandBuffer commandBuffer_) {

        vkEndCommandBuffer(commandBuffer_);

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer_;

        vkQueueSubmit(
            vk::engine->graphicsQueue,
            1,
            &submitInfo,
            VK_NULL_HANDLE
            );

        vkQueueWaitIdle(vk::engine->graphicsQueue);

        vkFreeCommandBuffers(
            engine->logicalDevice,
            vk::engine->standardCommandPool,
            1,
            &commandBuffer_
            );

    }

    void imageLayoutTransition(
        VkImage         image_,
        VkFormat        format_,
        VkImageLayout   oldLayout_,
        VkImageLayout   newLayout_,
        uint32_t        mipLevels_
        ) {

        VkCommandBuffer commandBuffer               = startCommandBuffer();

        VkImageMemoryBarrier barrier                = {};
        barrier.sType                               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout                           = oldLayout_;
        barrier.newLayout                           = newLayout_;
        barrier.srcQueueFamilyIndex                 = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex                 = VK_QUEUE_FAMILY_IGNORED;
        barrier.image                               = image_;
        
        if (newLayout_ == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {

            barrier.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_DEPTH_BIT;

            if (hasStencilBufferComponent(format_)) {

                barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;

            }

        }
        else {

            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

        }

        barrier.subresourceRange.baseMipLevel       = 0;
        barrier.subresourceRange.levelCount         = 1;
        barrier.subresourceRange.baseArrayLayer     = 0;
        barrier.subresourceRange.layerCount         = 1;

        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;

        if (oldLayout_ == VK_IMAGE_LAYOUT_UNDEFINED && newLayout_ == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {

            barrier.srcAccessMask       = 0;
            barrier.dstAccessMask       = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage                 = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage            = VK_PIPELINE_STAGE_TRANSFER_BIT;

        }
        else if (oldLayout_ == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout_ == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {

            barrier.srcAccessMask       = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask       = VK_ACCESS_SHADER_READ_BIT;

            sourceStage                 = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage            = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

        }
        else if (oldLayout_ == VK_IMAGE_LAYOUT_UNDEFINED && newLayout_ == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {

            barrier.srcAccessMask       = 0;
            barrier.dstAccessMask       = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

            sourceStage                 = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage            = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        
        }
        else if (oldLayout_ == VK_IMAGE_LAYOUT_UNDEFINED && newLayout_ == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {

            barrier.srcAccessMask       = 0;
            barrier.dstAccessMask       = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            
            sourceStage                 = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage            = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

        }
        else {

            logger::log(ERROR_LOG, "Unsupported layout transition");

        }

        vkCmdPipelineBarrier(
            commandBuffer,
            sourceStage,
            destinationStage,
            0,
            0,
            nullptr,
            0,
            nullptr,
            1,
            &barrier
            );

        endCommandBuffer(commandBuffer);

    }

    void copyBufferToImage(
        VkBuffer        buffer_,
        VkImage         image_,
        uint32_t        width_,
        uint32_t        height_
        ) {

        VkCommandBuffer commandBuffer               = startCommandBuffer();

        VkBufferImageCopy copyRegion                = {};
        copyRegion.bufferOffset                     = 0;
        copyRegion.bufferRowLength                  = 0;
        copyRegion.bufferImageHeight                = 0;

        copyRegion.imageSubresource.aspectMask      = VK_IMAGE_ASPECT_COLOR_BIT;
        copyRegion.imageSubresource.mipLevel        = 0;
        copyRegion.imageSubresource.baseArrayLayer  = 0;
        copyRegion.imageSubresource.layerCount      = 1;

        copyRegion.imageOffset                      = { 0, 0, 0 };
        copyRegion.imageExtent                      = { width_, height_, 1 };

        vkCmdCopyBufferToImage(
            commandBuffer,
            buffer_,
            image_,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            &copyRegion
            );

        endCommandBuffer(commandBuffer);

    }

    VkImageView createImageView(
        VkImage                 image_, 
        VkFormat                format_, 
        VkImageAspectFlags      aspectFlags_,
        uint32_t                mipLevels_
        ) {

        logger::log(EVENT_LOG, "Creating image view...");

        VkImageViewCreateInfo imageViewCreateInfo               = {};
        imageViewCreateInfo.sType                               = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfo.image                               = image_;
        imageViewCreateInfo.viewType                            = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.format                              = format_;
        imageViewCreateInfo.subresourceRange.aspectMask         = aspectFlags_;
        imageViewCreateInfo.subresourceRange.baseMipLevel       = 0;
        imageViewCreateInfo.subresourceRange.levelCount         = 1;
        imageViewCreateInfo.subresourceRange.baseArrayLayer     = 0;
        imageViewCreateInfo.subresourceRange.layerCount         = 1;
        imageViewCreateInfo.components.r                        = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.g                        = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.b                        = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.a                        = VK_COMPONENT_SWIZZLE_IDENTITY;

        VkImageView imgView;
        VkResult result = vkCreateImageView(
            engine->logicalDevice,
            &imageViewCreateInfo,
            engine->allocator,
            &imgView
            );
        ASSERT(result, "Failed to create image view", VK_SC_IMAGE_VIEW_CREATION_ERROR);

        logger::log(EVENT_LOG, "Successfully created image view");

        return imgView;

    }

    VkFormat enumerateSupportedBufferFormat(const std::vector< VkFormat >& candidates_, VkImageTiling tiling_, VkFormatFeatureFlags features_) {

        for (VkFormat format : candidates_) {

            VkFormatProperties properties;
            vkGetPhysicalDeviceFormatProperties(engine->physicalDevice, format, &properties);

            if (tiling_ == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & features_) == features_) {      // Does the format support linear tiling?

                return format;

            }
            else if (tiling_ == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & features_) == features_) {       // Does the format support optimal tiling?

                return format;

            }

        }

        logger::log(ERROR_LOG, "Failed to enumerate a supported depth buffer format");

        return VK_FORMAT_UNDEFINED;

    }

    uint32_t enumerateSuitableMemoryType(uint32_t typeFilter_, VkMemoryPropertyFlags memoryPropertyFlags_) {

        VkPhysicalDeviceMemoryProperties memProp;
        vkGetPhysicalDeviceMemoryProperties(engine->physicalDevice, &memProp);

        for (uint32_t i = 0; i < memProp.memoryTypeCount; i++) {
            // Does the memory type have all of the necessary properties?
            if (typeFilter_ & (1 << i) && (memProp.memoryTypes[i].propertyFlags & memoryPropertyFlags_) == memoryPropertyFlags_) {        // Some bitwise-operation magic to find appropriate bit-indices

                return i;

            }

        }

        logger::log(ERROR_LOG, "Failed to find suitable memory type!");

        return VK_SC_BUFFER_MEMORY_TYPE_CREATION_ERROR;

    }

    VK_STATUS_CODE createImage(
        uint32_t                    width_,
        uint32_t                    height_,
        uint32_t                    mipLevels_,
        VkFormat                    format_,
        VkImageTiling               tiling_,
        VkImageUsageFlags           usage_,
        VkMemoryPropertyFlags       properties_,
        VkSampleCountFlagBits       samples_,
        VkImage& img_,
        VkDeviceMemory& imgMem_
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
            vk::engine->logicalDevice,
            &imgCreateInfo,
            vk::engine->allocator,
            &img_
            );
        ASSERT(result, "Failed to create image from the given parameters", VK_SC_IMAGE_CREATION_ERROR);

        VkMemoryRequirements memReqs;
        vkGetImageMemoryRequirements(vk::engine->logicalDevice, img_, &memReqs);

        VkMemoryAllocateInfo memoryAllocInfo        = {};
        memoryAllocInfo.sType                       = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memoryAllocInfo.allocationSize              = memReqs.size;
        memoryAllocInfo.memoryTypeIndex             = vk::enumerateSuitableMemoryType(memReqs.memoryTypeBits, properties_);

        result = vkAllocateMemory(
            vk::engine->logicalDevice,
            &memoryAllocInfo,
            vk::engine->allocator,
            &imgMem_
            );
        ASSERT(result, "Failed to allocate image memory", VK_SC_IMAGE_MEMORY_ALLOCATION_ERROR);

        vkBindImageMemory(
            vk::engine->logicalDevice,
            img_,
            imgMem_,
            0
            );

        return vk::errorCodeBuffer;

    }


    bool hasStencilBufferComponent(VkFormat format_) {

        return format_ == VK_FORMAT_D32_SFLOAT_S8_UINT || format_ == VK_FORMAT_D24_UNORM_S8_UINT;

    }

}