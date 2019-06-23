/**
    Implementation the Model class

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         Model.cpp
    @brief        Implementation of the Model class
*/
#include "Model.hpp"
#include "VK.hpp"
#include "ASSERT.cpp"


Model::Model(const char* path_, GraphicsPipeline& pipeline_) : pipeline(pipeline_) {

    VK_STATUS_CODE result = loadOBJ(path_);
    ASSERT(result, "Error loading model using ASSIMP", VK_SC_RESOURCE_LOADING_ERROR);

}

void Model::bind() {

    for (uint32_t i = 0; i < meshes.size(); i++) {
    
        meshes[i].bind();
    
    }

}

VK_STATUS_CODE Model::loadOBJ(const char* path_) {

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path_, aiProcess_Triangulate);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {

        std::string error = importer.GetErrorString();
        logger::log(ERROR_LOG, "Error loading model using ASSIMP: " + error);

        return VK_SC_MODEL_LOADING_ERROR_ASSIMP;

    }

    directory = (std::string(path_)).substr(0, (std::string(path_)).find_last_of("/"));
    processASSIMPNode(scene->mRootNode, scene);

    return vk::errorCodeBuffer;

}


void Model::processASSIMPNode(aiNode* node_, const aiScene* scene_) {

    // Process each of the meshes using iteration
    for (uint32_t i = 0; i < node_->mNumMeshes; i++) {
    
        aiMesh* mesh = scene_->mMeshes[node_->mMeshes[i]];
        meshes.push_back(processASSIMPMesh(mesh, scene_));
    
    }

    // Process each of ASSIMP's node's children using recursion in the same way
    for (uint32_t i = 0; i < node_->mNumChildren; i++) {
    
        processASSIMPNode(node_->mChildren[i], scene_);
    
    }

}


Mesh Model::processASSIMPMesh(aiMesh* mesh_, const aiScene* scene_) {

    std::vector< BaseVertex >       vertices;
    std::vector< uint32_t >         indices;
    std::vector< TextureObject >    textures;

    for (uint32_t i = 0; i < mesh_->mNumVertices; i++) {

        BaseVertex vertex;

        glm::vec3 vector;
        vector.x            = mesh_->mVertices[i].x;
        vector.y            = mesh_->mVertices[i].y;
        vector.z            = mesh_->mVertices[i].z;
        vertex.pos          = vector;

        if (mesh_->mTextureCoords[0]) {     // Does the mesh contain texture coordinates?
        
            glm::vec2 vec;
            vec.x           = mesh_->mTextureCoords[0][i].x;
            vec.y           = mesh_->mTextureCoords[0][i].y;
            vertex.tex      = vec;

        }
        else {

            vertex.tex      = glm::vec2(0.0f, 0.0f);        
        }

        vertices.push_back(vertex);
    
    }

    for (uint32_t i = 0; i < mesh_->mNumFaces; i++) {
    
        aiFace face = mesh_->mFaces[i];
        
        for (uint32_t j = 0; j < face.mNumIndices; j++) {
        
            indices.push_back(face.mIndices[j]);

        }

    }

    if (mesh_->mMaterialIndex >= 0) {
    
        aiMaterial* material = scene_->mMaterials[mesh_->mMaterialIndex];
        
        std::vector< TextureObject > diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, TT_DIFFUSE);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector< TextureObject > specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, TT_SPECULAR);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    
    }

    return Mesh(vertices.data(), indices.data(), textures.data());

}

std::vector< TextureObject > Model::loadMaterialTextures(aiMaterial* material_, aiTextureType type_, TEXTURE_TYPE typeID_) {

    std::vector< TextureObject > textures;

    for (uint32_t i = 0; i < material_->GetTextureCount(type_); i++) {
    
        aiString string;
        material_->GetTexture(type_, i, &string);
        
        bool skip = false;

        for (uint32_t j = 0; j < texturesLoaded.size(); j++) {

            if (std::strcmp(texturesLoaded[j].path, string.C_Str()) == 0) {

                textures.push_back(texturesLoaded[j]);
                skip = true;
                break;

            }

        }
        if(!skip) {

            TextureObject texture;
            texture.img         = textureFromFile(string.C_Str(), directory);
            texture.type        = typeID_;
            texture.path        = string.C_Str();
            textures.push_back(texture);
            texturesLoaded.push_back(texture);

        }

    }

    return textures;

}

TextureImage* Model::textureFromFile(const char *path_, const std::string& directory_, bool gamma_) {

    std::string path = std::string(directory + '/' + path_);

    TextureImage* img = new TextureImage(path.c_str(),
        VK_FORMAT_R8G8B8A8_UNORM,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

    return img;
    
}