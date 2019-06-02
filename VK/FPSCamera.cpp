/**
    Implements the FPSCamera class

    @author		D3PSI
    @version	0.0.1 02.12.2019

    @file		FPSCamera.cpp
    @brief		Implementation of the FPSCamera class
*/
#include "FPSCamera.hpp"


FPSCamera::FPSCamera() : BaseCamera() {



}

void FPSCamera::checkInput(GLFWwindow* window_) {

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

FPSCamera::~FPSCamera() {



}
