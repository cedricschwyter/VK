/**
    Implements the DepthBuffer class, inheriting BaseBuffer

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         DepthBuffer.cpp
    @brief        Implementation of the DepthBuffer class
*/
#include "DepthBuffer.hpp"
#include "VK.hpp"


DepthBuffer::DepthBuffer() {



}

VkFormat DepthBuffer::enumerateSupportedDepthBufferFormat() {

    return vk::enumerateSupportedBufferFormat(
        { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );

}

DepthBuffer::~DepthBuffer() {



}