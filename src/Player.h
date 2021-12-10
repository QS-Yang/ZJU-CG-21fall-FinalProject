#pragma once
#include "Entity.h"
#include "Terrain.h"
#include "Matrix.h"

class Player: public Entity 
{
private:
    float RUN_SPEED = 20;
    float TURN_SPEED = 160;
    float GRAVITY = 30;
    float JUMP_POWER = 10;

    float TERRAIN_HEIGHT = 0;

    float currentSpeed = 0;
    float currentTurnSpeed = 0;
    float upwardsSpeed = 0;

    bool isInAir = false;

    // void jump() {
    //     this->upwardsSpeed = JUMP_POWER;
    // }

    void checkInputs(){
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
            this->currentSpeed = RUN_SPEED;
        }else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
            this->currentSpeed = -RUN_SPEED;
        }else{
            this->currentSpeed = 0;
        }

        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
            this->currentTurnSpeed = TURN_SPEED;
        }else if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
            this->currentTurnSpeed = -TURN_SPEED;
        }else{
            this->currentTurnSpeed = 0;
        }

        // if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        //     jump();
        // }

    }

public:
    // Player(TexturedModel model, vec3 position, float rx, float ry, float rz, float scale):Entity(model, position, rx, ry, rz, scale){

    // }
    GLFWwindow* window;
    using Entity::Entity;

    void addWindow(GLFWwindow* window){
        this->window = window;
    }

    void move(float delta, Terrain terrain) {
        checkInputs();
        Rot(0, currentTurnSpeed * delta, 0);
        float distance = currentSpeed * delta;
        float dx = distance * sin(radians(this->ry));
        float dz = distance * cos(radians(this->ry));
        Up(dx, 0, dz); 
        //std::cout << this->ry << std::endl;   
        if(this->upwardsSpeed > -JUMP_POWER && this->upwardsSpeed < 0){
            this->upwardsSpeed -= GRAVITY * delta;
        }else if(this->upwardsSpeed > 0){
            this->upwardsSpeed -= GRAVITY * delta;
        }else{
            this->upwardsSpeed = 0;
        }
        Up(0, this->upwardsSpeed * delta, 0);
        float terrainHeight = terrain.getHeightOfTerrain(position.x, position.z); 
        this->position.y = terrainHeight;
        // if(this->position.y < terrainHeight){
        //     this->upwardsSpeed = 0;
        //     this->isInAir = false;
        //     this->position.y = terrainHeight;
        // }
    }


};