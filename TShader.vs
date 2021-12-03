#version 400 core

in vec3 position;
in vec2 textureCoord;
in vec3 normal;

out vec2 pTexCoord;
out vec3 surNormal;
out vec3 toLightVector;
out vec3 toCameraVector;

uniform mat4 transMatrix;
uniform mat4 projectMatrix;
uniform mat4 viewMatrix;
uniform vec3 lightPos;

void main()
{
	vec4 worldPos =transMatrix * vec4(position, 1.0);
	gl_Position = projectMatrix * viewMatrix * transMatrix * vec4(position.x, position.y, position.z, 1.0);
	pTexCoord = textureCoord * 40.0;

	surNormal = (transMatrix * vec4(normal,0.0)).xyz;
	toLightVector = lightPos - worldPos.xyz;
	toCameraVector = (inverse(viewMatrix) * vec4(0, 0, 0, 1)).xyz - worldPos.xyz;
}