#pragma once
#include "Model.h"
#include "Matrix.h"
#include "Entity.h"
#include "Player.h"
#include "TexturedModel.h"
#include "ShadowShader.h"
#include <vector>
#include <map>

using namespace glm;

class ShadowEntityRender {
private:
    mat4 projectionViewMatrix;
    ShadowShader shader;

public:
	ShadowEntityRender() {}
    ShadowEntityRender(ShadowShader shader, mat4 pvMatrix) {
        this->shader = shader;
        this->projectionViewMatrix = pvMatrix;
    }

    void render(std::map<TexturedModel, std::vector<Entity>> entities) {
		shader.Start();
		map<TexturedModel, std::vector<Entity>>::iterator iter;
		for(iter=entities.begin(); iter!=entities.end(); iter++){
			prepareTexturedModel(iter->first);
			std::vector<Entity> batch = iter->second;
			for(int i=0; i<batch.size(); i++){
				prepareInstance(batch[i]);
				glDrawElements(GL_TRIANGLES, iter->first.rawModel.VertexCount, GL_UNSIGNED_INT, 0);
			}
			unbindTextureModel();
		}

	}

	void setpvMatrix(mat4 pvMatrix) {
		this->projectionViewMatrix = pvMatrix;
	}

    void prepareTexturedModel(TexturedModel model){
		Model rawModel = model.rawModel;

		glBindVertexArray(rawModel.Vid);
		glEnableVertexAttribArray(0);
	}

	void unbindTextureModel(){
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}

	void prepareInstance(Entity entity){
		mat4 transMatrix = createTransMatirx(entity.position, entity.rx, entity.ry, entity.rz, entity.scale);
		mat4 mvpMatrix = projectionViewMatrix * transMatrix;
		shader.loadMvpMatrix(mvpMatrix);
	}
};