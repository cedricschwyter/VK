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

    float camSpeed       = 0.001f;

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
        Default constructor
    */
    ~FPSCamera(void);

};

