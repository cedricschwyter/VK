/**
    Defines the MVPBuffer struct

    @author		D3PSI
    @version	0.0.1 02.12.2019

    @file		MVPBuffer.cpp
    @brief		Definition of the MVPBuffer class
*/
#pragma once
#include <glm/glm.hpp>

struct MVPBufferObject {

    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;

};