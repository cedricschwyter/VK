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


Mesh::Mesh(void* vertices_, void* indices_, void* textures_) 
    : vertices(vertices_), indices(indices_), textures(textures_) {

    QueueFamily family                                          = vk::engine.findSuitableQueueFamily(vk::engine.physicalDevice);

    std::vector< uint32_t > queueFamilyIndices                  = { family.graphicsFamilyIndex.value(), family.transferFamilyIndex.value() };

    VkBufferCreateInfo vertexBufferCreateInfo                   = {};
    vertexBufferCreateInfo.sType                                = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    vertexBufferCreateInfo.size                                 = sizeof(vk::vertices[0]) * vk::vertices.size();
    vertexBufferCreateInfo.usage                                = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    vertexBufferCreateInfo.sharingMode                          = VK_SHARING_MODE_CONCURRENT;
    vertexBufferCreateInfo.queueFamilyIndexCount                = static_cast< uint32_t >(queueFamilyIndices.size());
    vertexBufferCreateInfo.pQueueFamilyIndices                  = queueFamilyIndices.data();

    vertexBuffer                                                = new VertexBuffer(&vertexBufferCreateInfo, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    VK_STATUS_CODE res                                          = vertexBuffer->fillS(vk::vertices.data(), sizeof(vk::vertices[0]) * vk::vertices.size());
    ASSERT(res, "Failed to fill vertex buffer", VK_SC_VERTEX_BUFFER_MAP_ERROR);

    VkBufferCreateInfo indexBufferCreateInfo                    = {};
    indexBufferCreateInfo.sType                                 = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    indexBufferCreateInfo.size                                  = sizeof(vk::indices[0]) * vk::indices.size();
    indexBufferCreateInfo.usage                                 = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    indexBufferCreateInfo.sharingMode                           = VK_SHARING_MODE_CONCURRENT;
    indexBufferCreateInfo.queueFamilyIndexCount                 = static_cast< uint32_t >(queueFamilyIndices.size());
    indexBufferCreateInfo.pQueueFamilyIndices                   = queueFamilyIndices.data();

    indexBuffer                                                 = new IndexBuffer(&indexBufferCreateInfo, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    res                                                         = indexBuffer->fillS(vk::indices.data(), sizeof(vk::indices[0]) * vk::indices.size());
    ASSERT(res, "Failed to fill index buffer", VK_SC_INDEX_BUFFER_MAP_ERROR);

    //  TODO: Set uniforms etc.

}

void Mesh::bind() {

    // TODO: Enumerate correct uniform name and write data to it

}
