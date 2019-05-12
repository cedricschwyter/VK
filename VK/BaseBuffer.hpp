/**
	Defines the BaseBuffer class

	@author		D3PSI
	@version	0.0.1 02.12.2019

	@file		BaseBuffer.hpp
	@brief		Definition of the BaseBuffer class
*/
#pragma once
#include <vulkan/vulkan.h>

#include <vector>

#include "VK_STATUS_CODE.hpp"
#include "BaseVertex.hpp"

class BaseBuffer
{
public:

    VkBuffer				buf             = VK_NULL_HANDLE;
    VkDeviceMemory			mem             = VK_NULL_HANDLE;

	/**
		Explicit Default constructor
	*/
	explicit BaseBuffer();

	/**
		Constructor

		@param		bufferCreateInfo_		A pointer to a VkBufferCreateInfo structure
        @param      properties_             Necessary memory properties
	*/
	explicit BaseBuffer(const VkBufferCreateInfo* bufferCreateInfo_, VkMemoryPropertyFlags properties_);

	/**
		Maps data to a buffer

		@param		bufData_		Pointer to the data that needs to be copied to the buffer

		@return		Returns VK_SC_SUCCESS on success
	*/
	virtual VK_STATUS_CODE fill(const std::vector< BaseVertex >* bufData_);

    /**
        Maps data to a buffer (overload for different data type as I cant find any other solution just now)

        @param      bufData_        Pointer to the data that needs to be copied to the buffer

        @return     Returns VK_SC_SUCCESS on success
    */
    virtual VK_STATUS_CODE fill(const std::vector< uint32_t >* bufData_);

	/**
		Returns the VkBuffer handle

		@return		Returns the VkBuffer handle
	*/
	VkBuffer get(void);

	/**
		Binds a buffer

		@return		Returns VK_SC_SUCCESS on success
	*/
	virtual VK_STATUS_CODE bind(void);

	/**
		Default destructor

		@return Returns VK_SC_SUCCESS on success
	*/
	~BaseBuffer(void);

protected:

    VkBufferCreateInfo		bufferCreateInfo           = {};

	/**
		Finds the appropriate memory type to use for the specified operation

		@param		typeFilter_					Filter for the right memory type
		@param		memoryPropertyFlags_		The necessary memory properties that must be supported by the memory

		@return		Returns an index for a memory type 
	*/
	uint32_t enumerateSuitableMemoryType(uint32_t typeFilter_, VkMemoryPropertyFlags memoryPropertyFlags_);

};

