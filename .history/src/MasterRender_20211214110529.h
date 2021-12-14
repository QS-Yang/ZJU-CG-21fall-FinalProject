#pragma once
#include "Model.h"
#include "Matrix.h"
#include "Entity.h"
#include "TexturedModel.h"
#include "Shader.h"
#include "Render.h"
#include "TerrRender.h"
#include "SkyboxRender.h"
#include <vector>
#include <map>

using namespace glm;

class MasterRender{
public:
    mat4 projectMatrix;
    float FOV = 70.0;
    float nearPlane = 0.1;
    float farPlane = 1000;
    float RED = 0.5;
    float GREEN = 0.5;
    float BLUE = 0.5;
    
    ShaderProgram shader= ShaderProgram("../Shader.vs", "../Shader.fs");
    //EntityRender renderer = EntityRender(shader);
    EntityRender renderer;

    TerrainRender terrainRender;
    TerrainShader terrainShader = TerrainShader("../TShader.vs", "../TShader.fs");

    std::map<TexturedModel, std::vector<Entity>> entities;
    std::vector<Terrain> terrains;

    SkyboxRender* skyboxRender;

    MasterRender() {
        glEnable(GL_CULL_FACE);
        glEnable(GL_BACK);
        createProjectMatrix();
        renderer = EntityRender(shader, projectMatrix);
        terrainRender = TerrainRender(terrainShader, projectMatrix);
    }

    void render(vector<Light> lights, Camera camera){
        Prepare();
        shader.Start();
        shader.loadSkyColor(RED, GREEN, BLUE);
        shader.loadLights(lights);
        shader.loadViewMatrix(camera);
        renderer.newRender(entities);
        shader.Stop();

        terrainShader.Start();
        terrainShader.loadSkyColor(RED, GREEN, BLUE);
        terrainShader.loadLights(lights);
        terrainShader.loadViewMatrix(camera);
        terrainRender.render(terrains);
        terrainShader.Stop();
        terrains.clear();
        entities.clear();
    }

    void processTerrain(Terrain terrain) {
        terrains.push_back(terrain);
    }
    void processEntity(Entity entity){
        TexturedModel entityModel = entity.model;
        if(entities.find(entityModel)!=entities.end()){
            entities.at(entityModel).push_back(entity);
        } else {
            std::vector<Entity> batch;
            batch.push_back(entity);
            entities.insert(std::pair<TexturedModel, std::vector<Entity>>(entityModel, batch));
        }
    }

    void Prepare() {
        glEnable(GL_DEPTH_TEST);
        glClearColor(RED, GREEN, BLUE, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glClear(GL_COLOR_BUFFER_BIT);
    }

    void createProjectMatrix() {
        float aspectRatio = (float)SCR_WIDTH / (float)SCR_HEIGHT;
        float y_scale = (float)(1.0 / tan(FOV / 2.0 / 360.0 * 2 * Pi)) * aspectRatio;
        float x_scale = y_scale / aspectRatio;
        float frustum_len = farPlane - nearPlane;

        projectMatrix = mat4(1.0f);
        projectMatrix[0][0] = x_scale;
        projectMatrix[1][1] = y_scale;
        projectMatrix[2][2] = -((farPlane + nearPlane) / frustum_len);
        projectMatrix[2][3] = -1;
        projectMatrix[3][2] = -((2 * nearPlane + farPlane) / frustum_len);
        projectMatrix[3][3] = 0;
    }

    void Clear(){
        shader.Clear();
        terrainShader.Clear();
    }
};