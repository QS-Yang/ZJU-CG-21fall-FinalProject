#pragma once
#include <glm/glm.hpp>
class Vertex {
public:
    glm::vec3 position;
    int textureIndex = -1;
    int normalIndex = -1;
    Vertex* duplicateVertex;
    int index;
    float length;

    Vertex(int index, glm::vec3 position) {
        this->index = index;
        this->position = position;
        this->length = position.length();
    }

    bool isSet() {
        return textureIndex != -1 && normalIndex != -1;
    }
    bool hasSameTextureAndNormal(int textureIndexOther, int normalIndexOther) {
        return textureIndexOther == textureIndex && normalIndexOther == normalIndex;
    }
    
};