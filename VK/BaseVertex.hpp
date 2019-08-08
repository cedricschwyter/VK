/**
    Defines the BaseVertex struct

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         BaseVertex.hpp
    @brief        Definition of the BaseVertex struct
*/
#ifndef BASE_VERTEX_HPP
#define BASE_VERTEX_HPP
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <array>
#include <functional>

namespace vk {

    /**
        Defines a templated hash-function to combine the fields of structs for a decent quality hash function

        @param      hash_       The previously computed hash
        @param      field_      The field to add to the hash
    */
    template < class T >
    void hashField(std::size_t& hash_, const T& field_) {

        std::hash< T > hashFunc;
        hash_ ^= hashFunc(field_) + 0x9e3779b9 + (hash_ << 6) + (hash_ >> 2);

    }

}

struct BaseVertex {

    alignas(16) glm::vec3 pos;
    alignas(16) glm::vec3 nor;
    alignas(16) glm::vec2 tex;
    alignas(16) glm::vec3 tan;
    alignas(16) glm::vec3 bit;

    static VkVertexInputBindingDescription getBindingDescription() {
    
        VkVertexInputBindingDescription vertexInputBindingDescription                               = {};
        vertexInputBindingDescription.binding                                                       = 0;
        vertexInputBindingDescription.stride                                                        = sizeof(BaseVertex);
        vertexInputBindingDescription.inputRate                                                     = VK_VERTEX_INPUT_RATE_VERTEX;

        return vertexInputBindingDescription;
    
    }

    static std::array< VkVertexInputAttributeDescription, 5 > getAttributeDescriptions() {
    
        std::array< VkVertexInputAttributeDescription, 5 > vertexInputAttributeDescriptions         = {};

        vertexInputAttributeDescriptions[0].binding                                                 = 0;
        vertexInputAttributeDescriptions[0].location                                                = 0;
        vertexInputAttributeDescriptions[0].format                                                  = VK_FORMAT_R32G32B32_SFLOAT;            
        vertexInputAttributeDescriptions[0].offset                                                  = offsetof(BaseVertex, pos);

        vertexInputAttributeDescriptions[1].binding                                                 = 0;
        vertexInputAttributeDescriptions[1].location                                                = 1;
        vertexInputAttributeDescriptions[1].format                                                  = VK_FORMAT_R32G32B32_SFLOAT;
        vertexInputAttributeDescriptions[1].offset                                                  = offsetof(BaseVertex, nor);

        vertexInputAttributeDescriptions[2].binding                                                 = 0;
        vertexInputAttributeDescriptions[2].location                                                = 2;
        vertexInputAttributeDescriptions[2].format                                                  = VK_FORMAT_R32G32_SFLOAT;
        vertexInputAttributeDescriptions[2].offset                                                  = offsetof(BaseVertex, tex);

        vertexInputAttributeDescriptions[3].binding                                                 = 0;
        vertexInputAttributeDescriptions[3].location                                                = 3;
        vertexInputAttributeDescriptions[3].format                                                  = VK_FORMAT_R32G32B32_SFLOAT;
        vertexInputAttributeDescriptions[3].offset                                                  = offsetof(BaseVertex, tan);

        vertexInputAttributeDescriptions[4].binding                                                 = 0;
        vertexInputAttributeDescriptions[4].location                                                = 4;
        vertexInputAttributeDescriptions[4].format                                                  = VK_FORMAT_R32G32B32_SFLOAT;
        vertexInputAttributeDescriptions[4].offset                                                  = offsetof(BaseVertex, bit);

        return vertexInputAttributeDescriptions;
    
    }

    /**
        Overload comparison-operator
    */
    bool operator==(const BaseVertex& otherVertex) const {

        return pos == otherVertex.pos && nor == otherVertex.nor && tex == otherVertex.tex && tan == otherVertex.tan && bit == otherVertex.bit;

    }

};

/**
    Expand standard namespace
*/
namespace std {

    /**
        Hash-function for vertex !!!This is some high level C++!!! (following recommended approach for hash-functions on cppreference.com's std::hash page)
    */
    /**
        Hash-function for vertex !!!This is some high level C++!!! (following recommended approach for hash-functions on cppreference.com's std::hash page)
    */
    template<  > struct hash< BaseVertex > {

        size_t operator()(BaseVertex const& vertex) const noexcept {

            std::size_t result = 0;

            vk::hashField(result, vertex.pos);
            vk::hashField(result, vertex.nor);
            vk::hashField(result, vertex.tex);
            vk::hashField(result, vertex.tan);
            vk::hashField(result, vertex.bit);

            return result;

        }

    };

}
#endif  // BASE_VERTEX_HPP
