#pragma once
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#define Pi 3.1415926

using namespace glm;

mat4 createTransMatirx(vec3 trans, float rx, float ry, float rz, float _scale) {
	mat4 ret = mat4(1.0f);
	ret = translate(ret, trans);
	ret = rotate<float>(ret, radians(rx), vec3(1.0f, 0.0f, 0.0f));
	ret = rotate<float>(ret, radians(ry), vec3(0.0f, 1.0f, 0.0f));
	ret = rotate<float>(ret, radians(rz), vec3(0.0f, 0.0f, 1.0f));
	ret = scale<float>(ret, vec3(_scale, _scale, _scale));
	return ret;
}

mat4 createViewMatrix(Camera camera){
	mat4 viewMat = mat4(1.0);
	viewMat = rotate<float>(viewMat, radians(camera.pitch), vec3(1, 0, 0));
	viewMat = rotate<float>(viewMat, radians(camera.yaw), vec3(0, 1, 0));
	vec3 cameraPos = camera.position;
	vec3 negCameraPos = -cameraPos;
	viewMat = translate<float>(viewMat, negCameraPos);
	return viewMat;
}