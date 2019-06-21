/**
    Implementation the Model class

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         Model.cpp
    @brief        Implementation of the Model class
*/
#include "Model.hpp"
#include "VK.hpp"


Model::Model(const char* path_, GraphicsPipeline& pipeline_) : pipeline(pipeline_) {

    loadOBJ(path_);

}

void Model::bind() {



}

VK_STATUS_CODE Model::loadOBJ(const char* path_) {



    return vk::errorCodeBuffer;

}


void Model::processASSIMPNode(aiNode* node_, const aiScene* scene_) {

    

}


Mesh Model::processASSIMPMesh(aiMesh* mesh_, const aiScene* scene_) {



    return Mesh(nullptr, nullptr, nullptr);

}