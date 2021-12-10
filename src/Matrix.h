#pragma once
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

float barryCentric(vec3 p1, vec3 p2, vec3 p3, vec2 pos){
	float det = (p2.z-p3.z)*(p1.x-p3.x)+(p3.x-p2.x)*(p1.z-p3.z);
	float l1 = ((p2.z-p3.z)*(pos.x-p3.x)+(p3.x-p2.x)*(pos.y-p3.z))/det;
	float l2 = ((p3.z-p1.z)*(pos.x-p3.x)+(p1.x-p3.x)*(pos.y-p3.z))/det;
	float l3 = 1.0-l1-l2;
	return l1*p1.y+l2*p2.y+l3*p3.y;
}