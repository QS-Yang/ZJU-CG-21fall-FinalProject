#version 400 core

in vec3 position;
in vec2 textureCoord;
in vec3 normal;

out vec2 pTexCoord;
out vec3 surNormal;
out vec3 toLightVector;
out vec3 toCameraVector;
out float visibility;

uniform mat4 transMatrix;
uniform mat4 projectMatrix;
uniform mat4 viewMatrix;
uniform vec3 lightPos;
const float density = 0.0035; //雾的浓度
const float gradient = 5.0; //随距离增加


void main()
{
	vec4 worldPos =transMatrix * vec4(position, 1.0);
	vec4 positionRelativeToCam = viewMatrix * worldPos;
	gl_Position = projectMatrix * positionRelativeToCam;
	pTexCoord = textureCoord * 40; //请勿修改

	toLightVector = lightPos - worldPos.xyz;
	toCameraVector = (inverse(viewMatrix) * vec4(0, 0, 0, 1)).xyz - worldPos.xyz;

	float distance = length(positionRelativeToCam.xyz);
	visibility = exp(-pow((distance*density),gradient));
	visibility = clamp(visibility, 0.0, 1.0);
}