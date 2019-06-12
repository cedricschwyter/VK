/**
    Implements the FPSCamera class

    @author		D3PSI
    @version	0.0.1 02.12.2019

    @file		FPSCamera.cpp
    @brief		Implementation of the FPSCamera class
*/
#include "FPSCamera.hpp"


void FPSCamera::processMouseMovement(double xPos_, double yPos_) {

    static double lastX = 0.0;
    static double lastY = 0.0;

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


        BaseCamera::yaw += xOff;
        BaseCamera::pitch += yOff;

        BaseCamera::pitch = std::clamp(BaseCamera::pitch, -89.0, 89.0);
        std::cout << BaseCamera::pitch << std::endl;


    updateCameraVectors();

}

void FPSCamera::processMouseScroll(double xOff_, double yOff_) {
   
    if (inputEnabled) {

        fov -= yOff_;

    }

    fov = std::clamp(fov, 1.0, 105.0);

}

FPSCamera::~FPSCamera() {



}
