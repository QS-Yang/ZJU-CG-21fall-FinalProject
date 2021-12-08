#pragma once
#include "Model.h"
#include "Loader.h"
#include "Texture.h"
#include "TerrainTexturePack.h"
#include "TerrainTexture.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define SIZE 800
#define MAX_HEIGHT 40
#define MAX_PIXEL_COLOUR 768
#define cnt VERTEX_COUNT * VERTEX_COUNT

class Terrain {
public:

	float x;
	float z;
	Model model;
	TerrainTexturePack texturePack;
	TerrainTexture blendMap;

	Terrain(int gridX, int gridZ, Loader loader, TerrainTexturePack texturePack, TerrainTexture blendMap, string heightMap) {
		this->texturePack = texturePack;
		this->blendMap = blendMap;
		this->x = gridX * SIZE;
		this->z = gridZ * SIZE;
		this->model = GenTerrain(loader, heightMap);
	}

	Model GenTerrain(Loader loader, string heightMap) {

		int width, height, nrChannels;
		unsigned char* data = stbi_load(heightMap.c_str(), &width, &height, &nrChannels, 0);

		int VERTEX_COUNT = height;
		//int cnt = VERTEX_COUNT * VERTEX_COUNT;
		float* vertices = new float[cnt * 3] ;
		float* normals = new float[cnt * 3];
		float* TexCoord = new float[cnt * 2];
		int* indices = new int[6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1)];

		int vertexPointer = 0;
		for (int i = 0; i < VERTEX_COUNT; i++) {
			for (int j = 0; j < VERTEX_COUNT; j++) {
				vertices[vertexPointer * 3] = -SIZE + (float)j / ((float)VERTEX_COUNT - 1) * SIZE;
				vertices[vertexPointer * 3 + 1] = getHeight(j,i,height, width, data);
				vertices[vertexPointer * 3 + 2] = -SIZE + (float)i / ((float)VERTEX_COUNT - 1) * SIZE;
				vec3 normal = calculateNormal(j, i, height, width, data);
				normals[vertexPointer * 3] = normal.x;
				normals[vertexPointer * 3 + 1] = normal.y;
				normals[vertexPointer * 3 + 2] = normal.z;
				TexCoord[vertexPointer * 2] = (float)j / ((float)VERTEX_COUNT - 1);
				TexCoord[vertexPointer * 2 + 1] = (float)i / ((float)VERTEX_COUNT - 1);
				vertexPointer++;
			}
		}
		int pointer = 0;
		for (int gz = 0; gz < VERTEX_COUNT - 1; gz++) {
			for (int gx = 0; gx < VERTEX_COUNT - 1; gx++) {
				int topLeft = (gz * VERTEX_COUNT) + gx;
				int topRight = topLeft + 1;
				int bottomLeft = ((gz + 1) * VERTEX_COUNT) + gx;
				int bottomRight = bottomLeft + 1;
				indices[pointer++] = topLeft;
				indices[pointer++] = bottomLeft;
				indices[pointer++] = topRight;
				indices[pointer++] = topRight;
				indices[pointer++] = bottomLeft;
				indices[pointer++] = bottomRight;
			}
		}
		stbi_image_free(data);
		return loader.LoadToV(vertices, cnt * 3, TexCoord, cnt * 2, indices, 6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1), normals, cnt * 3);//loadToV(vertices, TexCoord, normals, indices);
	}

private:
	vec3 calculateNormal(int x, int z, int IHeight, int IWidth, unsigned char* data){
		float heightL = getHeight(x-1, z, IHeight, IWidth, data);
		float heightR = getHeight(x+1, z, IHeight, IWidth, data);
		float heightD = getHeight(x-1, z-1, IHeight, IWidth, data);
		float heightU = getHeight(x-1, z+1, IHeight, IWidth, data);
		vec3 normal = vec3(heightL-heightR, 2.0, heightD-heightU);
		normal = normalize(normal);
		return normal;

	}

	float getHeight(int x, int z, int IHeight, int IWidth, unsigned char* data){
		if(x<0 || x >= IHeight || z<0 || z >= IHeight) {
			return 0;
		}

		int base = (x*IHeight + z) * 4;
		int r = data[base], g = data[base+1], b = data[base+2];
		float height = r+g+b;
		height -=  float(MAX_PIXEL_COLOUR/2);
		height /=  float(MAX_PIXEL_COLOUR/2);
		height *=  float(MAX_HEIGHT);
		return height;
	}
};