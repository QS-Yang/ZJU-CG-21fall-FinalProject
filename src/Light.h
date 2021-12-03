#pragma once
#include "Entity.h"

using namespace glm;

class Light{
public:
    vec3 pos;
    vec3 color;
    Light(vec3 pos, vec3 color){
        this->pos = pos;
        this->color = color;
    }
};