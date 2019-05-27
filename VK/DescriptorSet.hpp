/**
    Defines the DescriptorSet wrapper

    @author		D3PSI
    @version	0.0.1 02.12.2019

    @file		DescriptorSet.hpp
    @brief		Definition of the DescriptorSet wrapper
*/
#pragma once
#include <vector>

#include "UniformInfo.cpp"

class DescriptorSet
{
public:

    VkDescriptorPool                        descriptorPool              = VK_NULL_HANDLE;
    VkDescriptorSetLayout                   descriptorSetLayout         = VK_NULL_HANDLE;
    std::vector< VkDescriptorSet >          descriptorSets;

    /**
        Default constructor
    */
    DescriptorSet(void);

    /**
        Constructor

        @param      uniformBindings_        A reference of an std::vector of UniformInfo structs defining an interface between application and shaders
        @param      numUniforms_            Number of different bindings
    */
    DescriptorSet(const std::vector< UniformInfo >& uniformBindings_, uint32_t numUniforms_);

    /**
        Default destructor
    */
    ~DescriptorSet(void);
};

