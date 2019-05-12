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

class MVPBuffer :
    public BaseBuffer
{
    using BaseBuffer::BaseBuffer;
public:

    MVPBufferObject mvp;

    /**
        Default destructor
    */
    ~MVPBuffer(void);

};

