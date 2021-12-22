#pragma once
#include <glm/glm.hpp>
class Vertex {
public:
    glm::vec3 position;
    int textureIndex = -1;
    int normalIndex = -1;
    Vertex *duplicateVertex=NULL;
    int index;
    float length;

    Vertex(){}

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
    void setTextureIndex(int textureIndex){
		this->textureIndex = textureIndex;
	}
	
	void setNormalIndex(int normalIndex){
		this->normalIndex = normalIndex;
	}
    void setDuplicateVertex(Vertex *duplicateVertex) {
		this->duplicateVertex = duplicateVertex;
	}
    
};