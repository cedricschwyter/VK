/**
    Implements the BaseCamera class

    @author		D3PSI
    @version	0.0.1 02.12.2019

    @file		BaseCamera.cpp
    @brief		Implementation of the BaseCamera class
*/
#include "BaseCamera.hpp"


BaseCamera::BaseCamera() {



}

BaseCamera::~BaseCamera() {



}

glm::mat4 BaseCamera::getViewMatrix() {

    glm::mat4 view; 
    view = glm::lookAt(camPos, camPos + camFront, camUp);

    return view;

}