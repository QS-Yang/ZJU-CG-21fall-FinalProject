#version 400 core

in vec3 textureCoords;
out vec4 FragColor;

uniform samplerCube cubeMap;

void main(void) {
    FragColor = texture(cubeMap, textureCoords);
}