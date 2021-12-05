#pragma once
#include "Entity.h"
#define Step 0.2
using namespace glm;

class Camera{
public:
    vec3 position = vec3(0,5,0);
    float pitch = 10;
    float yaw;
    float roll;
    GLFWwindow* window;

    Camera(GLFWwindow* window){
        this->window = window;
    }

    void move(){
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
            position.z-=Step;
        }
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
            position.z+=Step;
        }
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
            position.x-=Step;
        }
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
            position.x+=Step;
        }
    }
};