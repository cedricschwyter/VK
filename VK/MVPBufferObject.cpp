/**
    Defines the MVPBufferObject struct

    @author        D3PSI
    @version    0.0.1 02.12.2019

    @file        MVPBufferObject.cpp
    @brief        Definition of the MVPBufferObject struct
*/
#pragma once
#include <glm/glm.hpp>

/**
    Holds model, view and projection matrices
*/
struct MVPBufferObject {

    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;

};