/**
    Implements the main entry point for the VKEngine

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         Main.cpp
    @brief        Implementation of the Vulkan-part for my Vulkan vs. OpenGL comparison (Maturaarbeit)
*/
#include "VK.hpp"

/**
    Returns the model matrix for the nanosuit model  
  
    @return     Returns a glm::mat4
*/
glm::mat4 nanosuit() {

    glm::mat4 model;
    model           = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model           = glm::scale(model, glm::vec3(0.02f));
    model[1][1]     *= -1.0f;
    
    return model;

}

/**
    Returns the model matrix for the rose model  
  
    @return     Returns a glm::mat4
*/
glm::mat4 rose() {

    glm::mat4 model;
    model           = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.0f));
    model           = glm::scale(model, glm::vec3(0.02f));
    model[1][1]     *= -1.0f;

    return model;

}

/**
    Returns the model matrix for the floating_city model  
  
    @return     Returns a glm::mat4
*/
glm::mat4 city() {

    glm::mat4 model;
    model           = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    model[1][1]     *= -1.0f;

    return model;

}

/**
    Entry point for the application
*/
int main() {

    vk::init();

    vk::push("res/models/nanosuit/nanosuit.obj", &nanosuit);
    vk::push("res/models/rose/rose.obj", &rose);
    vk::push("res/models/cottage/cottage.obj", &city);

    return vk::run();

}