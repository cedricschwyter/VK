/**
    Defines the DescriptorPool wrapper

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         DescriptorPool.hpp
    @brief        Definition of the DescriptorPool wrapper
*/
#ifndef DESCRIPTOR_POOL_HPP
#define DESCRIPTOR_POOL_HPP
#include <vulkan/vulkan.h>

#include "DescriptorSetLayout.hpp"

class DescriptorPool
{
public:

    VkDescriptorPool descriptorPool;

    /**
        Constructor

        @param      layout_     The descriptor set layout
    */
    DescriptorPool(const DescriptorSetLayout* layout_);

    /**
        Default destructor
    */
    ~DescriptorPool(void);

};
#endif  // DESCRIPTOR_POOL_HPP
