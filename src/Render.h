#pragma once
#include "Model.h"
#include "Matrix.h"
#include "Entity.h"
#include "Player.h"
#include "TexturedModel.h"
#include "Shader.h"
#include <vector>
#include <map>

using namespace glm;

extern const unsigned int SCR_WIDTH;
extern const unsigned int SCR_HEIGHT;

class EntityRender {
private:

	ShaderProgram shader;

public:
	EntityRender() {

	}
	EntityRender(ShaderProgram shader, mat4 projectMatrix){
		this->shader = shader;
		shader.Start();
		shader.loadProjectMatrix(projectMatrix);
		shader.Stop();
	}

	void newRender(std::map<TexturedModel, std::vector<Entity>> entities){
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
	
	// actually not be used
    void enableCulling(){
        glEnable(GL_CULL_FACE);
        glEnable(GL_BACK);
    }

    void disableCulling() {
        glDisable(GL_CULL_FACE);
        glDisable(GL_BACK);
    }

	void prepareTexturedModel(TexturedModel model){
		Model rawModel = model.rawModel;

		glBindVertexArray(rawModel.Vid);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		Texture texture = model.texture;
		shader.loadNumberOfRows(texture.numberOfRows);
		if(texture.hasTransparency){
			disableCulling();
		}
		shader.loadFakeLightingVariable(texture.useFakeLighting);
		shader.loadShine(texture.shineDamper, texture.reflectivity);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, model.texture.textureID);
	}

	void unbindTextureModel(){
		disableCulling(); 
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindVertexArray(0);
	}

	void prepareInstance(Entity entity){
		mat4 transMatrix = createTransMatirx(entity.position, entity.rx, entity.ry, entity.rz, entity.scale);
		shader.loadTransMatrix(transMatrix);
		shader.loadOffset(entity.getTextureXOffset(), entity.getTextureYOffset());
	}

};