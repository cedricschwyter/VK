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

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>


Model::Model(const char* path_, GraphicsPipeline& pipeline_, VKEngineModelLoadingLib lib_) : pipeline(pipeline_) {

    VK_STATUS_CODE result;

    if (lib_ == VKEngineModelLoadingLibASSIMP) {

        result = loadOBJASSIMP(path_);

    }
    else if (lib_ == VKEngineModelLoadingLibTINYOBJ) {
    
        result = loadOBJTINYOBJ(path_);
    
    }

    ASSERT(result, "Error loading model using ASSIMP", VK_SC_RESOURCE_LOADING_ERROR);

}

void Model::bind() {

    for (uint32_t i = 0; i < meshes.size(); i++) {
    
        //meshes[i]->bind();
    
    }

}

VK_STATUS_CODE Model::loadOBJTINYOBJ(const char* path_) {

    tinyobj::attrib_t                       attrib;
    std::vector< tinyobj::shape_t >         shapes;
    std::vector< tinyobj::material_t >      materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path_)) {

        logger::log(ERROR_LOG, warn + err);

    }

    for (const auto& shape : shapes) {
    
        tinyobj::mesh_t mesh = (shape.mesh);
        meshes.push_back(processTINYOBJMesh(reinterpret_cast< void* >(&mesh), &attrib));
    
    }

    return vk::errorCodeBuffer;

}

VK_STATUS_CODE Model::loadOBJASSIMP(const char* path_) {

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


Mesh* Model::processASSIMPMesh(aiMesh* mesh_, const aiScene* scene_) {

    std::vector< BaseVertex >                                vertices;
    std::vector< uint32_t >                                  indices;
    std::pair< TextureObject, Descriptor >                   texture;
    MeshVertexInfo                                           vertexInfo             = {};
    vertexInfo.vertexBase                                                           = vertexCount;
    vertexInfo.indexBase                                                            = indexCount;

    vertexCount                 += mesh_->mNumVertices;
    vertexInfo.vertexCount       = mesh_->mNumVertices;

    std::unordered_map< BaseVertex, uint32_t >               uniqueVertices         = {};

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

        if (uniqueVertices.count(vertex) == 0) {

            uniqueVertices[vertex] = static_cast< uint32_t >(vertices.size());
            vertices.push_back(vertex);

            indices.push_back(uniqueVertices[vertex]);
            vertexInfo.indexCount++;
            indexCount++;

        }
    
    }

    std::vector< std::pair< TextureObject, Descriptor > > textures;

    static uint32_t meshIndex = 0;
    meshIndex++;

    aiMaterial* material = scene_->mMaterials[mesh_->mMaterialIndex];
    std::vector< std::pair< TextureObject, Descriptor > > diffuseMaps = loadASSIMPMaterialTextures(material, aiTextureType_DIFFUSE, TT_DIFFUSE);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    return new Mesh(
        pipeline, 
        vertices, 
        indices, 
        textures[meshIndex - 1],
        vertexInfo
        );

}

Mesh* Model::processTINYOBJMesh(void* mesh_, void* attrib_) {

    MeshVertexInfo vertexInfo = {};

    tinyobj::mesh_t*        mesh        = reinterpret_cast< tinyobj::mesh_t* >(mesh_);
    tinyobj::attrib_t*      attrib      = reinterpret_cast< tinyobj::attrib_t* >(attrib_);

    std::vector< BaseVertex >                                vertices;
    std::vector< uint32_t >                                  indices;
    std::pair< TextureObject, Descriptor >                   texture;
    std::unordered_map< BaseVertex, uint32_t >               uniqueVertices         = {};

    for (const auto& index : mesh->indices) {

        BaseVertex vertex = {};

        vertex.pos = {
            attrib->vertices[3 * index.vertex_index + 0],
            attrib->vertices[3 * index.vertex_index + 1],
            attrib->vertices[3 * index.vertex_index + 2]
        };

        vertex.tex = {
            attrib->texcoords[2 * index.texcoord_index + 0],
            1.0f - attrib->texcoords[2 * index.texcoord_index + 1]
        };

        if (uniqueVertices.count(vertex) == 0) {

            uniqueVertices[vertex] = static_cast< uint32_t >(vertices.size());
            vertices.push_back(vertex);

        }

        indices.push_back(uniqueVertices[vertex]);

    }

    return new Mesh(
        pipeline, 
        vertices, 
        indices, 
        texture,
        vertexInfo
        );

}

std::vector< std::pair< TextureObject, Descriptor > > Model::loadASSIMPMaterialTextures(aiMaterial* material_, aiTextureType type_, TEXTURE_TYPE typeID_) {

    std::vector< std::pair< TextureObject, Descriptor > > textures;

    for (uint32_t i = 0; i < material_->GetTextureCount(type_); i++) {

        aiString texturePath;
        material_->GetTexture(type_, i, &texturePath);

        bool skip = false;

        for (uint32_t j = 0; j < texturesLoaded.size(); j++) {

            if (std::strcmp(texturesLoaded[j].first.path.data(), texturePath.C_Str()) == 0) {

                textures.push_back(texturesLoaded[j]);
                skip = true;
                break;

            }

        }

        if (!skip) {

            logger::log(EVENT_LOG, "Loading texture at: " + directory + '/' + texturePath.C_Str());

            TextureObject texture;
            texture.img                         = textureFromFile(texturePath.C_Str(), directory);
            texture.type                        = typeID_;
            texture.path                        = texturePath.C_Str();

            VkDescriptorImageInfo imageInfo     = {};
            imageInfo.sampler                   = reinterpret_cast< TextureImage* >(texture.img)->imgSampler;
            imageInfo.imageLayout               = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView                 = reinterpret_cast< TextureImage* >(texture.img)->imgView;

            UniformInfo samplerInfo             = {};
            samplerInfo.binding                 = 1;
            samplerInfo.stageFlags              = VK_SHADER_STAGE_FRAGMENT_BIT;
            samplerInfo.imageInfo               = imageInfo;
            samplerInfo.type                    = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

            Descriptor samplerDescriptor        = Descriptor(&samplerInfo);

            textures.push_back(std::make_pair(texture, samplerDescriptor));
            texturesLoaded.push_back(std::make_pair(texture, samplerDescriptor));

            descriptors.push_back(samplerDescriptor);

        }

    }

    return textures;

}

TextureImage* Model::textureFromFile(const char *path_, const std::string& directory_, bool gamma_) {

    std::string path = std::string(directory + '/' + path_);

    TextureImage* img = new TextureImage(
        path.c_str(),
        VK_FORMAT_R8G8B8A8_UNORM,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

    return img;
    
}


Model::~Model() {

    for (auto img : texturesLoaded) {
    
        delete img.first.img;
    
    }

    for (auto mesh : meshes) {
    
        delete mesh;
    
    }

}