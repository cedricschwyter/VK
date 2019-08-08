/**
    Defines the DescriptorSetLayout wrapper

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         DescriptorSetLayout.hpp
    @brief        Definition of the DescriptorSetLayout wrapper
*/
#ifndef DESCRIPTOR_SET_LAYOUT_HPP
#define DESCRIPTOR_SET_LAYOUT_HPP
#include <vulkan/vulkan.h>

#include <vector>

#include "UniformInfo.cpp"
#include "Descriptor.hpp"

class DescriptorSetLayout
{
public:

    VkDescriptorSetLayout           descriptorSetLayout;
    std::vector< Descriptor >       descriptors;

    /**
        Default constructor
    */
    DescriptorSetLayout(void) = default;

    /**
        Constructor

        @param      uniformBindings_        A reference of an std::vector of UniformInfo structs defining an interface between application and shaders
    */
    DescriptorSetLayout(const std::vector< Descriptor >& descriptors_);

    /**
        Default destructor
    */
    ~DescriptorSetLayout(void);

};
#endif  // DESCRIPTOR_SET_LAYOUT_HPP
