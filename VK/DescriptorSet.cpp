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

DescriptorSet::DescriptorSet(const std::vector< UniformInfo >& uniformBindings_) {

    std::vector< VkDescriptorSetLayoutBinding > bindings;
    bindings.resize(uniformBindings_.size());

    for (size_t i = 0; i < uniformBindings_.size(); i++) {

        VkDescriptorSetLayoutBinding binding                    = {};
        binding.binding                                         = uniformBindings_[i].binding;
        binding.descriptorType                                  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        binding.descriptorCount                                 = 1;
        binding.stageFlags                                      = uniformBindings_[i].stageFlags;

        bindings[i] = binding;

    }

    VkDescriptorSetLayoutCreateInfo layoutCreateInfo            = {};
    layoutCreateInfo.sType                                      = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutCreateInfo.bindingCount                               = static_cast< uint32_t >(bindings.size());
    layoutCreateInfo.pBindings                                  = bindings.data();

    vk::engine.result = vkCreateDescriptorSetLayout(
        vk::engine.logicalDevice,
        &layoutCreateInfo,
        vk::engine.allocator,
        &descriptorSetLayout
        );
    ASSERT(vk::engine.result, "Failed to create descriptor set layout", VK_SC_DESCRIPTOR_SET_LAYOUT_CREATION_ERROR);

    VkDescriptorPoolSize descriptorPoolSize                     = {};
    descriptorPoolSize.type                                     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorPoolSize.descriptorCount                          = static_cast< uint32_t >(vk::engine.swapchainImages.size());

    VkDescriptorPoolCreateInfo descriptorPoolCreateInfo         = {};
    descriptorPoolCreateInfo.sType                              = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolCreateInfo.poolSizeCount                      = 1;
    descriptorPoolCreateInfo.pPoolSizes                         = &descriptorPoolSize;
    descriptorPoolCreateInfo.maxSets                            = static_cast< uint32_t >(vk::engine.swapchainImages.size());

    vk::engine.result = vkCreateDescriptorPool(
        vk::engine.logicalDevice,
        &descriptorPoolCreateInfo,
        vk::engine.allocator,
        &descriptorPool
    );
    ASSERT(vk::engine.result, "Failed to create descriptor pool", VK_SC_DESCRIPTOR_POOL_ERROR);

    std::vector< VkDescriptorSetLayout > layouts(vk::engine.swapchainImages.size(), descriptorSetLayout);

    VkDescriptorSetAllocateInfo allocateInfo        = {};
    allocateInfo.sType                              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocateInfo.descriptorPool                     = descriptorPool;
    allocateInfo.descriptorSetCount                 = static_cast< uint32_t >(vk::engine.swapchainImages.size());
    allocateInfo.pSetLayouts                        = layouts.data();

    descriptorSets.resize(vk::engine.swapchainImages.size());
    vk::engine.result = vkAllocateDescriptorSets(vk::engine.logicalDevice, &allocateInfo, descriptorSets.data());
    ASSERT(vk::engine.result, "Failed to allocate descriptor sets", VK_SC_DESCRIPTOR_SET_CREATION_ERROR);

    for (size_t i = 0; i < vk::engine.swapchainImages.size(); i++) {

        VkWriteDescriptorSet writeDescriptorSet         = {};
        writeDescriptorSet.sType                        = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescriptorSet.dstSet                       = descriptorSets[i];
        writeDescriptorSet.dstBinding                   = 0;
        writeDescriptorSet.dstArrayElement              = 0;
        writeDescriptorSet.descriptorType               = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writeDescriptorSet.descriptorCount              = 1;
        writeDescriptorSet.pBufferInfo                  = &(uniformBindings_[i].bufferInfo);

        vkUpdateDescriptorSets(
            vk::engine.logicalDevice,
            1,
            &writeDescriptorSet,
            0,
            nullptr
            );

    }

}

DescriptorSet::~DescriptorSet() {

    vkDestroyDescriptorSetLayout(vk::engine.logicalDevice, descriptorSetLayout, vk::engine.allocator);

    vkDestroyDescriptorPool(vk::engine.logicalDevice, descriptorPool, vk::engine.allocator);
    logger::log(EVENT_LOG, "Successfully destroyed descriptor pool");

}
