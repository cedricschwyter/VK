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

class Mesh
{
public:

    void*        vertices;
    void*        indices;
    void*        textures;

    /**
        Constructor

        @param      vertices_       Pointer to vertex data of mesh
        @param      indices_        Pointer to index data of mesh
        @param      textures_       Pointer to texturing data of mesh
    */
    Mesh(void* vertices_, void* indices_, void* textures_);

    /**
        Binds the vertex and index data for command buffer recording
    */
    void bind(void);

private:

    BaseBuffer* vertexBuffer;
    BaseBuffer* indexBuffer;

};

