/**
	Implements the VertexBuffer class, inheriting BaseBuffer

	@author		D3PSI
	@version	0.0.1 02.12.2019

	@file		VertexBuffer.cpp
	@brief		Implementation of the VertexBuffer class
*/
#include "VertexBuffer.hpp"
#include "VK.hpp"


VK_STATUS_CODE VertexBuffer::fill(std::vector< BaseVertex > bufData_) {

    VkDeviceSize bufferSize = sizeof(vk::vertices[0]) * vk::vertices.size();

    VkBufferCreateInfo bufferCreateInfo         = {};
    bufferCreateInfo.sType                      = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.size                       = bufferSize;
    bufferCreateInfo.usage                      = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    bufferCreateInfo.sharingMode                = VK_SHARING_MODE_EXCLUSIVE;
    
    BaseBuffer* stagingBuffer = new BaseBuffer(&bufferCreateInfo, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    void* data;
    vkMapMemory(
        vk::engine.logicalDevice,
        stagingBuffer->mem,
        0,
        bufferSize,
        0,
        &data
    );
    memcpy(data, bufData_.data(), static_cast< size_t >(bufferSize));
    vkUnmapMemory(vk::engine.logicalDevice, stagingBuffer->mem);

    vk::copyBuffer(stagingBuffer->buf, this->buf, bufferSize);

    delete stagingBuffer;

    return VK_SC_SUCCESS;

}
