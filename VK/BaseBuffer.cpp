/**
    Implements the BaseBuffer class

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         BaseBuffer.cpp
    @brief        Implementation of the BaseBuffer class
*/
#include "BaseBuffer.hpp"
#include "VK.hpp"
#include "ASSERT.cpp"


BaseBuffer::BaseBuffer(const VkBufferCreateInfo* bufferCreateInfo_, VkMemoryPropertyFlags properties_) {

    bufferCreateInfo = *bufferCreateInfo_;

    logger::log(EVENT_LOG, "Creating buffer...");
    VkResult result = vkCreateBuffer(
        VKCore::logicalDevice,
        bufferCreateInfo_,
        VKCore::allocator,
        &buf
        );
    ASSERT(result, "Failed to create buffer", VK_SC_BUFFER_CREATION_ERROR);
    logger::log(EVENT_LOG, "Successfully created buffer");

    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(VKCore::logicalDevice, buf, &memoryRequirements);

    VkMemoryAllocateInfo memoryAllocateInfo                = {};
    memoryAllocateInfo.sType                               = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.allocationSize                      = memoryRequirements.size;
    memoryAllocateInfo.memoryTypeIndex                     = vk::enumerateSuitableMemoryType(memoryRequirements.memoryTypeBits, properties_);

    result = vkAllocateMemory(
        VKCore::logicalDevice,
        &memoryAllocateInfo,
        VKCore::allocator,
        &mem
        );
    ASSERT(result, "Failed to allocate buffer memory", VK_SC_BUFFER_ALLOCATION_ERROR);

    bind();

}

BaseBuffer::BaseBuffer(VkDeviceSize size_, VkBufferUsageFlags usage_, VkMemoryPropertyFlags properties_ ) {

    logger::log(EVENT_LOG, "Creating buffer...");

    QueueFamily family                        = VKCore::findSuitableQueueFamily(VKCore::physicalDevice);
    std::vector< uint32_t > indices           = { family.transferFamilyIndex.value() };

    bufferCreateInfo.sType                    = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.size                     = size_;
    bufferCreateInfo.usage                    = usage_;
    bufferCreateInfo.sharingMode              = VK_SHARING_MODE_EXCLUSIVE;
    bufferCreateInfo.pQueueFamilyIndices      = indices.data();
    bufferCreateInfo.queueFamilyIndexCount    = static_cast< uint32_t >(indices.size());

    VkResult result = vkCreateBuffer(
        VKCore::logicalDevice,
        &bufferCreateInfo, 
        VKCore::allocator,
        &buf
        );
    ASSERT(result, "Failed to create buffer", VK_SC_BUFFER_CREATION_ERROR);
    logger::log(EVENT_LOG, "Successfully created buffer");

    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(VKCore::logicalDevice, buf, &memoryRequirements);

    VkMemoryAllocateInfo memoryAllocateInfo         = {};
    memoryAllocateInfo.sType                        = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.allocationSize               = memoryRequirements.size;
    memoryAllocateInfo.memoryTypeIndex              = vk::enumerateSuitableMemoryType(memoryRequirements.memoryTypeBits, properties_);

    result = vkAllocateMemory(
        VKCore::logicalDevice,
        &memoryAllocateInfo,
        VKCore::allocator,
        &mem
        );
    ASSERT(result, "Failed to allocate buffer memory", VK_SC_BUFFER_ALLOCATION_ERROR);

    bind();

}

BaseBuffer::~BaseBuffer() {

    vkDestroyBuffer(VKCore::logicalDevice, buf, VKCore::allocator);
    logger::log(EVENT_LOG, "Successfully destroyed buffer");

    vkFreeMemory(VKCore::logicalDevice, mem, VKCore::allocator);
    logger::log(EVENT_LOG, "Successfully destroyed buffer memory");

}

VK_STATUS_CODE BaseBuffer::bind() {
    
    VkResult result = vkBindBufferMemory(
        VKCore::logicalDevice,
        buf,
        mem,
        static_cast< uint64_t >(0)
        );
    ASSERT(result, "Failed to bind buffer memory", VK_SC_BUFFER_BINDING_ERROR);

    return vk::errorCodeBuffer;

}

VK_STATUS_CODE BaseBuffer::fill(const void* bufData_) {

    void* data;
    vkMapMemory(
        VKCore::logicalDevice,
        mem,
        0,
        bufferCreateInfo.size,
        0,
        &data
        );
    memcpy(data, bufData_, static_cast< size_t >(bufferCreateInfo.size));
    vkUnmapMemory(VKCore::logicalDevice, mem);

    return vk::errorCodeBuffer;

}

VK_STATUS_CODE BaseBuffer::fill(const unsigned char* bufData_) {

    void* data;
    vkMapMemory(
        VKCore::logicalDevice,
        mem,
        0,
        bufferCreateInfo.size,
        0,
        &data
        );
    memcpy(data, bufData_, static_cast<size_t>(bufferCreateInfo.size));
    vkUnmapMemory(VKCore::logicalDevice, mem);

    return vk::errorCodeBuffer;

}

VK_STATUS_CODE BaseBuffer::fillS(const void* bufData_, size_t bufSize_) {

    QueueFamily family                      = VKCore::findSuitableQueueFamily(VKCore::physicalDevice);
    std::vector< uint32_t > indices         = { family.transferFamilyIndex.value() };

    VkBufferCreateInfo bufferCreateInfo     = {};
    bufferCreateInfo.sType                  = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.size                   = bufSize_;
    bufferCreateInfo.usage                  = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    bufferCreateInfo.sharingMode            = VK_SHARING_MODE_EXCLUSIVE;
    bufferCreateInfo.pQueueFamilyIndices    = indices.data();
    bufferCreateInfo.queueFamilyIndexCount  = static_cast< uint32_t >(indices.size());

    BaseBuffer* stagingBuffer               = new BaseBuffer(&bufferCreateInfo, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    stagingBuffer->fill(bufData_);

    vk::copyBuffer(stagingBuffer->buf, buf, bufSize_);

    delete stagingBuffer;

    return vk::errorCodeBuffer;

}