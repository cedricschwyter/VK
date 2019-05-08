/**
	Defines the VertexBuffer class, inheriting BaseBuffer

	@author		D3PSI
	@version	0.0.1 02.12.2019

	@file		VertexBuffer.hpp
	@brief		Definition of the VertexBuffer class
*/
#pragma once
#include "BaseBuffer.hpp"

class VertexBuffer :
	public BaseBuffer
{
public:

	/**
		Constructor
		
		@param		bufferCreateInfo_		A pointer to a VkBufferCreateInfo structure
	*/
	VertexBuffer(const VkBufferCreateInfo* bufferCreateInfo_);

};

