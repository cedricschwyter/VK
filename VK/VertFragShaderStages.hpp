/**
	Defines the ShaderStage wrapper for a Vulkan shader stage

	@author		D3PSI
	@version	0.0.1 02.12.2019

	@file		ShaderStage.hpp
	@brief		Definition of the ShaderStage for a Vulkan shader stage
*/
#pragma once
#include <vulkan/vulkan.h>

#include <array>

#include "Logger.hpp"
#include "VK.hpp"

class VertFragShaderStages
{
public:

	/**
		Default constructor

		@param		vertPath_		(Relative) path to the SPIR-V-compiled vertex shader file
		@param		fragPath_		(Relative) path to the SPIR-V-compile fragment shader file
	*/
	VertFragShaderStages(const char* vertPath_, const char* fragPath_);

	/**
		Get a pointer to an array containing the two shader stage create infos

		@return		Returns a pointer to an array containing the shader stage create infos
	*/
	const std::array< VkPipelineShaderStageCreateInfo, 2 >* getStageInfos(void);

	/**
		Destroys allocated resources and handles cleaning of shader modules

		@return		Returns VK_SC_SUCCESS on success
	*/
	VK_STATUS_CODE destroyModules(void);

private:

	VkShaderModule vertModule;
	VkShaderModule fragModule;
	VkPipelineShaderStageCreateInfo vertStageInfo;
	VkPipelineShaderStageCreateInfo fragStageInfo;

	/**
		Creates a VkShaderModule handle from binary code

		@return		Returns a VkShaderModule handle
	*/
	VkShaderModule createShaderModuleFromBinary(const std::vector< char >& code_);

};

