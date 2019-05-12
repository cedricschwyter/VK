/**
    Defines the IndexBuffer class, inheriting BaseBuffer

    @author		D3PSI
    @version	0.0.1 02.12.2019

    @file		IndexBuffer.hpp
    @brief		Definition of the IndexBuffer class
*/
#pragma once
#include <iostream>

#include "BaseBuffer.hpp"

class IndexBuffer :
    public BaseBuffer
{
    using BaseBuffer::BaseBuffer;
public:

    /**
        Fills the index buffer with the data supplied

        @param      bufData_        A pointer to the data to fill the buffer with

        @return     Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE fill(const std::vector< uint32_t >* bufData_);

};

