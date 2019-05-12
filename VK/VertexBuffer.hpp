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
    using BaseBuffer::BaseBuffer;
public:
    
    /**
		Maps data to a buffer using a staging buffer

		@param		bufData_		Data of whatever sort which needs to be copied to the buffer

		@return		Returns VK_SC_SUCCESS on success
	*/
    VK_STATUS_CODE fill(std::vector< BaseVertex > bufData_);

};

