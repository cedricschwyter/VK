/**
    Defines the BaseVertex struct

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         BaseVertex.hpp
    @brief        Definition of the BaseVertex struct
*/
#pragma once
#include <glm/glm.hpp>

#include <array>

struct BaseVertex {

    glm::vec3 pos;
    glm::vec2 tex;

    static VkVertexInputBindingDescription getBindingDescription() {
    
        VkVertexInputBindingDescription vertexInputBindingDescription                                = {};
        vertexInputBindingDescription.binding                                                        = 0;
        vertexInputBindingDescription.stride                                                         = sizeof(BaseVertex);
        vertexInputBindingDescription.inputRate                                                      = VK_VERTEX_INPUT_RATE_VERTEX;

        return vertexInputBindingDescription;
    
    }

    static std::array< VkVertexInputAttributeDescription, 2 > getAttributeDescriptions() {
    
        std::array< VkVertexInputAttributeDescription, 2 > vertexInputAttributeDescriptions          = {};
        vertexInputAttributeDescriptions[0].binding                                                  = 0;
        vertexInputAttributeDescriptions[0].location                                                 = 0;
        vertexInputAttributeDescriptions[0].format                                                   = VK_FORMAT_R32G32B32_SFLOAT;            
        vertexInputAttributeDescriptions[0].offset                                                   = offsetof(BaseVertex, pos);

        vertexInputAttributeDescriptions[1].binding                                                  = 0;
        vertexInputAttributeDescriptions[1].location                                                 = 1;
        vertexInputAttributeDescriptions[1].format                                                   = VK_FORMAT_R32G32_SFLOAT;
        vertexInputAttributeDescriptions[1].offset                                                   = offsetof(BaseVertex, tex);

        return vertexInputAttributeDescriptions;
    
    }

};

