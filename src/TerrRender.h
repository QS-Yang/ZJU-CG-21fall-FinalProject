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
	mat4 spaceM;
	int flag = 1;

public:
	TerrainRender() {

	}
	TerrainRender(TerrainShader shader, mat4 projectionMatrix ) {
		this->shader = shader;
		shader.Start();
		shader.loadProjectMatrix(projectionMatrix);
		shader.connectTextureUnits();
		shader.Stop();
	}

	void render(std::vector<Terrain> terrains, mat4 toShadowSpace) {
		spaceM = toShadowSpace;
		shader.loadToShadowSpaceMatrix(toShadowSpace);
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

		bindTextures(terrain);
		shader.loadShine(1, 0); //可能需要修改

		// glActiveTexture(GL_TEXTURE0);
		// glBindTexture(GL_TEXTURE_2D, terrain.texture.textureID);
	}

	void bindTextures(Terrain terrain) {
		TerrainTexturePack texturePack = terrain.texturePack;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturePack.getBackgroundTexture().getTextureID());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texturePack.getRTexture().getTextureID());
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texturePack.getGTexture().getTextureID());
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, texturePack.getBTexture().getTextureID());
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, terrain.blendMap.getTextureID());
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, terrain.shadowMap);
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