#pragma once
#include "Model.h"
#include "Matrix.h"
#include "Entity.h"
#include "TexturedModel.h"
#include "Shader.h"
#include "Render.h"
#include "TerrRender.h"
#include "SkyboxRender.h"
#include "ShadowShader.h"
#include "ShadowEntityRender.h"
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

    const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
    unsigned int depthMapFBO;
    unsigned int depthMap;
    
    ShaderProgram shader= ShaderProgram("../Shader.vs", "../Shader.fs");
    //EntityRender renderer = EntityRender(shader);
    EntityRender renderer;

    TerrainRender terrainRender;
    TerrainShader terrainShader = TerrainShader("../TShader.vs", "../TShader.fs");

    ShadowShader shadowShader = ShadowShader("../depth.vs", "../depth.fs");
    mat4 lightSpaceMatrix = mat4(1.0f);
    ShadowEntityRender shadowRender;

    std::map<TexturedModel, std::vector<Entity>> entities;
    std::vector<Terrain> terrains;

    SkyboxRender skyboxRender;
    
    MasterRender(){}

    MasterRender(Loader loader, Camera cam) {
        glEnable(GL_CULL_FACE);
        glEnable(GL_BACK);
        createProjectMatrix();
        renderer = EntityRender(shader, projectMatrix);
        terrainRender = TerrainRender(terrainShader, projectMatrix);
        skyboxRender = SkyboxRender(loader, projectMatrix);
        shadowRender = ShadowEntityRender(shadowShader, lightSpaceMatrix);
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
        //terrainRender.render(terrains, shadowMapRenderer.getToShadowMapSpaceMatrix());
        terrainRender.render(terrains, lightSpaceMatrix);
        terrainShader.Stop();

        skyboxRender.render(camera, vec3(RED, GREEN, BLUE));
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
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, depthMap);
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

    void renderShadowMap(Light sun) {
        glm::mat4 lightProjection, lightView;
        float near_plane = 1300.0f, far_plane = 2200.0f;
        //lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
        lightProjection = glm::ortho(-1300.0f, 0.0f, -250.0f, 250.0f, near_plane, far_plane);
        lightView = glm::lookAt(sun.pos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
        shadowRender.setpvMatrix(lightSpaceMatrix);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        shadowRender.render(entities);
        entities.clear();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // reset viewport
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void prepareShadowFBO() {
        glGenFramebuffers(1, &depthMapFBO);
        // create depth texture
        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        // attach depth texture as FBO's depth buffer
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
         if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
             std::cout << "fail to check fbo" << std::endl;
         }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Clear(){
        shader.Clear();
        terrainShader.Clear();
    }
};