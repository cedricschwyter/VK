/**
	Implements a fragment shader for all main calculations of a fragment's color

	@author		D3PSI
	@version	0.0.1 02.12.2019
	
	@file		shader.vert
	@brief		Implementation of a fragment shader for all main calculations of a fragment's color
*/
#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragColor;

layout(location = 0) out vec4 outColor;

void main() {

    outColor = vec4(fragColor, 1.0);

}