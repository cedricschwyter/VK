/**
    Declares the Model class

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         Model.hpp
    @brief        Declaration of the Model class
*/
#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "GraphicsPipeline.hpp"
#include "Mesh.hpp"

class Model
{
public:

    GraphicsPipeline pipeline;

    /**
        Constructor

        @param      path_           The path to the .obj-file
        @param      pipeline_       The pipeline to render the model with
    */
    Model(const char* path_, GraphicsPipeline& pipeline_);

    /**
        Binds the model and the correct uniforms
    */
    void bind(void);

private:

    std::vector< Mesh > meshes;
    std::string directory;

    /**
        Handles and coordinates all loading actions for the specified file

        @param      path_       The path to the .obj-file to load

        @return     Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE loadOBJ(const char* path_);

    /**
        Helper function for ASSIMP's node-loading system

        @param      node_       A pointer to ASSIMP's node
        @param      scene_      A pointer to ASSIMP's scene
    */
    void processASSIMPNode(aiNode* node_, const aiScene* scene_);

    /**
        Helper function for ASSIMP's mesh-loading system

        @param      mesh_       A pointer to ASSIMP's mesh
        @param      scene_      A pointer to ASSIMP's scene

        @return     Returns a (parsed) Mesh-object that VKEngine can work with
    */
    Mesh processASSIMPMesh(aiMesh* mesh_, const aiScene* scene_);

};

