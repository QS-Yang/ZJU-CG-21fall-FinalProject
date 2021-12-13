#pragma once
#include "Model.h"
#include "Camera.h"
#include "Matrix.h"
#include "Light.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace glm;

#define MAX_LIGHTS 4

class TerrainShader {
private:
	int programID;
	int vertexShaderID;
	int fragmentShaderID;
	int transMatrixLocation;
	int projectMatrixLocation;
	int viewMatrixLocation;
	int lightPosLocation[MAX_LIGHTS];
	int lightColorLocation[MAX_LIGHTS];
	int attenuationLocation[MAX_LIGHTS]={0};
	int shineDamperLocation;
	int reflectLocation;
	int skyColorLocation;
	int location_backgroundTexture;
	int location_rTexture;
	int location_gTexture;
	int location_bTexture;
	int location_blendMap;

	unsigned int loadShader(int type, const char* Path) {
		std::string Code;
		std::ifstream ShaderFile;
		try
		{
			ShaderFile.open(Path);
			std::stringstream ShaderStream;
			ShaderStream << ShaderFile.rdbuf();
			ShaderFile.close();
			Code = ShaderStream.str();
		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const char* ShaderCode = Code.c_str();

		unsigned int ShaderID;
		ShaderID = glCreateShader(type);
		glShaderSource(ShaderID, 1, &ShaderCode, NULL);
		glCompileShader(ShaderID);
		int  success;
		char infoLog[512];
		glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(ShaderID, 512, NULL, infoLog);
			if (type == GL_VERTEX_SHADER)
				std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			else
				std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		return ShaderID;
	}

public:
	TerrainShader() {

	}
	TerrainShader(const char* vertexPath, const char* fragmentPath) {
		vertexShaderID = loadShader(GL_VERTEX_SHADER, vertexPath);
		fragmentShaderID = loadShader(GL_FRAGMENT_SHADER, fragmentPath);
		programID = glCreateProgram();
		glAttachShader(programID, vertexShaderID);
		glAttachShader(programID, fragmentShaderID);
		BindAttrib();
		glLinkProgram(programID);

		int  success;
		char infoLog[512];
		glGetProgramiv(programID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(programID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		glValidateProgram(programID);
		getAllUniformLocations();
	}

	void Start() {
		glUseProgram(programID);
	}

	void Stop() {
		glUseProgram(0);
	}

	void Clear() {
		Stop();
		glDetachShader(programID, vertexShaderID);
		glDetachShader(programID, fragmentShaderID);
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		glDeleteProgram(programID);
	}

	void BindAttrib() {
		glBindAttribLocation(programID, 0, "position");
		glBindAttribLocation(programID, 1, "textureCoord");
		glBindAttribLocation(programID, 2, "normal");
	}

	void getAllUniformLocations() {
		transMatrixLocation = getUniformLocation("transMatrix");
		projectMatrixLocation = getUniformLocation("projectMatrix");
		viewMatrixLocation = getUniformLocation("viewMatrix");
		shineDamperLocation = getUniformLocation("shineDamper");
		reflectLocation = getUniformLocation("reflectivity");
		skyColorLocation = getUniformLocation("skyColor");
		for(int i=0; i<MAX_LIGHTS;i++){
			lightPosLocation[i] = getUniformLocation("lightPos[" + to_string(i) + "]");
			lightColorLocation[i] = getUniformLocation("lightColor["+ to_string(i) + "]");
			attenuationLocation[i] = getUniformLocation("attenuation[" + to_string(i)+ "]");
		}

		//check写对没有
		location_backgroundTexture = getUniformLocation("backgroundTexture");
		location_rTexture = getUniformLocation("rTexture");
		location_gTexture = getUniformLocation("gTexture");
		location_bTexture = getUniformLocation("bTexture");
		location_blendMap = getUniformLocation("blendMap");
	}

	void connectTextureUnits() {
		glUniform1i(location_backgroundTexture, 0);
		glUniform1i(location_rTexture, 1);
		glUniform1i(location_gTexture, 2);
		glUniform1i(location_bTexture, 3);
		glUniform1i(location_blendMap, 4);
	}
	
	void loadSkyColor(float r, float g, float b){
		loadVector(skyColorLocation, vec3(r,g,b));
	}

	unsigned int getUniformLocation(std::string uniformName) {
		return glGetUniformLocation(programID, uniformName.c_str());
	}

	void loadShine(float damper, float reflectivity) {
		loadFloat(shineDamperLocation, damper);
		loadFloat(reflectLocation, reflectivity);
	}

	void loadLights(vector<Light> lights){
		for(int i=0; i<MAX_LIGHTS; i++){
			if(i < lights.size()){
				loadVector(lightPosLocation[i], lights[i].pos);
				loadVector(lightColorLocation[i], lights[i].color);
				loadVector(attenuationLocation[i], lights[i].attenuation); 
			}else{
				loadVector(lightPosLocation[i], vec3(0, 0, 0));
				loadVector(lightColorLocation[i], vec3(0, 0, 0));
				loadVector(attenuationLocation[i], vec3(1, 0, 0));
			}
		}
	}

	void loadTransMatrix(mat4 matrix) {
		loadMatrix(transMatrixLocation, matrix);
	}

	void loadProjectMatrix(mat4 matrix) {
		loadMatrix(projectMatrixLocation, matrix);
	}

	void loadViewMatrix(Camera camera) {
		mat4 viewMatrix = createViewMatrix(camera);
		loadMatrix(viewMatrixLocation, viewMatrix);
	}

	void loadFloat(int location, float value) {
		glUniform1f(location, value);
	}

	void loadVector(int location, vec3 vector) {
		glUniform3f(location, vector.x, vector.y, vector.z);
	}

	void loadBoolean(int location, bool value) {
		float toLoad = 0;
		if (value) {
			toLoad = 1;
		}
		glUniform1f(location, toLoad);
	}

	void loadMatrix(int location, mat4 matrix) {
		glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(matrix));
	}

};