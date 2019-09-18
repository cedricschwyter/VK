/**
    Implements the main entry point for the VKEngine

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         Main.cpp
    @brief        Implementation of the Vulkan-part for my Vulkan vs. OpenGL comparison (Maturaarbeit)
*/
#include "VK.hpp"

namespace dp {

    const double        PI                      = 3.1415;

    uint32_t            p1_length               = 1;
    uint32_t            p2_length               = 1;
    float               p1_theta                = PI / 4;
    float               p2_theta                = PI / 4;
    float               p1_vel                  = 0.1;
    float               p2_vel                  = 0.1;
    float               p1_acc                  = 0;
    float               p2_acc                  = 0;
    glm::vec3           p1_origin               = ORIGIN;
    glm::vec3           p1_pos                  = glm::vec3(p1_length * glm::cos(p1_theta), 0, p1_length * glm::sin(p1_theta));
    glm::vec3           p2_origin               = p1_pos;
    glm::vec3           p2_pos                  = glm::vec3(p2_length * glm::cos(p2_theta), 0, p2_length * glm::sin(p2_theta));
    uint32_t            p1_mass                 = 5;
    uint32_t            p2_mass                 = 5;
    float               delta_p1_vel            = -0.000001;
    float               delta_p2_vel            = -0.000001;

    /**
        Returns the model matrix for the first stick model

        @return     Returns a glm::mat4
    */
    glm::mat4 stick1() {

        glm::mat4 model;

        return model;

    }

    /**
        Returns the model matrix for the second stick model

        @return     Returns a glm::mat4
    */
    glm::mat4 stick2() {

        glm::mat4 model;

        return model;

    }

    /**
        Returns the model matrix for the first ball model

        @return     Returns a glm::mat4
    */
    glm::mat4 ball1() {

        glm::mat4 model;

        return model;

    }

    /**
        Returns the model matrix for the second ball model

        @return     Returns a glm::mat4
    */
    glm::mat4 ball2() {

        glm::mat4 model;

        return model;

    }

}

/**
    Entry point for the application
*/
int main() {

    vk::init();

    vk::push("res/models/tennisball/Tennis Ball-1.obj", &dp::ball1);
    vk::push("res/models/tennisball/Tennis Ball-1.obj", &dp::ball2);
    vk::push("res/models/stick/lathi.obj", &dp::stick1);
    vk::push("res/models/stick/lathi.obj", &dp::stick2);

    return vk::run();

}