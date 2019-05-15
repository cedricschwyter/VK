/**
    Defines the MVPBuffer class, inheriting BaseBuffer

    @author		D3PSI
    @version	0.0.1 02.12.2019

    @file		MVPBuffer.hpp
    @brief		Definition of the MVPBuffer class
*/
#pragma once
#include "BaseBuffer.hpp"
#include "MVPBufferObject.cpp"
#include "VK_STATUS_CODE.hpp"

class MVPBuffer :
    public BaseBuffer
{
    using BaseBuffer::BaseBuffer;
public:

    /**
        Fills the buffer memory

        @param      mvp_        The MVPBufferObject to upload to shader

        @return     Returns VK_SC_SUCCESS on success
    */
    //VK_STATUS_CODE fill(MVPBufferObject mvp_);

};

