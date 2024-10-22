/**
    Implements the VertFragShaderStages wrapper for a Vulkan shader stage

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         VertFragShaderStages.cpp
    @brief        Implementation of the VertFragShaderStages wrapper for a Vulkan shader stage
*/
#include "VertFragShaderStages.hpp"
#include "VK.hpp"
#include "ASSERT.cpp"

VertFragShaderStages::VertFragShaderStages() {



}

VertFragShaderStages::VertFragShaderStages(const char* vertPath_, const char* fragPath_) {

    const std::vector< char > vertCode                      = vk::loadFile(vertPath_);
    const std::vector< char > fragCode                      = vk::loadFile(fragPath_);

    vertModule                                              = createShaderModuleFromBinary(&vertCode);
    fragModule                                              = createShaderModuleFromBinary(&fragCode);

    vertStageInfo                                           = {};
    vertStageInfo.sType                                     = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertStageInfo.stage                                     = VK_SHADER_STAGE_VERTEX_BIT;
    vertStageInfo.module                                    = vertModule;
    vertStageInfo.pName                                     = "main";

    fragStageInfo                                           = {};
    fragStageInfo.sType                                     = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragStageInfo.stage                                     = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragStageInfo.module                                    = fragModule;
    fragStageInfo.pName                                     = "main";

    stages                                                  = { vertStageInfo, fragStageInfo };

}

VK_STATUS_CODE VertFragShaderStages::destroyModules() {

    vkDestroyShaderModule(vk::core::logicalDevice, vertModule, vk::core::allocator);
    vkDestroyShaderModule(vk::core::logicalDevice, fragModule, vk::core::allocator);

    return vk::errorCodeBuffer;

}

VkShaderModule VertFragShaderStages::createShaderModuleFromBinary(const std::vector< char >* code_) {

    logger::log(EVENT_LOG, "Creating shader module...");

    VkShaderModuleCreateInfo shaderModuleCreateInfo            = {};
    shaderModuleCreateInfo.sType                            = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderModuleCreateInfo.codeSize                            = code_->size();
    shaderModuleCreateInfo.pCode                            = reinterpret_cast< const uint32_t* >(code_->data());

    VkShaderModule module;
    VkResult result = vkCreateShaderModule(
        vk::core::logicalDevice,
        &shaderModuleCreateInfo, 
        vk::core::allocator,
        &module
        );
    ASSERT(result, "Failed to create shader module", VK_SC_SHADER_MODULE_CREATION_ERROR);

    logger::log(EVENT_LOG, "Successfully created shader module");

    return module;

}