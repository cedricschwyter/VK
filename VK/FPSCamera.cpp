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

void FPSCamera::processMouseMovement(double xPos_, double yPos_) {

    double xOff;
    double yOff;

    if (firstMouse) {

        xOff = 0;
        yOff = 0;
        firstMouse = false;

    }
    else {
    
        xOff = xPos_ - lastX;
        yOff = -(yPos_ - lastY);
    
    }
 
    lastX = xPos_;
    lastY = yPos_;

    float sensitivity = 0.05f;
    xOff *= sensitivity;
    yOff *= sensitivity;

    yaw += xOff;
    pitch += yOff;

    pitch = std::clamp(pitch, -89.0, 89.0);

    updateCameraVectors();

}

void FPSCamera::updateCameraVectors() {

    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    camFront = glm::normalize(newFront);
    camRight = glm::normalize(glm::cross(camFront, camUp));
    camUp = glm::normalize(glm::cross(camRight, camFront));

}

FPSCamera::~FPSCamera() {



}
