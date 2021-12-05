#pragma once
#include "TerrShader.h"
#include "Terrain.h"
#include "TerrainTexture.h"
#include "TerrainTexturePack.h"

#include <vector>
using namespace glm;

class TerrainRender {

private:
	TerrainShader shader;

public:
	TerrainRender() {

	}
	TerrainRender(TerrainShader shader, mat4 projectionMatrix ) {
		this->shader = shader;
		shader.Start();
		shader.loadProjectMatrix(projectionMatrix);
		shader.Stop();
	}

	void render(std::vector<Terrain> terrains) {
		for (int i = 0; i < terrains.size(); i++) {
			prepareTerrain(terrains[i]);
			loadModelMatrix(terrains[i]);
			glDrawElements(GL_TRIANGLES, terrains[i].model.VertexCount, GL_UNSIGNED_INT, 0);
			unbindTextureModel();
		}
	}

	void prepareTerrain(Terrain terrain) {
		Model rawModel = terrain.model;

		glBindVertexArray(rawModel.Vid);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		Texture texture = terrain.texture;
		shader.loadShine(texture.shineDamper, texture.reflectivity);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, terrain.texture.textureID);
	}

	void bindTextures(Terrain terrain) {
		TerrainTexturePack texturePack = terrain.texturePack;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturePack.getBackgroundTexture().getTextureID());
		glActiveTexture(GL_TEXTURE1);
	}

	void unbindTextureModel() {
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindVertexArray(0);
	}

	void loadModelMatrix(Terrain terrain) {
		mat4 transMatrix = createTransMatirx(vec3(terrain.x, 0, terrain.z), 0, 0, 0, 1);
		shader.loadTransMatrix(transMatrix);
	}
};