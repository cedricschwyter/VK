/**
    Implementation the Mesh class

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         Mesh.cpp
    @brief        Implementation of the Mesh class
*/
#include "Mesh.hpp"
#include "VK.hpp"
#include "ASSERT.cpp"


Mesh::Mesh(
    GraphicsPipeline&                                               pipeline_, 
    std::vector< BaseVertex >&                                      vertices_, 
    std::vector< uint32_t >&                                        indices_,
    std::vector< TextureObject >&                                   textures_
    )
    : pipeline(pipeline_), vertices(vertices_), indices(indices_), textures(textures_) {

    QueueFamily family                                          = vk::engine->findSuitableQueueFamily(vk::engine->physicalDevice);

    std::vector< uint32_t > queueFamilyIndices                  = { family.graphicsFamilyIndex.value(), family.transferFamilyIndex.value() };

    VkBufferCreateInfo vertexBufferCreateInfo                   = {};
    vertexBufferCreateInfo.sType                                = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    vertexBufferCreateInfo.size                                 = sizeof(vertices[0]) * vertices.size();
    vertexBufferCreateInfo.usage                                = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    vertexBufferCreateInfo.sharingMode                          = VK_SHARING_MODE_CONCURRENT;
    vertexBufferCreateInfo.queueFamilyIndexCount                = static_cast< uint32_t >(queueFamilyIndices.size());
    vertexBufferCreateInfo.pQueueFamilyIndices                  = queueFamilyIndices.data();

    vertexBuffer                                                = new VertexBuffer(&vertexBufferCreateInfo, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    VK_STATUS_CODE res                                          = vertexBuffer->fillS(vertices.data(), sizeof(vertices[0]) * vertices.size());
    ASSERT(res, "Failed to fill vertex buffer", VK_SC_VERTEX_BUFFER_MAP_ERROR);

    VkBufferCreateInfo indexBufferCreateInfo                    = {};
    indexBufferCreateInfo.sType                                 = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    indexBufferCreateInfo.size                                  = sizeof(indices[0]) * indices.size();
    indexBufferCreateInfo.usage                                 = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    indexBufferCreateInfo.sharingMode                           = VK_SHARING_MODE_CONCURRENT;
    indexBufferCreateInfo.queueFamilyIndexCount                 = static_cast< uint32_t >(queueFamilyIndices.size());
    indexBufferCreateInfo.pQueueFamilyIndices                   = queueFamilyIndices.data();

    indexBuffer                                                 = new IndexBuffer(&indexBufferCreateInfo, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    res                                                         = indexBuffer->fillS(indices.data(), sizeof(indices[0]) * indices.size());
    ASSERT(res, "Failed to fill index buffer", VK_SC_INDEX_BUFFER_MAP_ERROR);

}

std::vector< Descriptor > Mesh::getDescriptors() {

    std::vector< Descriptor > descriptors;

    for (unsigned int i = 0; i < textures.size(); i++) {

        VkDescriptorImageInfo imageInfo     = {};
        imageInfo.sampler                   = reinterpret_cast< TextureImage* >(textures[i].img)->imgSampler;
        imageInfo.imageLayout               = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView                 = reinterpret_cast< TextureImage* >(textures[i].img)->imgView;

        UniformInfo samplerInfo             = {};
        samplerInfo.binding                 = textures[i].type;
        samplerInfo.stageFlags              = VK_SHADER_STAGE_FRAGMENT_BIT;
        samplerInfo.imageInfo               = imageInfo;
        samplerInfo.type                    = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

        descriptors.push_back(Descriptor(samplerInfo));

    }

    return descriptors;

}

void Mesh::draw(std::vector< VkCommandBuffer >& commandBuffers_, uint32_t imageIndex_) {

    std::vector< VkDeviceSize > offsets = { 0 };

    vkCmdBindVertexBuffers(
        commandBuffers_[imageIndex_],
        0,
        1,
        &(vertexBuffer->buf),
        offsets.data()
        );

    vkCmdBindIndexBuffer(
        commandBuffers_[imageIndex_],
        indexBuffer->buf,
        0,
        VK_INDEX_TYPE_UINT32
        );

    vkCmdDrawIndexed(
        commandBuffers_[imageIndex_],
        static_cast< uint32_t >(indices.size()),
        1,
        0,
        0,
        0
        );

}

Mesh::~Mesh() {

    delete vertexBuffer;
    delete indexBuffer;

}