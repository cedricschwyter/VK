/**
	Defines the VertFragShaderStages wrapper for a Vulkan shader stage

	@author		D3PSI
	@version	0.0.1 02.12.2019

	@file		VertFragShaderStages.hpp
	@brief		Definition of the VertFragShaderStages for a Vulkan shader stage
*/
#pragma once
#include <vulkan/vulkan.h>

#include <vector>
#include <array>

#include "Logger.hpp"
#include "VK_STATUS_CODE.hpp"

class VertFragShaderStages
{
public:

	std::vector< VkPipelineShaderStageCreateInfo >		stages;

	/**
		Default constructor
	*/
	VertFragShaderStages(void);

	/**
		Constructor

		@param		vertPath_		(Relative) path to the SPIR-V-compiled vertex shader file
		@param		fragPath_		(Relative) path to the SPIR-V-compile fragment shader file
	*/
	VertFragShaderStages(const char* vertPath_, const char* fragPath_);

	/**
		Destroys allocated resources and handles cleaning of shader modules

		@return		Returns VK_SC_SUCCESS on success
	*/
	VK_STATUS_CODE destroyModules(void);

private:

	VkShaderModule										vertModule;
	VkShaderModule										fragModule;
	VkPipelineShaderStageCreateInfo						vertStageInfo;
	VkPipelineShaderStageCreateInfo						fragStageInfo;

	/**
		Creates a VkShaderModule handle from binary code

		@param		code_		A pointer to an array containing the bytecode

		@return		Returns a VkShaderModule handle
	*/
	VkShaderModule createShaderModuleFromBinary(const std::vector< char >* code_);

};

