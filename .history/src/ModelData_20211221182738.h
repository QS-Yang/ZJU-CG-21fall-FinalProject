#pragma once

class ModelData {

public:
	float* vertices;
	float* textureCoords;
	float* normals;
	int* indices;
	float furthestPoint;

	ModelData(float vertices[], float textureCoords[], float normals[], int indices[],
			float furthestPoint) {
		this.vertices = vertices;
		this.textureCoords = textureCoords;
		this.normals = normals;
		this.indices = indices;
		this.furthestPoint = furthestPoint;
	}

}
