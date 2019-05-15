/**
	Implements the BaseBuffer class

	@author		D3PSI
	@version	0.0.1 02.12.2019

	@file		BaseBuffer.cpp
	@brief		Implementation of the BaseBuffer class
*/
#include "BaseBuffer.hpp"
#include "VK.hpp"


BaseBuffer::BaseBuffer() {



}

BaseBuffer::BaseBuffer(const VkBufferCreateInfo* bufferCreateInfo_, VkMemoryPropertyFlags properties_) {

	bufferCreateInfo = *bufferCreateInfo_;

	logger::log(EVENT_LOG, "Creating buffer...");
	vk::engine.result = vkCreateBuffer(
		vk::engine.logicalDevice,
		bufferCreateInfo_,
		vk::engine.allocator,
		&buf
		);
	ASSERT(vk::engine.result, "Failed to create buffer", VK_SC_BUFFER_CREATION_ERROR);
	logger::log(EVENT_LOG, "Successfully created buffer");

	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(vk::engine.logicalDevice, buf, &memoryRequirements);

	VkMemoryAllocateInfo memoryAllocateInfo				= {};
	memoryAllocateInfo.sType							= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocateInfo.allocationSize					= memoryRequirements.size;
	memoryAllocateInfo.memoryTypeIndex					= enumerateSuitableMemoryType(memoryRequirements.memoryTypeBits, properties_);

	vk::engine.result = vkAllocateMemory(
		vk::engine.logicalDevice,
		&memoryAllocateInfo,
		vk::engine.allocator,
		&mem
		);
	ASSERT(vk::engine.result, "Failed to allocate buffer memory", VK_SC_BUFFER_ALLOCATION_ERROR);

	bind();

}

BaseBuffer::~BaseBuffer() {

	vkDestroyBuffer(vk::engine.logicalDevice, buf, vk::engine.allocator);
	logger::log(EVENT_LOG, "Successfully destroyed buffer");

	vkFreeMemory(vk::engine.logicalDevice, mem, vk::engine.allocator);
	logger::log(EVENT_LOG, "Successfully destroyed buffer memory");

}

uint32_t BaseBuffer::enumerateSuitableMemoryType(uint32_t typeFilter_, VkMemoryPropertyFlags memoryPropertyFlags_) {

	VkPhysicalDeviceMemoryProperties memProp;
	vkGetPhysicalDeviceMemoryProperties(vk::engine.physicalDevice, &memProp);

	for (uint32_t i = 0; i < memProp.memoryTypeCount; i++) {
									// Does the memory type have all of the necessary properties?
		if (typeFilter_ & (1 << i) && (memProp.memoryTypes[i].propertyFlags & memoryPropertyFlags_) == memoryPropertyFlags_) {		// Some bitwise-operation magic to find appropriate bit-indices
		
			return i;
		
		}
	
	}

	logger::log(ERROR_LOG, "Failed to find suitable memory type!");

	return VK_SC_BUFFER_MEMORY_TYPE_CREATION_ERROR;

}

VK_STATUS_CODE BaseBuffer::bind() {
	
	vk::engine.result = vkBindBufferMemory(
		vk::engine.logicalDevice,
		buf,
		mem,
		static_cast< uint64_t >(0)
		);
	ASSERT(vk::engine.result, "Failed to bind buffer memory", VK_SC_BUFFER_BINDING_ERROR);

	return VK_SC_SUCCESS;

}

VK_STATUS_CODE BaseBuffer::fill(const void* bufData_) {

	void* data;
	vkMapMemory(
		vk::engine.logicalDevice,
		mem,
		0,
		bufferCreateInfo.size,
		0,
		&data
		);
	memcpy(data, bufData_, static_cast< size_t >(bufferCreateInfo.size));
	vkUnmapMemory(vk::engine.logicalDevice, mem);

	return VK_SC_SUCCESS;

}

VK_STATUS_CODE BaseBuffer::fillS(const void* bufData_, size_t bufSize_) {

    VkBufferCreateInfo bufferCreateInfo     = {};
    bufferCreateInfo.sType                  = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.size                   = bufSize_;
    bufferCreateInfo.usage                  = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    bufferCreateInfo.sharingMode            = VK_SHARING_MODE_EXCLUSIVE;

    BaseBuffer* stagingBuffer               = new BaseBuffer(&bufferCreateInfo, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    void* data;
    vkMapMemory(
        vk::engine.logicalDevice,
        stagingBuffer->mem,
        0,
        bufSize_,
        0,
        &data
    );
    memcpy(data, bufData_, static_cast< size_t >(bufSize_));
    vkUnmapMemory(vk::engine.logicalDevice, stagingBuffer->mem);

    vk::copyBuffer(stagingBuffer->buf, buf, bufSize_);

    delete stagingBuffer;

    return VK_SC_SUCCESS;

}