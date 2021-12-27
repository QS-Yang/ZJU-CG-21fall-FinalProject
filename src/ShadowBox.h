#pragma once
#include "MasterRender.h"
#include "Camera.h"

using namespace glm;
int flag = 10;

class ShadowBox {
private:
    float OFFSET = 10;
    vec4 UP = vec4(0,1,0,0);
    vec4 FORWARD = vec4(0,0,-1,0);
    float SHADOW_DISTANCE = 100;

	float FOV = 70.0;
    float nearPlane = 0.1;
    float farPlane = 1000;

    float minX, maxX;
	float minY, maxY;
	float minZ, maxZ;
	mat4 lightViewMatrix;
	Camera cam;
	vec4 points[8];

	float farHeight, farWidth, nearHeight, nearWidth; 
	int flag = 1;

    void calculateFrustumVertices(mat4 rotation, vec3 forwardVector, vec3 centerNear, vec3 centerFar) {
		vec3 upVector = vec3(rotation * UP);
		vec3 rightVector = cross(forwardVector, upVector);
		vec3 downVector = vec3(-upVector.x, -upVector.y, -upVector.z);
		vec3 leftVector = vec3(-rightVector.x, -rightVector.y, -rightVector.z);
		vec3 farTop = centerFar + vec3(upVector.x * farHeight, upVector.y * farHeight, upVector.z * farHeight);
		vec3 farBottom = centerFar + vec3(downVector.x * farHeight, downVector.y * farHeight, downVector.z * farHeight);
		vec3 nearTop = centerNear + vec3(upVector.x * nearHeight, upVector.y * nearHeight, upVector.z * nearHeight);
		vec3 nearBottom = centerNear + vec3(downVector.x * nearHeight, downVector.y * nearHeight, downVector.z * nearHeight);
		
		
        //std::cout << "will create points" << std::endl;
		//vec4 points[8];
		points[0] = calculateLightSpaceFrustumCorner(farTop, rightVector, farWidth);
		points[1] = calculateLightSpaceFrustumCorner(farTop, leftVector, farWidth);
		points[2] = calculateLightSpaceFrustumCorner(farBottom, rightVector, farWidth);
		points[3] = calculateLightSpaceFrustumCorner(farBottom, leftVector, farWidth);
		points[4] = calculateLightSpaceFrustumCorner(nearTop, rightVector, nearWidth);
		points[5] = calculateLightSpaceFrustumCorner(nearTop, leftVector, nearWidth);
		points[6] = calculateLightSpaceFrustumCorner(nearBottom, rightVector, nearWidth);
		points[7] = calculateLightSpaceFrustumCorner(nearBottom, leftVector, nearWidth);
		
        //std::cout << "create points boom" << std::endl;
		//return points;
	}

    vec4 calculateLightSpaceFrustumCorner(vec3 startPoint, vec3 direction, float width) {
		vec3 point = startPoint + vec3(direction.x * width, direction.y * width, direction.z * width);
		vec4 point4f = lightViewMatrix * vec4(point.x, point.y, point.z, 1.0f);
		return point4f;
	}

    void calculateWidthsAndHeights() {
		farWidth = (float) (SHADOW_DISTANCE * tan(radians(FOV)));
		nearWidth = (float) (nearPlane * tan(radians(FOV)));
		farHeight = farWidth / getAspectRatio();
		nearHeight = nearWidth / getAspectRatio();
	}

    float getAspectRatio() {
		return (float) SCR_WIDTH / (float) SCR_HEIGHT;
	}

    mat4 calculateCameraRotationMatrix() {
		mat4 rotation = mat4(1.0f);
        rotation = rotate(rotation, radians(-cam.yaw), vec3(0,1,0));
        rotation = rotate(rotation, radians(-cam.pitch), vec3(1,0,0));
		return rotation;
	}
public:
	ShadowBox(){}
    ShadowBox(mat4 lightViewMatrix, Camera camera) {
		this->lightViewMatrix = lightViewMatrix;
		this->cam = camera;
		calculateWidthsAndHeights();
	}

    void update() {
		//std::cout << "update shadowbox" << std::endl;
		mat4 rotation = calculateCameraRotationMatrix();
		vec3 forwardVector = vec3(rotation * FORWARD);

		vec3 toFar = forwardVector * SHADOW_DISTANCE;
		vec3 toNear = forwardVector * nearPlane;
		vec3 centerNear = toNear + cam.position;
		vec3 centerFar = toFar + cam.position;

		calculateFrustumVertices(rotation, forwardVector, centerNear, centerFar);

		int first = 1;
		for (int i=0; i<8; i++) {
			if (first) {
				minX = points[i].x;
				maxX = points[i].x;
				minY = points[i].y;
				maxY = points[i].y;
				minZ = points[i].z;
				maxZ = points[i].z;
				first = 0;
				continue;
			}
			if (points[i].x > maxX) {
				maxX = points[i].x;
			} else if (points[i].x < minX) {
				minX = points[i].x;
			}
			if (points[i].y > maxY) {
				maxY = points[i].y;
			} else if (points[i].y < minY) {
				minY = points[i].y;
			}
			if (points[i].z > maxZ) {
				maxZ = points[i].z;
			} else if (points[i].z < minZ) {
				minZ = points[i].z;
			}
		}
		maxZ += OFFSET;

	}

	void setLightViewMatrix(mat4 Matrix) {
		//std::cout << "set LightViewMatrix" << std::endl;
		lightViewMatrix = Matrix;
	}

	void setCam(Camera c){
		cam = c;
	}

    vec3 getCenter() {
        float x = (minX + maxX) / 2.0f;
		float y = (minY + maxY) / 2.0f;
		float z = (minZ + maxZ) / 2.0f;
		vec4 cen = vec4(x, y, z, 1);
		mat4 invertedLight = inverse(lightViewMatrix);
        return vec3(invertedLight * cen);
    }

    float getWidth() {
		return maxX - minX;
	}

	float getHeight() {
		return maxY - minY;
	}

	float getLength() {
		return maxZ - minZ;
	}

};