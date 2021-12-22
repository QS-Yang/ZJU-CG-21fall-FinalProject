#pragma once
#include "ModelData.h"
#include "Vertex.h"
#include <glm/glm.hpp>

class ObjLoader {
public:
    ObjLoader();
    ~ObjLoader();
    ModelData* loadObj(const char* fileName);
    void freeModelData(ModelData* modelData);
};