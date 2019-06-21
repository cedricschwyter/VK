/**
    Defines the MVPBufferObject struct

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         MVPBufferObject.cpp
    @brief        Definition of the MVPBufferObject struct
*/
#pragma once
#include <glm/glm.hpp>

/**
    Holds model, view and projection matrices
*/
struct MVPBufferObject {

    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;

};