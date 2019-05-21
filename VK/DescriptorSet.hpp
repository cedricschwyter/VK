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

    VkDescriptorPool                        descriptorPool;
    VkDescriptorSetLayout                   descriptorSetLayout;
    std::vector< VkDescriptorSet >          descriptorSets;

    /**
        Default constructor
    */
    DescriptorSet(void);

    /**
        Constructor

        @param      uniformBindings_        A reference of an std::vector of UniformInfo structs defining an interface between application and shaders
    */
    DescriptorSet(const std::vector< UniformInfo >& uniformBindings_);

    /**
        Default destructor
    */
    ~DescriptorSet(void);
};

