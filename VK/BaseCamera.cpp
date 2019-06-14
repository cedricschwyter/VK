/**
    Implements the BaseCamera class

    @author        D3PSI
    @version       0.0.1 02.12.2019

    @file          BaseCamera.cpp
    @brief         Implementation of the BaseCamera class
*/
#include "BaseCamera.hpp"
#include "VK.hpp"

BaseCamera::BaseCamera(
    glm::vec3         pos_,
    glm::vec3         up_,
    float             yaw_,
    float             pitch_,
    float             roll_
    ) : camFront(glm::vec3(0.0f, 0.0f, -1.0f)), 
        camUp(glm::vec3(0.0f, 1.0f, 0.0f)),
        speed(vk::SPEED), 
        sens(vk::SENS), 
        fov(vk::FOV) {

    camPos      = pos_;
    worldUp     = up_;
    yaw         = yaw_;
    pitch       = pitch_;
    roll        = roll_;
    updateCameraVectors();

}

BaseCamera::~BaseCamera() {



}

void BaseCamera::proccessKeyboardInput(GLFWwindow* window_) {

    

}

void BaseCamera::processMouseMovement(double xPos_, double yPos_) {

    

}

void BaseCamera::processMouseScroll(double xOff_, double yOff_) {



}

void BaseCamera::updateCameraVectors() {

    glm::vec3 newFront;
    newFront.x = glm::cos(static_cast< float >(glm::radians(BaseCamera::yaw))) * glm::cos(static_cast< float >(glm::radians(BaseCamera::pitch)));
    newFront.y = glm::sin(static_cast< float >(glm::radians(BaseCamera::pitch)));
    newFront.z = glm::sin(static_cast< float >(glm::radians(BaseCamera::yaw))) * glm::cos(static_cast< float >(glm::radians(BaseCamera::pitch)));

    camFront = glm::normalize(newFront);
    camUp = worldUp;
    camRight = glm::normalize(glm::cross(camFront, camUp));

}

glm::mat4 BaseCamera::getViewMatrix() {

    glm::mat4 view; 
    view = glm::lookAt(camPos, camPos + camFront, camUp);

    return view;

}