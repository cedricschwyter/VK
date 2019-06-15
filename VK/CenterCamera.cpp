/**
    Implents the CenterCamera class

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         CenterCamera.cpp
    @brief        Implementation of the CenterCamera class
*/
#include "CenterCamera.hpp"


CenterCamera::CenterCamera(glm::vec3 target_, float radius_) {

    camPos = glm::vec3(radius_, 0.0f, 0.0f);
    radius = radius_;
    camTar = target_;
    updateCameraVectors();

}

void CenterCamera::processKeyboardInput(GLFWwindow* window_) {

    if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS) {
    
        pitch -= 1.0 * sens;
    
    }

    if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS) {

        pitch += 1.0 * sens;

    }

    if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS) {

        yaw += 1.0 * sens;

    }

    if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS) {

        yaw -= 1.0 * sens;

    }

    pitch = std::clamp(pitch, -89.0, 89.0);

    updateCameraVectors();

}

void CenterCamera::processMouseScroll(double xOff_, double yOff_) {

    if (inputEnabled) {

        fov -= yOff_;

        fov = std::clamp(fov, 1.0, 105.0);

    }

}

void CenterCamera::updateCameraVectors() {

    glm::vec3 tempPos;
    tempPos.x = static_cast< float >(glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)));
    tempPos.y = static_cast< float >(glm::sin(glm::radians(pitch)));
    tempPos.z = static_cast< float >(glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch)));

    camPos = radius * glm::normalize(tempPos);
    camFront = glm::normalize(camTar - camPos);

    camRight = glm::normalize(glm::cross(camFront, worldUp));
    camUp = glm::normalize(glm::cross(camRight, camFront));

}

CenterCamera::~CenterCamera() {



}
