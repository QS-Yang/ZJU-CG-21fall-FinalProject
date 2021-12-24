#pragma once
#include "Entity.h"
#include "Terrain.h"
#include "Matrix.h"
#include <vector>

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

    vector<glm::vec3> objectPos;
    float collideSize = 2.5;

    float slope=0;

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

    void setCollideObject(vector<glm::vec3> objectPos){
        this->objectPos = objectPos;
    }

    float dis(float x1, float z1, float x2, float z2){
        return sqrt((x1-x2)*(x1-x2)+(z1-z2)*(z1-z2));
    }

    void move(float delta, Terrain terrain) {
        checkInputs();
        Rot(0, currentTurnSpeed * delta, 0);
        float distance = currentSpeed * delta;
        float dx = distance * sin(radians(this->ry));
        float dz = distance * cos(radians(this->ry));
        bool collision=false;
        for(int i=0; i<objectPos.size(); i++){
            if(dis(position.x+dx, position.z+dz, objectPos[i].x, objectPos[i].z)<collideSize){
                collision=true;
            }
        }
        if(!collision) Up(dx, 0, dz); 
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

        if(currentSpeed) {
            slope =(terrainHeight-terrain.getHeightOfTerrain(position.x-dx*10, position.z-dz*10))/(sqrt(dx*dx*100+dz*dz*100));
            //cout<<dx<<" "<<dz<<endl;
            if(currentSpeed<0) slope = -slope;
            //cout<<slope<<endl;
        }
        float angle = atan(slope)*360/(2*Pi);
        //cout<<angle<<endl;
        rx = -angle*cos(ry*2*Pi/360.0);
        rz = angle*sin(ry*2*Pi/360.0)/5.0;
        // rz = angle;
        
        // if(this->position.y < terrainHeight){
        //     this->upwardsSpeed = 0;
        //     this->isInAir = false;
        //     this->position.y = terrainHeight;
        // }
    }


};