#pragma once
#include "Entity.h"

using namespace glm;

class Light{
public:
    vec3 pos;
    vec3 color;
    vec3 attenuation;
    Light(vec3 pos, vec3 color, vec3 attenuation){
        this->pos = pos;
        this->color = color;
        this->attenuation = attenuation;
    }
};