/**
    Defines the DescriptorSet wrapper

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         DescriptorSet.hpp
    @brief        Definition of the DescriptorSet wrapper
*/
#pragma once
#include <vector>

#include "UniformInfo.cpp"
#include "DescriptorSetLayout.hpp"
#include "DescriptorPool.hpp"
#include "GraphicsPipeline.hpp"

class DescriptorSet
{
public:

    DescriptorPool*                         descriptorPool;
    DescriptorSetLayout*                    descriptorSetLayout;
    std::vector< VkDescriptorSet >          descriptorSets;

    /**
        Default constructor
    */
    DescriptorSet(void) = default;

    /**
        Constructor

        @param      descriptors_        An std::vector containing the actual descriptors
    */
    DescriptorSet(std::vector< Descriptor > descriptors_);

    /**
        Binds the descriptor set

        @param      commandBuffers_     The command buffers to be recorded
        @param      imageIndex_         The swapchain image index
        @param      pipeline_           The pipeline that the command buffer is recorded for
    */
    void bind(std::vector< VkCommandBuffer >& commandBuffers_, uint32_t imageIndex_, GraphicsPipeline pipeline_);

    /**
        Updates a descriptor sets info

        @param      descriptors_        The new desriptors
    */
    void update(std::vector< Descriptor > descriptors_);

    /**
        Default destructor
    */
    ~DescriptorSet(void);

};

