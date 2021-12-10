#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Model.h"
#include "Loader.h"
#include "Render.h"
#include "ObjLoader.h"
#include "Texture.h"
#include "TexturedModel.h"
#include "stb_image.h"
#include "MasterRender.h"
#include "TerrainTexturePack.h"
#include <vector>
#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//窗口设置
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const unsigned int FPS_CAP = 120;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    //glfw初始化和配置
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //glfw创建窗口
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //glad加载所有openGL函数指针
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    //Render
    Loader loader;
    //Load Texture of Terrain
    TerrainTexture backgroundTexture = TerrainTexture(loader.loadTexture("../texture/grass.png"));
    TerrainTexture rTexture = TerrainTexture(loader.loadTexture("../texture/mud.png"));
    TerrainTexture gTexture = TerrainTexture(loader.loadTexture("../texture/mud.png"));
    TerrainTexture bTexture = TerrainTexture(loader.loadTexture("../texture/path.png"));

    TerrainTexturePack texturePack(backgroundTexture, rTexture, gTexture, bTexture);
    TerrainTexture blendMap = TerrainTexture(loader.loadTexture("../texture/blendMap.png"));
    //
    vector<string> filenames{"../object/Car2.obj"};
    Texture rawtexture=Texture(loader.loadTexture("../texture/white.png"));

    vector<Entity> entities;

    for(int i=0; i<filenames.size(); i++){
        ObjLoader Loader = ObjLoader(filenames[i], loader);
        Model model = Loader.Draw();
        TexturedModel texturedmodel=TexturedModel(model, rawtexture);
        texturedmodel.texture.setHasTransparency(0);
        texturedmodel.texture.setUseFakeLighting(0);
        Texture texture = texturedmodel.texture;
        texture.shineDamper = 10;
        texture.reflectivity = 1;
        entities.push_back(Entity(texturedmodel, glm::vec3(0, 0, -25), 0, 0, 0, 1));
    }

    Light light = Light(glm::vec3(0,0,-20), glm::vec3(1,1,1));

    Terrain terrain1(1, 1, loader, texturePack, blendMap, "../texture/heightmap.png");
    // Terrain terrain2(1, 0, loader, texturePack, blendMap, "../texture/heightmap.png");

    // Player try
    ObjLoader PLoader = ObjLoader("../object/Car2.obj", loader);
    Texture Prawtexture=Texture(loader.loadTexture("../texture/mud.png"));
    Model Pmodel = PLoader.Draw();
    TexturedModel Ptexturedmodel=TexturedModel(Pmodel, Prawtexture);
    Ptexturedmodel.texture.setHasTransparency(0);
    Ptexturedmodel.texture.setUseFakeLighting(0);
    Texture Ptexture = Ptexturedmodel.texture;
    Ptexture.shineDamper = 10;
    Ptexture.reflectivity = 1;
    Player player = Player(Ptexturedmodel, glm::vec3(0, 0, 0), 0, 0, 0, 1);
    player.addWindow(window);
    Camera camera= Camera(window, &player);

    // render loop
    MasterRender renderer = MasterRender();
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        player.move(deltaTime, terrain1);
        camera.move();

        renderer.processEntity(player);
        renderer.processTerrain(terrain1);
        // renderer.processTerrain(terrain2);
        for(int i=0;i<entities.size(); i++){
            renderer.processEntity(entities[i]);
        }
        renderer.render(light,camera);
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    renderer.Clear();
    loader.Clean();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

float getCurrentTime() {
    return glfwGetTime();
}