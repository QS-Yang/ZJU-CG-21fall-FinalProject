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
	//mat4 projectMatrix;
	//float FOV = 70.0;
	//float nearPlane = 0.1;
	//float farPlane = 1000;
	ShaderProgram shader;

public:
	EntityRender() {

	}
	EntityRender(ShaderProgram shader, mat4 projectMatrix){
		this->shader = shader;
		//glEnable(GL_CULL_FACE);
		//glEnable(GL_BACK);
		//createProjectMatrix();
		shader.Start();
		shader.loadProjectMatrix(projectMatrix);
		shader.Stop();
	}

	/*
	void Prepare() {
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glClear(GL_COLOR_BUFFER_BIT);
	}
	*/
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
	}

	// void Redenring(Entity entity, ShaderProgram shader) {
	// 	TexturedModel model = entity.model;
	// 	Model rawModel = model.rawModel;

	// 	glBindVertexArray(rawModel.Vid);
	// 	glEnableVertexAttribArray(0);
	// 	glEnableVertexAttribArray(1);
	// 	glEnableVertexAttribArray(2);
	// 	mat4 transMatrix = createTransMatirx(entity.position, entity.rx, entity.ry, entity.rz, entity.scale);
	// 	shader.loadTransMatrix(transMatrix);
	// 	Texture texture = model.texture;
	// 	shader.loadShine(texture.shineDamper, texture.reflectivity);

	// 	glActiveTexture(GL_TEXTURE0);
	// 	glBindTexture(GL_TEXTURE_2D, model.texture.textureID);
	// 	glDrawElements(GL_TRIANGLES, rawModel.VertexCount, GL_UNSIGNED_INT, 0);
	// 	glDisableVertexAttribArray(0);
	// 	glDisableVertexAttribArray(1);
	// 	glDisableVertexAttribArray(2);
	// 	glBindVertexArray(0);
	// }

	/*
	void createProjectMatrix(){
		float aspectRatio = (float)SCR_WIDTH/(float)SCR_HEIGHT;
		float y_scale = (float) (1.0/tan(FOV/2.0/360.0*2*Pi))*aspectRatio;
		float x_scale = y_scale/aspectRatio;
		float frustum_len = farPlane - nearPlane;

		projectMatrix = mat4(1.0f);
		projectMatrix[0][0] = x_scale;
		projectMatrix[1][1] = y_scale;
		projectMatrix[2][2] = -((farPlane+nearPlane)/frustum_len);
		projectMatrix[2][3] = -1;
		projectMatrix[3][2] = -((2*nearPlane+farPlane)/frustum_len);
		projectMatrix[3][3] = 0;
	}
	*/
};