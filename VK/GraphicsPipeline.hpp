/**
    Defines the GraphicsPipeline wrapper for a Vulkan graphics pipeline

    @author        D3PSI
    @version    0.0.1 02.12.2019

    @file        GraphicsPipeline.hpp
    @brief        Definition of the GraphicsPipeline wrapper for a Vulkan graphics pipeline
*/
#pragma once
#include <vulkan/vulkan.h>

#include "VertFragShaderStages.hpp"
#include "DescriptorSetLayout.hpp"

class GraphicsPipeline
{
public:

    VkPipelineLayout                      pipelineLayout;
    VkPipeline                            pipeline;

    /**
        Default constructor
    */
    GraphicsPipeline(void) = default;

    /**
        Constructor

        @param        vertPath_                           (Relative) file path to the SPIR-V compiled vertex shader file
        @param        fragPath_                           (Relative) file path to the SPIR-V compiled fragment shader file
        @param        vertexInputStateCreateInfo_         A pointer to a vertex input state create info structure
        @param        inputAssemblyStateCreateInfo_       A pointer to a input assembly state create info structure
        @param        viewportStateCreateInfo_            A pointer to a viewport state create info structure
        @param        rasterizationStateCreateInfo_       A pointer to a rasterization state create info structure
        @param        multisampleStateCreateInfo_         A pointer to a multisample state create info structure
        @param        depthStencilStateCreateInfo_        A pointer to a depth stencil state create info structure
        @param        colorBlendAttachmentState_          A pointer to a color blend attachment state structure
        @param        colorBlendStateCreateInfo_          A pointer to a color blend state create info structure
        @param        dynamicStateCreateInfo_             A pointer to a dynamic state create info structure
        @param        descriptorSetLayout_                A descriptor set layout
        @param        renderPass_                         A valid VkRenderPass handle
    */
    GraphicsPipeline(
        const char*                                         vertPath_, 
        const char*                                         fragPath_,
        const VkPipelineVertexInputStateCreateInfo*         vertexInputStateCreateInfo_,
        const VkPipelineInputAssemblyStateCreateInfo*       inputAssemblyStateCreateInfo_,
        const VkPipelineViewportStateCreateInfo*            viewportStateCreateInfo_,
        const VkPipelineRasterizationStateCreateInfo*       rasterizationStateCreateInfo_,
        const VkPipelineMultisampleStateCreateInfo*         multisampleStateCreateInfo_,
        const VkPipelineDepthStencilStateCreateInfo*        depthStencilStateCreateInfo_,
        const VkPipelineColorBlendAttachmentState*          colorBlendAttachmentState_,
        const VkPipelineColorBlendStateCreateInfo*          colorBlendStateCreateInfo_,
        const VkPipelineDynamicStateCreateInfo*             dynamicStateCreateInfo_,
        const DescriptorSetLayout*                          descriptorSetLayout_,
        VkRenderPass                                        renderPass_
        );

    /**
        Destroys the VkShaderModules
        
        @return        Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE destroyShaderModules(void);

    /**
        Destroys all other allocated resources of the graphics pipeline

        @return        Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE destroy(void);

private:

    VertFragShaderStages        stages;

};

