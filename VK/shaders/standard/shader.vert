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

layout(binding = 0) uniform VPBuffer {

    mat4 view;
    mat4 proj;

} vp;

layout(binding = 1) buffer MBuffer {

    mat4 model[];

} m;

layout( push_constant ) uniform modelMatrixIndex {

    int index;

} index;

layout(location = 0) out vec3 outPos;
layout(location = 1) out vec2 outTex;
layout(location = 2) out vec3 outNor;

void main() {

    gl_Position         = vp.proj * vp.view * m.model[index.index] * vec4(pos, 1.0);
    outTex              = tex;
    outPos              = vec3(m.model[index.index] * vec4(pos, 1.0));
    outNor              = mat3(transpose(inverse(m.model[index.index]))) * nor;

}