#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Model.h"
#include "Loader.h"
#include "Render.h"
// #include "ObjLoader.h"
#include "OBJFileLoader.h"
#include "Texture.h"
#include "TexturedModel.h"
#include "stb_image.h"
#include "MasterRender.h"
#include "TerrainTexturePack.h"
#include <vector>
#include <iostream>

int main(){
    ObjLoader objloader = ObjLoader();
    ModelData data = objloader.loadObj("../object/Car2.obj");
    cout<<sizeof(data.vertices)<<endl;
    cout<<sizeof(data.indices)<<endl;
}