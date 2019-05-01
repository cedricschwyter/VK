/**
	Implements the ShaderStage wrapper for a Vulkan shader stage

	@author		D3PSI
	@version	0.0.1 02.12.2019

	@file		ShaderStage.cpp
	@brief		Implementation of the ShaderStage wrapper for a Vulkan shader stage
*/
#include "VertFragShaderStages.hpp"


VertFragShaderStages::VertFragShaderStages(const char* vertPath_, const char* fragPath_) {

	auto vertCode = vk::loadFile(vertPath_);
	auto fragCode = vk::loadFile(fragPath_);

	vertModule = createShaderModuleFromBinary(vertCode);
	fragModule = createShaderModuleFromBinary(fragCode);

	vertStageInfo.sType									= VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertStageInfo.stage									= VK_SHADER_STAGE_VERTEX_BIT;
	vertStageInfo.module								= vertModule;
	vertStageInfo.pName									= "main";

	fragStageInfo.sType									= VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragStageInfo.stage									= VK_SHADER_STAGE_FRAGMENT_BIT;
	fragStageInfo.module								= fragModule;
	fragStageInfo.pName									= "main";

}

VK_STATUS_CODE VertFragShaderStages::destroyModules() {

	vkDestroyShaderModule(VKEngine::logicalDevice, vertModule, VKEngine::allocator);
	vkDestroyShaderModule(VKEngine::logicalDevice, fragModule, VKEngine::allocator);

	return VK_SC_SUCCESS;

}

VkShaderModule VertFragShaderStages::createShaderModuleFromBinary(const std::vector< char >& code_) {

	VkShaderModuleCreateInfo shaderModuleCreateInfo			= {};
	shaderModuleCreateInfo.sType							= VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModuleCreateInfo.codeSize							= code_.size();
	shaderModuleCreateInfo.pCode							= reinterpret_cast< const uint32_t* >(code_.data());

	VkShaderModule module;
	VKEngine::result = vkCreateShaderModule(
		VKEngine::logicalDevice, 
		&shaderModuleCreateInfo, 
		VKEngine::allocator,
		&module
		);
	ASSERT(VKEngine::result, "Failed to create shader module", VK_SC_SHADER_MODULE_CREATION_ERROR);

	return module;

}

const std::array< VkPipelineShaderStageCreateInfo, 2 >* VertFragShaderStages::getStageInfos() {

	std::array< VkPipelineShaderStageCreateInfo, 2 > stages = { vertStageInfo, fragStageInfo };

	return &stages;

}