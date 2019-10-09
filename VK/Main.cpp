/**
    Implements the main entry point for the VKEngine

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         Main.cpp
    @brief        Implementation of the Vulkan-part for my Vulkan vs. OpenGL comparison (Maturaarbeit)
*/
#include "VK.hpp"

namespace dp {

    const float         pi              = 3.1415926535897932384626f;        // 23 digits in decimal (rounded), which will equal ~32 digits in binary
    const float         g               = 0.9806000000000000000000f;        // which is the maximum floating point precision I want to use here 

    float               p1_length       = 10.0f;
    float               p2_length       = 10.0f;
    float               p1_theta        = pi / 4.0f;
    float               p2_theta        = pi / 4.0f;
    float               p1_vel          = 0.1f;
    float               p2_vel          = 0.1f;
    float               p1_acc          = 0.0f;
    float               p2_acc          = 0.0f;
    glm::vec3           p1_origin       = ORIGIN;
    glm::vec3           p1_pos          = glm::vec3(p1_length * glm::cos(glm::radians(p1_theta)), p1_length * glm::sin(glm::radians(p1_theta)), 0.0f);
    glm::vec3           p2_origin       = p1_pos;
    glm::vec3           p2_pos          = glm::vec3(p2_length * glm::cos(glm::radians(p2_theta)), p2_length * glm::sin(glm::radians(p2_theta)), 0.0f);
    float               p1_mass         = 0.002f;
    float               p2_mass         = 0.002f;
    float               delta_p1_vel    = -0.000001f;
    float               delta_p2_vel    = -0.000001f;
    std::mutex          p1_pos_mutex;
    std::mutex          p2_pos_mutex;

    /**
        Returns the nominal acceleration of p1

        @return     Returns a float representing the nominal acceleration of p1
    */
    float getAccP1() {

        float temp = ((-g * (2 * p1_mass + p2_mass) * glm::sin(glm::radians(p1_theta))) -
            (p2_mass * g * glm::sin(glm::radians(p1_theta - 2 * p2_theta))) - (2 * glm::sin(glm::radians(p1_theta - p2_theta)) * p2_mass *
            (p2_vel * p2_vel * p2_length + p1_vel * p1_vel * p1_length * glm::cos(glm::radians(p1_theta - p2_theta))))) /
            (p1_length * (2 * p1_mass + p2_mass - p2_mass * glm::cos(glm::radians(2 * p1_theta - 2 * p2_theta))));

        return temp;

    }

    /**
        Returns the nominal acceleration of p2

        @return     Returns a float representing the nominal acceleration of p2
    */
    float getAccP2() {

        float temp = ((2 * glm::sin(glm::radians(p1_theta - p2_theta)) * (p1_vel * p1_vel * p1_length *
            (p1_mass + p2_mass) + g * (p1_mass + p2_mass) * glm::cos(glm::radians(p1_theta)) +
            (p2_vel * p2_vel * p2_length * p2_mass * glm::cos(glm::radians(p1_theta - p2_theta)))))) /
            (p2_length * (2 * p1_mass + p2_mass - p2_mass * glm::cos(glm::radians(2 * p1_theta - 2 * p2_theta))));

        return temp;

    }

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

        std::scoped_lock< std::mutex > lock(p1_pos_mutex);
        glm::mat4 model;
        model = glm::translate(glm::mat4(1.0f), p1_pos);
        model = glm::scale(model, glm::vec3(p1_mass / 10.0f));
        std::cout << p1_pos.x << " " << p1_pos.y << " " << p1_pos.z << std::endl;

        return model;

    }

    /**
        Returns the model matrix for the second ball model

        @return     Returns a glm::mat4
    */
    glm::mat4 ball2() {

        std::scoped_lock< std::mutex > lock(p2_pos_mutex);
        glm::mat4 model;
        model = glm::translate(glm::mat4(1.0f), p2_pos / 10.0f);
        model = glm::scale(model, glm::vec3(p2_mass));

        return model;

    }

    /**
        Initializes the application

        @return     Returns the main status code
    */
    VK_STATUS_CODE init() {

        vk::init();

        vk::push("res/models/tennisball/Tennis Ball-1.obj", &ball1);
        vk::push("res/models/tennisball/Tennis Ball-1.obj", &ball2);
        //vk::push("res/models/stick/lathi.obj", ST_STANDARD, &stick1);
        //vk::push("res/models/stick/lathi.obj", ST_STANDARD, &stick2);

        return vk::run();

    }

    /**
        Computes the overall state of the pendulum
    */
    void computePendulumState() {

        double          now       = glfwGetTime();
        static double   last      = 0.0;

        if (now - last >= 1 / 60.0) {

            std::scoped_lock< std::mutex > p1lock(p1_pos_mutex);
            std::scoped_lock< std::mutex > p2lock(p2_pos_mutex);
            p1_acc = getAccP1();
            p2_acc = getAccP2();

            p1_origin       = ORIGIN;
            p1_pos          = glm::vec3(p1_length / 10.0f * glm::cos(glm::radians(p1_theta)), p1_length / 10.0f * glm::sin(glm::radians(p1_theta)), 0.0f);
            p2_origin       = p1_pos;
            p2_pos          = glm::vec3(p2_length / 10.0f * glm::cos(glm::radians(p2_theta)), p2_length / 10.0f * glm::sin(glm::radians(p2_theta)), 0.0f);

            p1_vel          += p1_acc;
            p2_vel          += p2_acc;
            p1_theta        += p1_vel;
            p2_theta        += p2_vel;
            p1_vel          += delta_p1_vel;
            p2_vel          += delta_p2_vel;
            last            = now;

        }

    }

}

/**
    Entry point for the application
*/
int main() {

    std::thread pendulumThread([=]() {

        while (!vk::core::initialized) std::this_thread::sleep_for(std::chrono::duration(std::chrono::milliseconds(1)));

        while (!glfwWindowShouldClose(vk::core::window)) {

            dp::computePendulumState();

        }

    });
    pendulumThread.detach();

    return dp::init();

}