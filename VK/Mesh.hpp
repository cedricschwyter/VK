/**
    Declares the Mesh class

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         Mesh.hpp
    @brief        Declaration of the Mesh class
*/
#pragma once
#include <vector>

#include "TextureObject.cpp"
#include "BaseBuffer.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "GraphicsPipeline.hpp"

class Mesh
{
public:

    GraphicsPipeline                                        pipeline;
    std::vector< BaseVertex >                               vertices;
    std::vector< uint32_t >                                 indices;
    BaseBuffer*                                             vertexBuffer;
    BaseBuffer*                                             indexBuffer;
    std::vector< TextureObject >                            textures;

    /**
        Constructor

        @param      pipeline_               The graphics pipeline to render the mesh with
        @param      vertices_               Reference to vertex data of mesh
        @param      indices_                Reference to index data of mesh
        @param      textures_               Reference to texturing data of mesh
    */
    Mesh(
        GraphicsPipeline&                                               pipeline_,
        std::vector< BaseVertex >&                                      vertices_,
        std::vector< uint32_t >&                                        indices_,
        std::vector< TextureObject >&                                   textures_
        );

    /**
        Returns the correct descriptor to bind

        @return     Returns an array of descriptor objects
    */
    std::vector< Descriptor > getDescriptors(void);

    /**
        Binds the vertex and index data for command buffer recording and executes the draw call

        @param      commandBuffers_     The command buffers to be recorded
        @param      imageIndex_         The swapchain image index
    */
    void draw(std::vector< VkCommandBuffer >& commandBuffers_, uint32_t imageIndex_);

    /**
        Default destructor
    */
    ~Mesh(void);

};

