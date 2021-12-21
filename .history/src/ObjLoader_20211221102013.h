#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <math.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Loader.h"
using namespace std;

class ObjLoader{
public:
    struct vertex{
		float x;
		float y;
		float z;
	};
    ObjLoader(string filename, Loader loader);
	Model Draw();
    void CleanUp();

private:
    vector<vector<GLfloat>> v;
	vector<vector<GLint>> f;
    vector<vector<GLfloat>> vt;
    vector<vector<GLfloat>> vn;
    vector<vector<GLint>> fvt;
    vector<vector<GLint>> fvn;
    Loader loader;
};
