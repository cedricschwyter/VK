/**
    Defines the BaseCamera class

    @author		D3PSI
    @version	0.0.1 02.12.2019

    @file		BaseCamera.hpp
    @brief		Definition of the BaseCamera class
*/
#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <iostream>

namespace vk {

    // Starting/Default camera state values
    extern const double                         YAW;
    extern const double                         PITCH;
    extern const double                         ROLL;
    extern const double                         SPEED;
    extern const double                         SENS;
    extern const double                         FOV;

}

/**
    Defines camera movement states, not necessary because unused though
*/
enum CameraMovement {
    
    FWD,
    BCK,
    LFT,
    RGT

};

class BaseCamera
{
public:
    
    glm::vec3       camPos;
    glm::vec3       camFront; 
    glm::vec3       worldUp;
    glm::vec3       camRight;
    glm::vec3       camUp;

    double          yaw;
    double          pitch;
    double          roll;
    double          speed;
    double          sens;
    double          fov;


    bool            firstMouse          = true;

    /**
        Constructor with arguments

        @param      pos_        The cameras initial position
        @param      up_         The cameras initial up-vector
        @param      yaw_        The cameras initial yaw-angle
        @param      pitch_      The cameras initial pitch-angle
        @param      roll_       The cameras initial roll-angle
    */
    BaseCamera(
        glm::vec3         pos_          = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3         up_           = glm::vec3(0.0f, 1.0f, 0.0f),
        float             yaw_          = static_cast< float >(vk::YAW),
        float             pitch_        = static_cast< float >(vk::PITCH),
        float             roll_         = static_cast< float >(vk::ROLL)
        );

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
    virtual void processMouseMovement(double xPos_, double yPos_);

    /**
        Processes mouse scrolling events

        @param      xOff_       The x-offset of the mousewheel
        @param      yOff_       The y-offset of the mousewheel
    */
    virtual void processMouseScroll(double xOff_, double yOff_);

    /**
        Updates the camera orientation vectors
    */
    void updateCameraVectors(void);

    /**
        Calculates the view matrix using the current camera state

        @return     Returns a glm::mat4 by reference which is to be uploaded to the shaders as the view matrix
    */
    glm::mat4 getViewMatrix(void);

    /**
        Default destructor
    */
    ~BaseCamera(void);

};

