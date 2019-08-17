/**
    Implements a vertex shader for all main calculations of a vertex' color

    @author       D3PSI
    @version      0.0.1 02.12.2019
    
    @file         shader.vert
    @brief        Implementation of a vertex shader for all main calculations of a vertex' color
*/
#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 nor;
layout(location = 2) in vec2 tex;
layout(location = 3) in vec3 tan;
layout(location = 4) in vec3 bit;

layout(binding = 0) uniform MVPBuffer {

    mat4 model;
    mat4 view;
    mat4 proj;

} mvp;

layout(location = 0) out vec3 outPos;
layout(location = 1) out vec2 outTex;
layout(location = 2) out vec3 outNor;

void main() {

    gl_Position         = mvp.proj * mvp.view * mvp.model * vec4(pos, 1.0);
    outTex              = tex;
    outPos              = vec3(mvp.model * vec4(pos, 1.0));
    outNor              = mat3(transpose(inverse(mvp.model))) * nor;

}