#pragma once
#include "Entity.h"

using namespace glm;

class Light{
public:
    vec3 pos;
    vec3 color;
    vec3 attenuation;
    vec3  direction;
    float cutOff;
    Light(vec3 pos, vec3 color, vec3 attenuation, vec3 direction, float cutoff){
        this->pos = pos;
        this->color = color;
        this->attenuation = attenuation;
        this->direction = direction;
        this->cutOff = cutoff;
    }
};