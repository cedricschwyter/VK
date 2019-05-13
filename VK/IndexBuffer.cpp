/**
    Implements the IndexBuffer class, inheriting BaseBuffer

    @author		D3PSI
    @version	0.0.1 02.12.2019

    @file		IndexBuffer.cpp
    @brief		Implementation of the IndexBuffer class
*/
#include "IndexBuffer.hpp"
#include "VK.hpp"


VK_STATUS_CODE IndexBuffer::fill(const std::vector< uint32_t >* bufData_) {

    VkDeviceSize bufferSize                         = sizeof(bufData_[0]) * bufData_->size();

    VkBufferCreateInfo bufferCreateInfo             = {};
    bufferCreateInfo.sType                          = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.size                           = bufferSize;
    bufferCreateInfo.usage                          = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    bufferCreateInfo.sharingMode                    = VK_SHARING_MODE_EXCLUSIVE;

    BaseBuffer* stagingBuffer                       = new BaseBuffer(&bufferCreateInfo, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    void* data;
    vkMapMemory(
        vk::engine.logicalDevice,
        stagingBuffer->mem,
        0,
        bufferSize,
        0,
        &data
    );
    memcpy(data, bufData_->data(), static_cast< size_t >(bufferSize));
    vkUnmapMemory(vk::engine.logicalDevice, stagingBuffer->mem);

    vk::copyBuffer(stagingBuffer->buf, this->buf, bufferSize);

    delete stagingBuffer;

    return VK_SC_SUCCESS;

}