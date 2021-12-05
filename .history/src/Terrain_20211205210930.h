#pragma once
#include "Model.h"
#include "Loader.h"
#include "Texture.h"
#include "TerrainTexturePack.h"
#define SIZE 800
#define VERTEX_COUNT 128
#define cnt VERTEX_COUNT * VERTEX_COUNT

class Terrain {
public:

	float x;
	float z;
	Model model;
	TerrainTexturePack texturePack;

	Terrain(int gridX, int gridZ, Loader loader, Texture texture) {
		this->texture = texture;
		this->x = gridX * SIZE;
		this->z = gridZ * SIZE;
		this->model = GenTerrain(loader);
	}

	Model GenTerrain(Loader loader) {
		//int cnt = VERTEX_COUNT * VERTEX_COUNT;
		float* vertices = new float[cnt * 3] ;
		float* normals = new float[cnt * 3];
		float* TexCoord = new float[cnt * 2];
		int* indices = new int[6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1)];

		int vertexPointer = 0;
		for (int i = 0; i < VERTEX_COUNT; i++) {
			for (int j = 0; j < VERTEX_COUNT; j++) {
				vertices[vertexPointer * 3] = -SIZE + (float)j / ((float)VERTEX_COUNT - 1) * SIZE;
				vertices[vertexPointer * 3 + 1] = 0;
				vertices[vertexPointer * 3 + 2] = -SIZE + (float)i / ((float)VERTEX_COUNT - 1) * SIZE;
				normals[vertexPointer * 3] = 0;
				normals[vertexPointer * 3 + 1] = 1;
				normals[vertexPointer * 3 + 2] = 0;
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
		return loader.LoadToV(vertices, cnt * 3, TexCoord, cnt * 2, indices, 6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1), normals, cnt * 3);//loadToV(vertices, TexCoord, normals, indices);
	}
};