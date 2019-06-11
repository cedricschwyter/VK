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

class BaseCamera
{
public:
    
    glm::vec3       camPos              = glm::vec3(2.0f, 2.0f, 2.0f);
    glm::vec3       tar                 = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3       camFront            = glm::normalize(tar - camPos); 
    glm::vec3       up                  = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3       camRight            = glm::normalize(glm::cross(up, camFront));
    glm::vec3       camUp               = glm::cross(camFront, camRight);

    double          yaw                 = 90.0;
    double          pitch               = 0.0;
    double          roll                = 0.0;

    double          lastX               = 0;
    double          lastY               = 0;
    bool            firstMouse          = true;

    /**
        Default constructor
    */
    BaseCamera(void);

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

