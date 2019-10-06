/**
    Implements the DescriptorPool wrapper

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         DescriptorPool.cpp
    @brief        Implementation of the DescriptorPool wrapper
*/
#include "DescriptorPool.hpp"
#include "VK.hpp"
#include "ASSERT.cpp"


DescriptorPool::DescriptorPool(const DescriptorSetLayout* layout_) {

    std::vector< VkDescriptorPoolSize > descriptorPoolSizes;
    descriptorPoolSizes.resize(layout_->descriptors.size());

    for (uint32_t i = 0; i < layout_->descriptors.size(); i++) {

        descriptorPoolSizes[i].type                         = layout_->descriptors[i].info.type;
        descriptorPoolSizes[i].descriptorCount              = static_cast< uint32_t > (vk::core::swapchainImages.size());

    }

    VkDescriptorPoolCreateInfo descriptorPoolCreateInfo     = {};
    descriptorPoolCreateInfo.sType                          = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolCreateInfo.poolSizeCount                  = static_cast< uint32_t >(descriptorPoolSizes.size());
    descriptorPoolCreateInfo.pPoolSizes                     = descriptorPoolSizes.data();
    descriptorPoolCreateInfo.maxSets                        = static_cast< uint32_t >(vk::core::swapchainImages.size());

    VkResult result = vkCreateDescriptorPool(
        vk::core::logicalDevice,
        &descriptorPoolCreateInfo,
        vk::core::allocator,
        &descriptorPool
        );
    ASSERT(result, "Failed to create descriptor pool", VK_SC_DESCRIPTOR_POOL_ERROR);

}

DescriptorPool::~DescriptorPool() {

    vkDestroyDescriptorPool(vk::core::logicalDevice, descriptorPool, vk::core::allocator);

}