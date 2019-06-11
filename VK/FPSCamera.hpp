/**
    Defines the FPSCamera class

    @author		D3PSI
    @version	0.0.1 02.12.2019

    @file		FPSCamera.hpp
    @brief		Definition of the FPSCamera class
*/
#pragma once
#include "BaseCamera.hpp"

class FPSCamera :
    public BaseCamera
{
public:

    /**
        Default constructor
    */
    FPSCamera(void);

    /**
        Check for mouse and keyboard input related to the camera object

        @param      window_     A pointer to the GLFWwindow in focus
    */
    void checkInput(GLFWwindow* window_);

    /**
        Calculates the new orientation for the camera

        @param      xPos_       The x-position of the cursor
        @param      yPos_       The y-position of the cursor
    */
    void processMouseMovement(double xPos_, double yPos_);

    /**
        Updates the camera orientation vectors
    */
    void updateCameraVectors(void);

    /**
        Default constructor
    */
    ~FPSCamera(void);

};

