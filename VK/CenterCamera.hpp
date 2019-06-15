/**
    Defines the CenterCamera class

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         CenterCamera.hpp
    @brief        Definition of the CenterCamera class
*/
#pragma once
#include "BaseCamera.hpp"
#include "Makros.hpp"

class CenterCamera :
    public BaseCamera
{
public:

    /**
        Constructor with default arguments

        @param      target_         Where the camera is facing
        @param      distance        The initial radius to place the camera in
    */
    CenterCamera(glm::vec3 target_ = ORIGIN, float distance_ = 5.0f);
    
    /**
        Check for keyboard input related to the camera object

        @param      window_     A pointer to the GLFWwindow in focus
    */
    void processKeyboardInput(GLFWwindow* window_);

    /**
        Processes mouse scrolling events

        @param      xOff_       The x-offset of the mousewheel
        @param      yOff_       The y-offset of the mousewheel
    */
    void processMouseScroll(double xOff_, double yOff_);

    /**
        Recalculates the camera's vectors
    */
    void updateCameraVectors(void);

    /**
        Default destructor
    */
    ~CenterCamera();

private:

    float radius = 0;
    glm::vec3 camTar;

};

