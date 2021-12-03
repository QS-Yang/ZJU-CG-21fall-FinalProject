#pragma once
#include "TexturedModel.h"

using namespace glm;

class Entity {
public:
	TexturedModel model;
	vec3 position;
	float rx, ry, rz;
	float scale;

	Entity(TexturedModel model, vec3 position, float rx, float ry, float rz, float scale) {
		this->model = model;
		this->position = position;
		this->rx = rx;
		this->ry = ry;
		this->rz = rz;
		this->scale = scale;
	}

	void Rot(float dx, float dy, float dz) {
		this->rx += dx;
		this->ry += dy;
		this->rz += dz;
	}

	void Up(float dx, float dy, float dz) {
		this->position.x += dx;
		this->position.y += dy;
		this->position.z += dz;
	}
};