/**
    Defines the FPSCamera class, inheriting BaseCamera

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         FPSCamera.hpp
    @brief        Definition of the FPSCamera class
*/
#pragma once
#include "BaseCamera.hpp"

class FPSCamera :
    public BaseCamera
{
    using BaseCamera::BaseCamera;
public:

    /**
        Handles keyboard input

        @param      window_     A pointer to a GLFW window
    */
    void processKeyboardInput(GLFWwindow* window_);

    /**
        Calculates the new orientation for the camera

        @param      xPos_       The x-position of the cursor
        @param      yPos_       The y-position of the cursor
    */
    void processMouseMovement(double xPos_, double yPos_);

    /**
        Processes mouse scrolling events

        @param      xOff_       The x-offset of the mousewheel
        @param      yOff_       The y-offset of the mousewheel
    */
    void processMouseScroll(double xOff_, double yOff_);

    /**
        Default constructor
    */
    ~FPSCamera(void);

};

