/**
    Implements the main entry point for the VKEngine

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         Main.cpp
    @brief        Implementation of the Vulkan-part for my Vulkan vs. OpenGL comparison (Maturaarbeit)
*/
#include "VK.hpp"

/**
    Entry point for the application
*/
int main() {

    vk::init();

    vk::push("res/models/nanosuit/nanosuit.obj");
    vk::push("res/models/nanosuit/nanosuit.obj");
    vk::push("res/models/nanosuit/nanosuit.obj");
    vk::push("res/models/nanosuit/nanosuit.obj");
    vk::push("res/models/nanosuit/nanosuit.obj");
    vk::push("res/models/nanosuit/nanosuit.obj");
    vk::push("res/models/nanosuit/nanosuit.obj");
    vk::push("res/models/nanosuit/nanosuit.obj");
    vk::push("res/models/nanosuit/nanosuit.obj");

    return vk::run();

}