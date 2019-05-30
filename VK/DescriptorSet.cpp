/**
    Implements the DescriptorSet wrapper

    @author		D3PSI
    @version	0.0.1 02.12.2019

    @file		DescriptorSet.cpp
    @brief		Implementation of the DescriptorSet wrapper
*/
#include "DescriptorSet.hpp"
#include "VK.hpp"


DescriptorSet::DescriptorSet() {



}

DescriptorSet::DescriptorSet(const std::vector< UniformInfo >& uniformBindings_, uint32_t numUniforms_) {

    std::vector< VkDescriptorSetLayoutBinding > bindings;
    bindings.resize(numUniforms_);

    for (size_t i = 0; i < numUniforms_; i++) {

        VkDescriptorSetLayoutBinding binding                    = {};
        binding.binding                                         = uniformBindings_[i].binding;
        binding.descriptorType                                  = uniformBindings_[i].type;
        binding.descriptorCount                                 = 1;
        binding.stageFlags                                      = uniformBindings_[i].stageFlags;

        bindings[i] = binding;

    }

    VkDescriptorSetLayoutCreateInfo layoutCreateInfo            = {};
    layoutCreateInfo.sType                                      = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutCreateInfo.bindingCount                               = static_cast< uint32_t >(bindings.size());
    layoutCreateInfo.pBindings                                  = bindings.data();

    VkResult result = vkCreateDescriptorSetLayout(
        vk::engine.logicalDevice,
        &layoutCreateInfo,
        vk::engine.allocator,
        &descriptorSetLayout
        );
    ASSERT(result, "Failed to create descriptor set layout", VK_SC_DESCRIPTOR_SET_LAYOUT_CREATION_ERROR);

    std::vector< VkDescriptorPoolSize > descriptorPoolSizes;
    descriptorPoolSizes.resize(numUniforms_);
    for (uint32_t i = 0; i < numUniforms_; i++) {

        descriptorPoolSizes[i].type                     = uniformBindings_[i].type;
        descriptorPoolSizes[i].descriptorCount          = static_cast<uint32_t>(vk::engine.swapchainImages.size());

    }

    VkDescriptorPoolCreateInfo descriptorPoolCreateInfo         = {};
    descriptorPoolCreateInfo.sType                              = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolCreateInfo.poolSizeCount                      = static_cast< uint32_t >(descriptorPoolSizes.size());
    descriptorPoolCreateInfo.pPoolSizes                         = descriptorPoolSizes.data();
    descriptorPoolCreateInfo.maxSets                            = static_cast< uint32_t >(vk::engine.swapchainImages.size());

    result = vkCreateDescriptorPool(
        vk::engine.logicalDevice,
        &descriptorPoolCreateInfo,
        vk::engine.allocator,
        &descriptorPool
        );
    ASSERT(result, "Failed to create descriptor pool", VK_SC_DESCRIPTOR_POOL_ERROR);

    std::vector< VkDescriptorSetLayout > layouts(vk::engine.swapchainImages.size(), descriptorSetLayout);

    VkDescriptorSetAllocateInfo allocateInfo        = {};
    allocateInfo.sType                              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocateInfo.descriptorPool                     = descriptorPool;
    allocateInfo.descriptorSetCount                 = static_cast< uint32_t >(vk::engine.swapchainImages.size());
    allocateInfo.pSetLayouts                        = layouts.data();

    descriptorSets.resize(vk::engine.swapchainImages.size());
    result = vkAllocateDescriptorSets(vk::engine.logicalDevice, &allocateInfo, descriptorSets.data());
    ASSERT(result, "Failed to allocate descriptor sets", VK_SC_DESCRIPTOR_SET_CREATION_ERROR);

    for (size_t i = 0; i < vk::engine.swapchainImages.size(); i++) {

        for (size_t j = 0; j < numUniforms_; j++) {
        
            VkWriteDescriptorSet writeDescriptorSet         = {};
            writeDescriptorSet.sType                        = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeDescriptorSet.dstSet                       = descriptorSets[i];
            writeDescriptorSet.dstBinding                   = uniformBindings_[j].binding;
            writeDescriptorSet.dstArrayElement              = 0;
            writeDescriptorSet.descriptorType               = uniformBindings_[j].type;
            writeDescriptorSet.descriptorCount              = 1;
            writeDescriptorSet.pBufferInfo                  = &(uniformBindings_[j].bufferInfo);
            writeDescriptorSet.pImageInfo                   = &(uniformBindings_[j].imageInfo);

            vkUpdateDescriptorSets(
                vk::engine.logicalDevice,
                1,
                &writeDescriptorSet,
                0,
                nullptr
                );

        }

    }

}

DescriptorSet::~DescriptorSet() {

    if (descriptorPool != VK_NULL_HANDLE && descriptorSetLayout != VK_NULL_HANDLE) {

        vkDestroyDescriptorSetLayout(vk::engine.logicalDevice, descriptorSetLayout, vk::engine.allocator);
        logger::log(EVENT_LOG, "Successfully destroyed descriptor set layout");

        vkDestroyDescriptorPool(vk::engine.logicalDevice, descriptorPool, vk::engine.allocator);
        logger::log(EVENT_LOG, "Successfully destroyed descriptor pool");

    }

}
