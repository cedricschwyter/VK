/**
    Implements a fragment shader for all main calculations of a fragment's color

    @author       D3PSI
    @version      0.0.1 02.12.2019
    
    @file         shader.vert
    @brief        Implementation of a fragment shader for all main calculations of a fragment's color
*/
#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 outPos;
layout(location = 1) in vec2 outTex;
layout(location = 2) in vec3 outNor;

layout(binding = 1) uniform sampler2D diffSampler;

layout(binding = 2) uniform LightData {

    vec3 lightPos;
    vec3 viewPos;
    vec3 lightCol;

} ld;

layout(location = 0) out vec4 outColor;

void main() {

    vec3 objectColor            = texture(diffSampler, outTex).xyz;

    // ambient lighting
    float ambientStrength       = 0.1;
    vec3 ambient                = ambientStrength * ld.lightCol;
  	
    // diffuse lighting
    vec3 norm                   = normalize(outNor);
    vec3 lightDir               = normalize(ld.lightPos - outPos);
    float diff                  = max(dot(norm, lightDir), 0.0);
    vec3 diffuse                = diff * ld.lightCol;
    
    // specular lighting
    float specularStrength      = 0.5;
    vec3 viewDir                = normalize(ld.viewPos - outPos);
    vec3 reflectDir             = reflect(-lightDir, norm);  
    float spec                  = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular               = specularStrength * spec * ld.lightCol;  
        
    vec3 result                 = (ambient + diffuse + specular) * objectColor;
    outColor                    = vec4(result, 1.0);

}