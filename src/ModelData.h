#pragma once

class ModelData {

public:
	float* vertices;
	float* textureCoords;
	float* normals;
	int* indices;
	float furthestPoint;
	int numOfVertices;
	int numOfIndices;

	ModelData(float vertices[], float textureCoords[], float normals[], int indices[],
			float furthestPoint, int numOfVertices, int numOfIndices) {
		this->vertices = vertices;
		this->textureCoords = textureCoords;
		this->normals = normals;
		this->indices = indices;
		this->furthestPoint = furthestPoint;
		this->numOfIndices = numOfIndices;
		this->numOfVertices = numOfVertices;
	}
};
