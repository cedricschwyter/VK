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

class BaseCamera
{
public:
    
    glm::vec3 camPos        = glm::vec3(2.0f, 2.0f, 2.0f);
    glm::vec3 tar           = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 camFront      = -(glm::normalize(camPos - tar)); // inverse as this vector would point out the back of the camera
    glm::vec3 up            = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 camRight      = glm::normalize(glm::cross(up, camFront));
    glm::vec3 camUp         = glm::cross(camFront, camRight);

    /**
        Default constructor
    */
    BaseCamera(void);

    /**
        Calculates the view matrix using the current camera state

        @return     Returns a glm::mat4 by reference which is to be uploaded to the shaders as the view matrix
    */
    glm::mat4& getViewMatrix(void);

    /**
        Default destructor
    */
    ~BaseCamera(void);

};

