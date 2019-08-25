/**
    Declares the Model class

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         Model.hpp
    @brief        Declaration of the Model class
*/
#ifndef MODEL_HPP
#define MODEL_HPP
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>

#include <functional>

#include "GraphicsPipeline.hpp"
#include "Mesh.hpp"

/**
    Defines an enumeration for different model loading libraries to choose from
*/
typedef enum VKEngineModelLoadingLib {

    VKEngineModelLoadingLibASSIMP,
    VKEngineModelLoadingLibTINYOBJ

} VKEngineModelLoadingLib;

class Model
{
public:

    GraphicsPipeline                                            pipeline;
    std::vector< Mesh* >                                        meshes;

    /**
        Constructor

        @param      path_               The path to the .obj-file
        @param      pipeline_           The pipeline to render the model with
        @param      lib_                The VKEngineModelLoadingLib flag to tell the Model loader which library to use
        @param      modelMatrixFunc_    A function pointer to calculcate the model matrix for the model
    */
    Model(const char* path_, GraphicsPipeline& pipeline_, VKEngineModelLoadingLib lib_, glm::mat4 (*modelMatrixFunc_)());

    /**
        Returns the models model-matrix

        @return     Returns the models model-matrix
     */
    glm::mat4 getModelMatrix(void);

    /**
        Binds the model and the correct uniforms
    */
    void bind(void);

    /**
        Default destructor
    */
    ~Model(void);

private:

    std::string                                                 directory;
    std::vector< TextureObject >                                texturesLoaded;
    glm::mat4                                                   (*modelMatrix)();

    /**
        Handles and coordinates all loading actions for the specified file, using ASSIMP

        @param      path_       The path to the .obj-file to load

        @return     Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE loadOBJASSIMP(const char* path_);

    /**
        Handler and coordinates all loading actions for the specified file, using tinyobjloader

        @param      path_       The path to the .obj-file to load

        @return     Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE loadOBJTINYOBJ(const char* path_);

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

        @return     Returns a (parsed) Mesh-object pointer that VKEngine can work with
    */
    Mesh* processASSIMPMesh(aiMesh* mesh_, const aiScene* scene_);

    /**
        Helper function for tinyobj's mesh-loading system

        @param      mesh_       A pointer to tinyobj's mesh
        @param      attrib_     A pointer to tinyobj's attrib

        @return     Returns a (parsed) Mesh-object pointer that VKEngine can work with
    */
    Mesh* processTINYOBJMesh(void* mesh_, void* attrib_);

    /**
        Helper function for ASSIMP's texture loading system

        @param      material_       A pointer to ASSIMP's material
        @param      type_           ASSIMP texture type flags
        @param      typeID_         Own TEXTURE_TYPE type enumeration

        @return     Returns an std::vector of TextureObjects
    */
    std::vector< TextureObject > loadASSIMPMaterialTextures(aiMaterial* material_, aiTextureType type_, TEXTURE_TYPE typeID_);

    /**
        Loads a texture from a file

        @param      path_           The path (actually just the filename) to the file
        @param      directory_      A reference to a string containing the directory path
        @param      gamma_          Is the texture file already gamma corrected?

        @return     Returns a pointer to the texture
    */
    TextureImage* textureFromFile(const char* path_, const std::string& directory_, bool gamma_ = false);

};
#endif  // MODEL_HPP
