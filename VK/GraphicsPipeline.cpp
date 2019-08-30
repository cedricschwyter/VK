/**
    Implements the GraphicsPipeline wrapper for a Vulkan graphics pipeline

    @author        D3PSI
    @version    0.0.1 02.12.2019

    @file        GraphicsPipeline.cpp
    @brief        Implementation of the GraphicsPipeline wrapper for a Vulkan graphics pipeline
*/
#include "GraphicsPipeline.hpp"
#include "VK.hpp"
#include "ASSERT.cpp"


GraphicsPipeline::GraphicsPipeline(
    const char*                                            vertPath_,
    const char*                                            fragPath_,
    const VkPipelineVertexInputStateCreateInfo*            vertexInputStateCreateInfo_,
    const VkPipelineInputAssemblyStateCreateInfo*          inputAssemblyStateCreateInfo_,
    const VkPipelineViewportStateCreateInfo*               viewportStateCreateInfo_,
    const VkPipelineRasterizationStateCreateInfo*          rasterizationStateCreateInfo_,
    const VkPipelineMultisampleStateCreateInfo*            multisampleStateCreateInfo_,
    const VkPipelineDepthStencilStateCreateInfo*           depthStencilStateCreateInfo_,
    const VkPipelineColorBlendAttachmentState*             colorBlendAttachmentState_,
    const VkPipelineColorBlendStateCreateInfo*             colorBlendStateCreateInfo_,
    const VkPipelineDynamicStateCreateInfo*                dynamicStateCreateInfo_,
    const VkPushConstantRange*                             pushConstants_,
    const uint32_t                                         pushConstantCount_,
    const DescriptorSetLayout*                             descriptorSetLayout_,
    VkRenderPass                                           renderPass_
    ) {

    stages = VertFragShaderStages(vertPath_, fragPath_);
    
    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo         = {};
    pipelineLayoutCreateInfo.sType                              = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCreateInfo.setLayoutCount                     = 1;
    pipelineLayoutCreateInfo.pSetLayouts                        = &(descriptorSetLayout_->descriptorSetLayout);
    pipelineLayoutCreateInfo.pushConstantRangeCount             = pushConstantCount_;
    pipelineLayoutCreateInfo.pPushConstantRanges                = pushConstants_;

    VkResult result = vkCreatePipelineLayout(
        vk::core::logicalDevice,
        &pipelineLayoutCreateInfo,
        vk::core::allocator,
        &pipelineLayout
        );
    ASSERT(result, "Failed to create pipeline layout", VK_SC_PIPELINE_LAYOUT_CREATION_ERROR);
    logger::log(EVENT_LOG, "Successfully created pipeline layout");

    VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo            = {};
    graphicsPipelineCreateInfo.sType                                   = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    graphicsPipelineCreateInfo.stageCount                              = 2;
    graphicsPipelineCreateInfo.pStages                                 = stages.stages.data();
    graphicsPipelineCreateInfo.pVertexInputState                       = vertexInputStateCreateInfo_;
    graphicsPipelineCreateInfo.pInputAssemblyState                     = inputAssemblyStateCreateInfo_;
    graphicsPipelineCreateInfo.pViewportState                          = viewportStateCreateInfo_;
    graphicsPipelineCreateInfo.pRasterizationState                     = rasterizationStateCreateInfo_;
    graphicsPipelineCreateInfo.pMultisampleState                       = multisampleStateCreateInfo_;
    graphicsPipelineCreateInfo.pDepthStencilState                      = depthStencilStateCreateInfo_;
    graphicsPipelineCreateInfo.pColorBlendState                        = colorBlendStateCreateInfo_;
    graphicsPipelineCreateInfo.pDynamicState                           = dynamicStateCreateInfo_;    
    graphicsPipelineCreateInfo.layout                                  = pipelineLayout;                    // Reference fixed-function stage
    graphicsPipelineCreateInfo.renderPass                              = renderPass_;
    graphicsPipelineCreateInfo.subpass                                 = 0;
    graphicsPipelineCreateInfo.basePipelineHandle                      = VK_NULL_HANDLE;                    // No base pipeline
    graphicsPipelineCreateInfo.basePipelineIndex                       = -1;

    result = vkCreateGraphicsPipelines(
        vk::core::logicalDevice,
        VK_NULL_HANDLE,                        // No pipeline cache will be used
        1,                                    // Create only one pipeline, might change in the future
        &graphicsPipelineCreateInfo,
        vk::core::allocator,
        &pipeline
        );
    ASSERT(result, "Failed to create graphics pipeline", VK_SC_GRAPHICS_PIPELINE_CREATION_ERROR);

    destroyShaderModules();

}

VK_STATUS_CODE GraphicsPipeline::destroyShaderModules() {

    return stages.destroyModules();

}

VK_STATUS_CODE GraphicsPipeline::destroy() {

    vkDestroyPipeline(vk::core::logicalDevice, pipeline, vk::core::allocator);
    logger::log(EVENT_LOG, "Successfully destroyed graphics pipeline");

    vkDestroyPipelineLayout(vk::core::logicalDevice, pipelineLayout, vk::core::allocator);
    logger::log(EVENT_LOG, "Successfully destroyed pipeline layout");

    return vk::errorCodeBuffer;

}
