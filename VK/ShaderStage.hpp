/**
	Defines the ShaderStage wrapper for a Vulkan shader stage

	@author		D3PSI
	@version	0.0.1 02.12.2019

	@file		ShaderStage.hpp
	@brief		Definition of the ShaderStage for a Vulkan shader stage
*/
#pragma once
class ShaderStage
{
public:

	/**
		Default constructor

		@param		vertPath_		(Relative) path to the SPIR-V-compiled vertex shader file
		@param		fragPath_		(Relative) path to the SPIR-V-compile fragment shader file
	*/
	ShaderStage(const char* vertPath_, const char* fragPath_);
};

