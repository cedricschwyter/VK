/**
    Defines the CenterCamera class

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         CenterCamera.hpp
    @brief        Definition of the CenterCamera class
*/
#pragma once
#include "BaseCamera.hpp"

class CenterCamera :
    public BaseCamera
{
public:

    CenterCamera(glm::vec3 target_ = glm::vec3(0.0f, 0.0f, 0.0f), float distance_ = 5.0f);
    
    /**
        Check for keyboard input related to the camera object

        @param      window_     A pointer to the GLFWwindow in focus
    */
    void proccessKeyboardInput(GLFWwindow* window_);
    
    ~CenterCamera();
};

