/**
    Implements the DescriptorSet wrapper

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         DescriptorSet.cpp
    @brief        Implementation of the DescriptorSet wrapper
*/
#include "DescriptorSet.hpp"
#include "VK.hpp"
#include "ASSERT.cpp"


DescriptorSet::DescriptorSet(std::vector< Descriptor > descriptors_) {

    descriptorSetLayout = new DescriptorSetLayout(descriptors_);

    descriptorPool = new DescriptorPool(descriptorSetLayout);

    std::vector< VkDescriptorSetLayout > layouts(vk::engine->swapchainImages.size(), descriptorSetLayout->descriptorSetLayout);

    VkDescriptorSetAllocateInfo allocateInfo        = {};
    allocateInfo.sType                              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocateInfo.descriptorPool                     = descriptorPool->descriptorPool;
    allocateInfo.descriptorSetCount                 = static_cast< uint32_t >(vk::engine->swapchainImages.size());
    allocateInfo.pSetLayouts                        = layouts.data();

    descriptorSets.resize(vk::engine->swapchainImages.size());
    VkResult result = vkAllocateDescriptorSets(vk::engine->logicalDevice, &allocateInfo, descriptorSets.data());
    ASSERT(result, "Failed to allocate descriptor sets", VK_SC_DESCRIPTOR_SET_CREATION_ERROR);

    for (size_t i = 0; i < vk::engine->swapchainImages.size(); i++) {

        for (size_t j = 0; j < descriptors_.size(); j++) {
        
            VkWriteDescriptorSet writeDescriptorSet         = {};
            writeDescriptorSet.sType                        = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeDescriptorSet.dstSet                       = descriptorSets[i];
            writeDescriptorSet.dstBinding                   = descriptors_[j].info->binding;
            writeDescriptorSet.dstArrayElement              = 0;
            writeDescriptorSet.descriptorType               = descriptors_[j].info->type;
            writeDescriptorSet.descriptorCount              = 1;
            writeDescriptorSet.pBufferInfo                  = &(descriptors_[j].info->bufferInfo);
            writeDescriptorSet.pImageInfo                   = &(descriptors_[j].info->imageInfo);

            vkUpdateDescriptorSets(
                vk::engine->logicalDevice,
                1,
                &writeDescriptorSet,
                0,
                nullptr
                );

        }

    }

}

void DescriptorSet::update(std::vector< Descriptor > descriptors_) {

    for (size_t i = 0; i < vk::engine->swapchainImages.size(); i++) {

        for (size_t j = 0; j < descriptors_.size(); j++) {

            VkWriteDescriptorSet writeDescriptorSet         = {};
            writeDescriptorSet.sType                        = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeDescriptorSet.dstSet                       = descriptorSets[i];
            writeDescriptorSet.dstBinding                   = descriptors_[j].info->binding;
            writeDescriptorSet.dstArrayElement              = 0;
            writeDescriptorSet.descriptorType               = descriptors_[j].info->type;
            writeDescriptorSet.descriptorCount              = 1;
            writeDescriptorSet.pBufferInfo                  = &(descriptors_[j].info->bufferInfo);
            writeDescriptorSet.pImageInfo                   = &(descriptors_[j].info->imageInfo);

            vkUpdateDescriptorSets(
                vk::engine->logicalDevice,
                1,
                &writeDescriptorSet,
                0,
                nullptr
                );

        }

    }

}

void DescriptorSet::bind(std::vector< VkCommandBuffer >& commandBuffers_, uint32_t imageIndex_, GraphicsPipeline pipeline_) {

    vkCmdBindDescriptorSets(
        commandBuffers_[imageIndex_],
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        pipeline_.pipelineLayout,
        0,
        1,
        &(descriptorSets[imageIndex_]),
        0,
        nullptr
        );

}

DescriptorSet::~DescriptorSet() {

     delete descriptorSetLayout;

     delete descriptorPool;

}
