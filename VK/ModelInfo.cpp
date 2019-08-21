/**
    Defines the ModelInfo struct

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         ModelInfo.cpp
    @brief        Definition of the ModelInfo struct
*/
#ifndef MODEL_INFO_CPP
#define MODEL_INFO_CPP
#include "Model.hpp"

/**
    Defines a struct to hold loading information for models
*/
struct ModelInfo {

    const char*                     path;
    GraphicsPipeline&               pipeline;
    VKEngineModelLoadingLib         lib;
    std::function< glm::mat4() >    modelMatrixLambda;

};
#endif  // MODEL_INFO_CPP
