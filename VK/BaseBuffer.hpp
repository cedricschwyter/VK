/**
    Defines the BaseBuffer class

    @author        D3PSI
    @version    0.0.1 02.12.2019

    @file        BaseBuffer.hpp
    @brief        Definition of the BaseBuffer class
*/
#pragma once
#include <vulkan/vulkan.h>

#include <vector>

#include "VK_STATUS_CODE.hpp"
#include "BaseVertex.hpp"

class BaseBuffer
{
public:

    VkBuffer                  buf             = VK_NULL_HANDLE;
    VkDeviceMemory            mem             = VK_NULL_HANDLE;

    /**
        Explicit Default constructor
    */
    explicit BaseBuffer(void) = default;

    /**
        Constructor

        @param        bufferCreateInfo_        A pointer to a VkBufferCreateInfo structure
        @param        properties_              Necessary memory properties
    */
    explicit BaseBuffer(const VkBufferCreateInfo* bufferCreateInfo_, VkMemoryPropertyFlags properties_);

    /**
        Constructor with more precise arguments

        @param      size_           The buffer size
        @param      usage_          Buffer usage flags
        @param      properties_     Necessary memory properties
    */
    explicit BaseBuffer(VkDeviceSize size_, VkBufferUsageFlags usage_, VkMemoryPropertyFlags properties_);

    /**
        Maps data to a buffer

        @param         bufData_        Pointer to the data that needs to be copied to the buffer

        @return        Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE fill(const void* bufData_);

    /**
        Maps data to a buffer

        @param         bufData_        Overloaded parameter specifically for images

        @return        Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE fill(const unsigned char* bufData_);

    /**
        Maps data to a buffer using a staging buffer

        @param      bufData_        Pointer to the data that needs to be copied to the buffer
        @param      bufSize_        The size of the buffer in bytes

        @return     Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE fillS(const void* bufData_, size_t bufSize_);

    /**
        Binds a buffer

        @return        Returns VK_SC_SUCCESS on success
    */
    virtual VK_STATUS_CODE bind(void);

    /**
        Default destructor

        @return Returns VK_SC_SUCCESS on success
    */
    ~BaseBuffer(void);

protected:

    VkBufferCreateInfo        bufferCreateInfo           = {};

};

