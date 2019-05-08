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

BaseBuffer::BaseBuffer(const VkBufferCreateInfo* bufferCreateInfo_) {

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
	memoryAllocateInfo.memoryTypeIndex					= enumerateSuitableMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	vk::engine.result = vkAllocateMemory(
		vk::engine.logicalDevice,
		&memoryAllocateInfo,
		vk::engine.allocator,
		&mem
		);
	ASSERT(vk::engine.result, "Failed to allocate buffer memory", VK_SC_BUFFER_ALLOCATION_ERROR);

	bind();

}

VK_STATUS_CODE BaseBuffer::destroy() {

	vkDestroyBuffer(vk::engine.logicalDevice, buf, vk::engine.allocator);
	logger::log(EVENT_LOG, "Successfully destroyed buffer");

	vkFreeMemory(vk::engine.logicalDevice, mem, vk::engine.allocator);
	logger::log(EVENT_LOG, "Successfully destroyed buffer memory");

	return VK_SC_SUCCESS;

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

VK_STATUS_CODE BaseBuffer::fill(std::vector< BaseVertex > bufData_) {

	void* data;
	vkMapMemory(
		vk::engine.logicalDevice,
		mem,
		0,
		bufferCreateInfo.size,
		0,
		&data
		);
	memcpy(data, bufData_.data(), static_cast<size_t>(bufferCreateInfo.size));
	vkUnmapMemory(vk::engine.logicalDevice, mem);

	return VK_SC_SUCCESS;

}

VkBuffer BaseBuffer::get() {

	return buf;

}