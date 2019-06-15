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
    ) : camUp(glm::vec3(0.0f, 1.0f, 0.0f)),
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

void BaseCamera::processKeyboardInput(GLFWwindow* window_) {

    

}

void BaseCamera::processMouseMovement(double xPos_, double yPos_) {

    

}

void BaseCamera::processMouseScroll(double xOff_, double yOff_) {



}

void BaseCamera::updateCameraVectors() {

    glm::vec3 newFront;
    newFront.x = static_cast< float >(glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)));
    newFront.y = static_cast< float >(glm::sin(glm::radians(pitch)));
    newFront.z = static_cast< float >(glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch)));

    camFront = glm::normalize(newFront);
    
    camRight = glm::normalize(glm::cross(camFront, worldUp));
    camUp = glm::normalize(glm::cross(camRight, camFront));
        
}

glm::mat4 BaseCamera::getViewMatrix() {

    glm::mat4 view; 
    view = glm::lookAt(camPos, camPos + camFront, camUp);

    return view;

}