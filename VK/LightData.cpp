/**
    Defines the LightData struct

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         LightData.cpp
    @brief        Definition of the LightData struct
*/ 
#ifndef LIGHT_DATA_CPP
#define LIGHT_DATA_CPP

#include <glm/glm.hpp>

/**
    Used to pass lighting information to the shaders as you can do this only using structs
*/
struct LightData {

    alignas(16) glm::vec3 lightPos;
    alignas(16) glm::vec3 viewPos;
    alignas(16) glm::vec3 lightCol;

};

#endif  // LIGHT_DATA_CPP