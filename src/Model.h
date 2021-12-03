#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
class Model
{
public:
	int Vid;
	int VertexCount;
	
	Model () {
		this->Vid = 0;
		this->VertexCount = 0;
	}
	Model(int Vid, int VertexCount) {
		this->Vid = Vid;
		this->VertexCount = VertexCount;
	}
};

