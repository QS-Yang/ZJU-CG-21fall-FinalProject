#version 400 core

in vec2 pTexCoord;
in vec3 surNormal;
in vec3 toLightVector;
in vec3 toCameraVector;

out vec4 FragColor;

uniform sampler2D textureSampler;
uniform vec3 lightColor;
uniform float shineDamper;
uniform float reflectivity;

void main()
{
	vec3 unitNormal = normalize(surNormal);
	vec3 unitLightVector = normalize(toLightVector);

	float nDot1 = dot(unitNormal, unitLightVector);
	float bright = max(nDot1, 0.2);
	vec3 diffuse = bright * lightColor;

	vec3 unitVectorToCamera = normalize(toCameraVector);
	vec3 lightDir = -unitLightVector;
	vec3 reflectDir = reflect(lightDir, unitNormal);

	float specFactor = dot(reflectDir, unitVectorToCamera);
	specFactor = max(specFactor, 0.0);
	float dampedFactor = pow(specFactor, shineDamper);
	vec3 finalSpec = dampedFactor * reflectivity *lightColor; 

	vec4 TexColor = texture(textureSampler, pTexCoord);
	if(TexColor.x < 0.5){
		discard;
	}

	FragColor = vec4(diffuse, 1.0) * texture(textureSampler, pTexCoord) + vec4(finalSpec, 1.0);
	// FragColor = texture(textureSampler, pTexCoord);
}