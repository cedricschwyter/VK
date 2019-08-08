/**
    Defines the UniformInfo struct

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         UniformInfo.cpp
    @brief        Definition of the UniformInfo struct
*/ 
#ifndef UNIFORM_INFO_CPP
#define UNIFORM_INFO_CPP
#include <vulkan/vulkan.h>

/**
    Holds uniform binding info
*/
struct UniformInfo {

    uint32_t                    binding;
    VkPipelineStageFlags        stageFlags;
    VkDescriptorBufferInfo      bufferInfo;
    VkDescriptorImageInfo       imageInfo;
    VkDescriptorType            type;

};
#endif
