/**
    Implements the DescriptorSetLayout wrapper

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         DescriptorSetLayout.cpp
    @brief        Implementation of the DescriptorSetLayout wrapper
*/
#include "DescriptorSetLayout.hpp"
#include "ASSERT.cpp"


DescriptorSetLayout::DescriptorSetLayout(const std::vector< Descriptor >& descriptors_) : descriptors(descriptors_) {

    std::vector< VkDescriptorSetLayoutBinding > bindings;
    bindings.resize(descriptors_.size());

    for (size_t i = 0; i < descriptors_.size(); i++) {

        VkDescriptorSetLayoutBinding binding        = {};
        binding.binding                             = descriptors_[i].info.binding;
        binding.descriptorType                      = descriptors_[i].info.type;
        binding.descriptorCount                     = 1;
        binding.stageFlags                          = descriptors_[i].info.stageFlags;
        
        bindings[i] = binding;

    }

    VkDescriptorSetLayoutCreateInfo layoutCreateInfo    = {};
    layoutCreateInfo.sType                              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutCreateInfo.bindingCount                       = static_cast< uint32_t >(bindings.size());
    layoutCreateInfo.pBindings                          = bindings.data();

    VkResult result = vkCreateDescriptorSetLayout(
        VKCore::logicalDevice,
        &layoutCreateInfo,
        VKCore::allocator,
        &descriptorSetLayout
        );
    ASSERT(result, "Failed to create descriptor set layout", VK_SC_DESCRIPTOR_SET_LAYOUT_CREATION_ERROR);

}

DescriptorSetLayout::~DescriptorSetLayout() {

    vkDestroyDescriptorSetLayout(VKCore::logicalDevice, descriptorSetLayout, VKCore::allocator);
    logger::log(EVENT_LOG, "Successfully destroyed descriptor set layout");

}
