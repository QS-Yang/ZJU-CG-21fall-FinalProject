#pragma once
#include "Entity.h"
#include "Terrain.h"
#include "Matrix.h"
#include "Light.h"
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
    float xSize1 = 8;
    float xSize2 = 6;
    float zSize = 1;
    float collideSize = 3;

    float Xslope=0;
    float Zslope=0;

    bool space_state = false;

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

        if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
            if(space_state == false){
                if(light->cutOff>-10) light->cutOff = -1000;
                else light->cutOff = -0.9;
                space_state = true;
            }
        } else space_state = false;

    }

public:

    GLFWwindow* window;
    Light *light;
    using Entity::Entity;

    void setLight(Light* light){
        this->light = light;
    }

    void addWindow(GLFWwindow* window){
        this->window = window;
    }

    void setCollideObject(vector<glm::vec3> objectPos){
        this->objectPos = objectPos;
    }

    bool checkCollision(float x1, float z1, float x2, float z2){
        return sqrt((x1-x2)*(x1-x2)+(z1-z2)*(z1-z2))<collideSize;
    }

    void move(float delta, Terrain terrain) {
        checkInputs();
        Rot(0, currentTurnSpeed * delta, 0);
        float distance = currentSpeed * delta;
        float dx = distance * sin(radians(this->ry));
        float dz = distance * cos(radians(this->ry));
        bool collision=false;
        for(int i=0; i<objectPos.size(); i++){
            if(checkCollision(position.x+dx, position.z+dz, objectPos[i].x, objectPos[i].z)){
                collision=true;
                break;
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

        Xslope =(terrainHeight-terrain.getHeightOfTerrain(position.x-sin(radians(this->ry))*10, position.z-cos(radians(this->ry))*10))/
                (sqrt(sin(radians(this->ry))*sin(radians(this->ry))*100+cos(radians(this->ry))*cos(radians(this->ry))*100));
        Zslope =(terrainHeight-terrain.getHeightOfTerrain(position.x-sin(radians(this->ry+270))*5, position.z-cos(radians(this->ry+270))*5))/
                (sqrt(sin(radians(this->ry+270))*sin(radians(this->ry+270))*25+cos(radians(this->ry+270))*cos(radians(this->ry+270))*25));

        float Xangle = atan(Xslope)*360/(2*Pi);
        float Zangle = atan(Zslope)*360/(2*Pi);

        //cout<<angle<<endl;
        rx = -Xangle*cos(ry*2*Pi/360.0);
        rz = -Zangle*fabs(sin(ry*2*Pi/360.0));

        light->pos.x = position.x + 4*sin(radians(this->ry));
        light->pos.z = position.z + 4*cos(radians(this->ry));
        light->pos.y = position.y - 4*sin(radians(this->rx)) + 0.5;
        light->direction = vec3(sin(radians(this->ry)), -sin(radians(this->rx)),cos(radians(this->ry)));
    }


};