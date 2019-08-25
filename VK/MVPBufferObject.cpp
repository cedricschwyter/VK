/**
    Defines the VPBufferObject and MBufferObject structs

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         MVPBufferObjects.cpp
    @brief        Definition of the VPBufferObject and MBufferObject structs
*/
#ifndef MVP_BUFFER_OBJECT_CPP
#define MVP_BUFFER_OBJECT_CPP
#include <glm/glm.hpp>

#include "Version.hpp"

/**
    Holds view and projection matrices
*/
struct VPBufferObject {

    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;

};

/**
    Holds model matrix
*/
struct MBufferObject {

    alignas(16) glm::mat4 model;

};
#endif  // MVP_BUFFER_OBJECT_CPP