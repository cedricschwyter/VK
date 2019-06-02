/**
    Implements the FPSCamera class

    @author		D3PSI
    @version	0.0.1 02.12.2019

    @file		FPSCamera.cpp
    @brief		Implementation of the FPSCamera class
*/
#include "FPSCamera.hpp"


FPSCamera::FPSCamera() {



}

void FPSCamera::checkInput(GLFWwindow* window_) {

    static float deltaTime      = 0.0f;
    static float lastFrame      = 0.0f;

    float currentFrame          = static_cast< float >(glfwGetTime());
    deltaTime                   = currentFrame - lastFrame;
    lastFrame                   = currentFrame;

    float camSpeed              = 2.5f * deltaTime;

    if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS) {

        camPos += camSpeed * camFront;

    }

    if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS) {

        camPos -= camSpeed * camFront;

    }

    if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS) {

        camPos -= glm::normalize(glm::cross(camFront, camUp)) * camSpeed;

    }

    if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS) {

        camPos += glm::normalize(glm::cross(camFront, camUp)) * camSpeed;

    }

}

void FPSCamera::calculateNewFrontVector(double xPos_, double yPos_) {

    static double lastX = 0;
    static double lastY = 0;

    double xOff = xPos_ - lastX;
    double yOff = yPos_ - lastY;  
    lastX = xPos_;
    lastY = yPos_;

}

FPSCamera::~FPSCamera() {



}
