/**
    Implements the FPSCamera class

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         FPSCamera.cpp
    @brief        Implementation of the FPSCamera class
*/
#include "FPSCamera.hpp"


void FPSCamera::processKeyboardInput(GLFWwindow* window_) {

    static float deltaTime = 0.0f;
    static float lastFrame = 0.0f;

    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    float camSpeed = 2.5f * deltaTime;

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

    if (glfwGetKey(window_, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {

        inputEnabled = false;
        glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstMouse = true;

    }
    else if (glfwGetKey(window_, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE) {

        inputEnabled = true;
        glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    }

}

void FPSCamera::processMouseMovement(double xPos_, double yPos_) {

    static double lastX = 0.0;
    static double lastY = 0.0;

    if (inputEnabled) {

        double xOff;
        double yOff;

        if (firstMouse) {

            xOff = 0;
            yOff = 0;
            firstMouse = false;

        }
        else {

            xOff = xPos_ - lastX;
            yOff = yPos_ - lastY;

        }

        lastX = xPos_;
        lastY = yPos_;

        xOff *= sens;
        yOff *= sens;


        yaw += xOff;
        pitch += yOff;

        pitch = std::clamp(pitch, -89.0, 89.0);

        updateCameraVectors();

    }

}

void FPSCamera::processMouseScroll(double xOff_, double yOff_) {
   
    if (inputEnabled) {

        fov -= yOff_;

        fov = std::clamp(fov, 1.0, 105.0);

    }

}

FPSCamera::~FPSCamera() {



}
