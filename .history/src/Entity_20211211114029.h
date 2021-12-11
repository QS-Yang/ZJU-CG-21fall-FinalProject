#pragma once
#include "TexturedModel.h"

using namespace glm;

class Entity {
public:
	TexturedModel model;
	vec3 position;
	float rx, ry, rz;
	float scale;

	int textureIndex = 0;

	Entity(TexturedModel model, int index, vec3 position, float rx, float ry, float rz, float scale) {
		this->textureIndex = index;
		this->model = model;
		this->position = position;
		this->rx = rx;
		this->ry = ry;
		this->rz = rz;
		this->scale = scale;
	}

	float getTextureXOffset() {
		int column = textureIndex % model.texture.numberOfRows;
		return (float)column / (float)model.texture.numberOfRows;
	}
	float getTextureYOffset() {
		int row = textureIndex / model.texture.numberOfRows;
		return (float)row / (float)model.texture.numberOfRows;
	}
	void Rot(float dx, float dy, float dz) {
		this->rx += dx;
		this->ry += dy;
		this->rz += dz;
	}

	void Up(float dx, float dy, float dz) {
		position.x += dx;
		position.y += dy;
		position.z += dz;
	}
};