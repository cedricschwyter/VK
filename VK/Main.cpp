/**
    Implements the main entry point for the VKEngine

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         Main.cpp
    @brief        Implementation of the Vulkan-part for my Vulkan vs. OpenGL comparison (Maturaarbeit)
*/
#include "VK.hpp"

/**
    Returns the model matrix for the rose model  
  
    @return     Returns a glm::mat4
*/
glm::mat4 rose() {

    static auto                     start           = std::chrono::high_resolution_clock::now();
    auto                            current         = std::chrono::high_resolution_clock::now();
                                
    float                           delta           = std::chrono::duration< float, std::chrono::seconds::period >(current - start).count();        // Namespaces are a fricking mess in <chrono>
            
    glm::mat4 model;
    model           = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.05f, 0.0f));
    model           = glm::scale(model, glm::vec3(0.005f));
    model           = glm::rotate(model, delta * glm::radians(90.0f), glm::vec3(0.0f, 0.05f, 0.0f));
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
    model           = glm::scale(model, glm::vec3(0.02f));
    model[1][1]     *= -1.0f;

    return model;

}

/**
    Entry point for the application
*/
int main() {

    vk::init();

    vk::push("res/models/tennisball/Tennis Ball-1.obj", &rose);
    vk::push("res/models/stick/lathi.obj", &city);

    return vk::run();

}