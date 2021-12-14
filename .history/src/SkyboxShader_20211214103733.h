#pragma once
#include "skybox.h"
#include "Shader.h"

class SkyboxShader {
private:
	unsigned int programID;
	unsigned int vertexShaderID;
	unsigned int fragmentShaderID;
    unsigned int location_projectionMatrix;
    unsigned int location_viewMatrix;
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

	void getAllUniformLocations() {
		location_projectionMatrix = glGetUniformLocation(programID, "projectionMatrix");
		location_viewMatrix = glGetUniformLocation(programID, "viewMatrix");
	}

	void BindAttrib() {
		glBindAttribLocation(programID, 0, "position");
	}
};