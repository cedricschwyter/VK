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
    const unsigned int                  MAX_IN_FLIGHT_FRAMES        = 3;
    const double                        YAW                         = 0.0;
    const double                        PITCH                       = 0.0;
    const double                        ROLL                        = 0.0;
    const double                        SPEED                       = 2.0;
    const double                        SENS                        = 0.1;
    const double                        FOV                         = 45.0;

    VkCommandPool                       vk::graphicsCommandPool     = VK_NULL_HANDLE;
    VkQueue                             vk::graphicsQueue           = VK_NULL_HANDLE;
    VkFence                             graphicsFence;
    std::mutex                          graphicsMutex;
    VkQueue                             transferQueue               = VK_NULL_HANDLE;
    VkCommandPool                       transferCommandPool         = VK_NULL_HANDLE;
    VkFence                             transferFence;
    std::mutex                          transferMutex;

    std::mutex                          loadingMutex;

    VK_STATUS_CODE init() {

        engine = new VKEngine();
        engine->initLoadingScreen();
        ASSERT(engine->initWindow(), "Window initialization error", VK_SC_WINDOW_ERROR);
        std::thread t0([]() {

                ASSERT(engine->initVulkan(), "Vulkan initialization error", VK_SC_VULKAN_ERROR);

            });
        t0.detach();

        return VK_SC_SUCCESS;

    }

    VK_STATUS_CODE run() {

        try {

            VK_STATUS_CODE* returnAddr = new VK_STATUS_CODE();
            std::thread t0(&VKEngine::init, engine, returnAddr);
            t0.join();

            VK_STATUS_CODE retCode = *returnAddr;
            delete returnAddr;

            return retCode;

        }
        catch (std::exception& e) {

            logger::log(ERROR_LOG, e.what());

            return errorCodeBuffer;

        }

    }

    VkResult createDebugUtilsMessenger(
        VkInstance                                      instance_,
        const VkDebugUtilsMessengerCreateInfoEXT*       pCreateInfo_,
        const VkAllocationCallbacks*                    pAllocator_,
        VkDebugUtilsMessengerEXT*                       pDebugMessenger_
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
        const VkAllocationCallbacks*        pAllocator_
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

        VkCommandBuffer commandBuffer = startCommandBuffer(TRANSFER_QUEUE);

        VkBufferCopy copy       = {};
        copy.srcOffset          = 0;
        copy.dstOffset          = 0;
        copy.size               = size_;
        
        std::unique_lock< std::mutex > lock(transferMutex);
        vkCmdCopyBuffer(commandBuffer, srcBuf_, dstBuf_, 1, &copy);
        lock.unlock();

        endCommandBuffer(commandBuffer, TRANSFER_QUEUE);

    }

    VkCommandBuffer startCommandBuffer(Queue queue_) {

        VkCommandPool   commandPool     = VK_NULL_HANDLE;
        std::mutex*     mutex           = nullptr;

        if (queue_ == TRANSFER_QUEUE) {

            std::scoped_lock< std::mutex > lock(transferMutex);
            commandPool = transferCommandPool;
            mutex       = &transferMutex;
        
        }
        else if (queue_ == GRAPHICS_QUEUE) {

            std::scoped_lock< std::mutex > lock(graphicsMutex);
            commandPool = graphicsCommandPool;
            mutex       = &graphicsMutex;
        
        }
        else {
        
            logger::log(ERROR_LOG, "Command buffer was allocated from unsupported command pool");
        
        }

        std::scoped_lock< std::mutex > lock(*mutex);
        VkCommandBufferAllocateInfo allocInfo   = {};
        allocInfo.sType                         = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level                         = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool                   = commandPool;
        allocInfo.commandBufferCount            = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(engine->logicalDevice, &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo      = {};
        beginInfo.sType                         = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags                         = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        return commandBuffer;

    }

    void endCommandBuffer(VkCommandBuffer commandBuffer_, Queue queue_) {

        waitForQueue(queue_);

        VkQueue         queue       = VK_NULL_HANDLE;
        VkFence         fence       = VK_NULL_HANDLE;
        VkCommandPool   commandPool = VK_NULL_HANDLE;
        std::mutex*     mutex       = nullptr;

        if (queue_ == TRANSFER_QUEUE) {
        
            std::scoped_lock< std::mutex > lock(transferMutex);
            queue           = transferQueue;
            fence           = transferFence;
            mutex           = &transferMutex;
            commandPool     = transferCommandPool;
        
        }
        else if (queue_ == GRAPHICS_QUEUE) {

            std::scoped_lock< std::mutex > lock(graphicsMutex);
            queue           = graphicsQueue;
            fence           = graphicsFence;
            mutex           = &graphicsMutex;
            commandPool     = graphicsCommandPool;
        
        } 
        else {
        
            logger::log(ERROR_LOG, "Command buffer was submitted to an unsupported queue");

        }

        std::scoped_lock< std::mutex > lock(*mutex);
        vkEndCommandBuffer(commandBuffer_);

        VkSubmitInfo submitInfo         = {};
        submitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount   = 1;
        submitInfo.pCommandBuffers      = &commandBuffer_;

        vkQueueSubmit(
            queue,
            1,
            &submitInfo,
            fence
            );

        vkQueueWaitIdle(queue);

        vkFreeCommandBuffers(
            engine->logicalDevice,
            commandPool,
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

        VkCommandBuffer commandBuffer               = startCommandBuffer(GRAPHICS_QUEUE);

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


        std::unique_lock< std::mutex > lock(graphicsMutex);
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
        lock.unlock();

        endCommandBuffer(
            commandBuffer, 
            GRAPHICS_QUEUE
            );

    }

    void copyBufferToImage(
        VkBuffer        buffer_,
        VkImage         image_,
        uint32_t        width_,
        uint32_t        height_
        ) {

        VkCommandBuffer commandBuffer               = startCommandBuffer(TRANSFER_QUEUE);

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

        std::unique_lock< std::mutex > lock(transferMutex);
        vkCmdCopyBufferToImage(
            commandBuffer,
            buffer_,
            image_,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            &copyRegion
            );
        lock.unlock();

        endCommandBuffer(commandBuffer, TRANSFER_QUEUE);

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
        VkImage&                    img_,
        VkDeviceMemory&             imgMem_
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

    VK_STATUS_CODE push(const char* path_) {

        engine->push(path_);
        logger::log(EVENT_LOG, "Pushing model at path " + std::string(path_) + " to loading queue");

        return vk::errorCodeBuffer;

    }

    VK_STATUS_CODE push(ModelInfo info_) {

        engine->push(info_);
        logger::log(EVENT_LOG, "Pushing model at path " + std::string(info_.path) + " to loading queue");

        return vk::errorCodeBuffer;

    }

    void waitForQueue(Queue queue_) {
    
        VkFence         fence       = VK_NULL_HANDLE;
        std::mutex*     mutex       = nullptr;

        if (queue_ == TRANSFER_QUEUE) {

            std::scoped_lock< std::mutex > lock(transferMutex);
            fence = transferFence;
            mutex = &transferMutex;
        
        }
        else if (queue_ == GRAPHICS_QUEUE) {

            std::scoped_lock< std::mutex > lock(graphicsMutex);
            fence = graphicsFence;
            mutex = &graphicsMutex;
        
        }
        else {
        
            logger::log(ERROR_LOG, "Waiting for unsupported queue");
        
        }

        std::scoped_lock< std::mutex > lock(*mutex);
        vkWaitForFences(
            engine->logicalDevice,
            1,
            &fence,
            VK_TRUE,
            std::numeric_limits< uint64_t >::max()
            );
        vkResetFences(engine->logicalDevice, 1, &fence);
    
    }

}