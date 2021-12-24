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
	int flag = 10;

public:
	ShadowEntityRender() {}
    ShadowEntityRender(ShadowShader shader, mat4 pvMatrix) {
        this->shader = shader;
        this->projectionViewMatrix = pvMatrix;
    }

    void render(std::map<TexturedModel, std::vector<Entity>> entities) {
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
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}

	void setpvMatrix(mat4 pvMatrix) {
		this->projectionViewMatrix = pvMatrix;
	}

    void prepareTexturedModel(TexturedModel model){
		Model rawModel = model.rawModel;

		glBindVertexArray(rawModel.Vid);
		glEnableVertexAttribArray(0);

		// Texture texture = model.texture;
		// glActiveTexture(GL_TEXTURE0);
		// glBindTexture(GL_TEXTURE_2D, model.texture.textureID);
	}

	void unbindTextureModel(){
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}

	void prepareInstance(Entity entity){
		mat4 transMatrix = createTransMatirx(entity.position, entity.rx, entity.ry, entity.rz, entity.scale);
		mat4 mvpMatrix = projectionViewMatrix * transMatrix;
		// if(flag) {
        //     mat4 matrix = mvpMatrix;
		// 	std::cout << std::endl;
		// 	std::cout << matrix[0][0] << " " << matrix[0][1] << " " << matrix[0][2] << " " << matrix[0][3] << std::endl;
		// 	std::cout << matrix[1][0] << " " << matrix[1][1] << " " << matrix[1][2] << " " << matrix[1][3] << std::endl;
        //     std::cout << matrix[2][0] << " " << matrix[2][1] << " " << matrix[2][2] << " " << matrix[2][3] << std::endl;
        //     std::cout << matrix[3][0] << " " << matrix[3][1] << " " << matrix[3][2] << " " << matrix[3][3] << std::endl;
		// 	flag -= 1;
		// }
		shader.loadMvpMatrix(mvpMatrix);
	}
};