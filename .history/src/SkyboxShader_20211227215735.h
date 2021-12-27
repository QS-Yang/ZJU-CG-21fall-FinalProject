#pragma once
#include <iostream>
#include "skybox.h"
#include "Shader.h"

class SkyboxShader {
private:
	unsigned int programID;
	unsigned int vertexShaderID;
	unsigned int fragmentShaderID;
    unsigned int projectMatrixLocation;
    unsigned int viewMatrixLocation;
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
	SkyboxShader(){}
    SkyboxShader(const char* vertexPath, const char* fragmentPath) {
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

	void loadMatrix(unsigned int location, mat4 matrix) {
		glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(matrix));
	}

	void loadFogColor(vec3 color) {
		//printf("%f %f %f\n", color.x, color.y, color.z);
		//printf("%d\n", frogColorLocation);
		glUniform3f(frogColorLocation, color.x, color.y, color.z);
	}
	
	void loadViewMatrix(Camera camera) {
		mat4 viewMatrix = createViewMatrix(camera);
		mat4 view =  glm::mat4(glm::mat3(viewMatrix));
		loadMatrix(viewMatrixLocation, view);
	}

	void loadProjectMatrix(mat4 matrix) {
		loadMatrix(projectMatrixLocation, matrix);
	}

	void getAllUniformLocations() {
		projectMatrixLocation = glGetUniformLocation(programID, "projectionMatrix");
		viewMatrixLocation = glGetUniformLocation(programID, "viewMatrix");
	}

	void BindAttrib() {
		glBindAttribLocation(programID, 0, "position");
		glBindAttribLocation(programID, 1, "textureCoord");
	}

	void setInt(const std::string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation( programID, name.c_str()), value); 
    }
};