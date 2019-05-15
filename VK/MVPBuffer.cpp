/**
    Implements the MVPBuffer class, inheriting BaseBuffer

    @author		D3PSI
    @version	0.0.1 02.12.2019

    @file		MVPBuffer.cpp
    @brief		Implementation of the MVPBuffer class
*/
#include "MVPBuffer.hpp"
#include "VK.hpp"


/*VK_STATUS_CODE MVPBuffer::fill(MVPBufferObject mvp_) {

    void* data;
    vkMapMemory(
        vk::engine.logicalDevice,
        mem,
        0,
        sizeof(mvp_),
        0,
        &data
    );
    memcpy(data, &mvp_, sizeof(mvp_));
    vkUnmapMemory(vk::engine.logicalDevice, mem);

    return VK_SC_SUCCESS;

}*/
