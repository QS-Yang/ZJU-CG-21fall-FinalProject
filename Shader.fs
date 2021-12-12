#version 400 core

in vec2 pTexCoord;
in vec3 surNormal;
in vec3 toLightVector[4];  //set to max number of light sources
in vec3 toCameraVector;
in float visibility;

out vec4 FragColor;

uniform sampler2D textureSampler;
uniform vec3 lightColor[4]; //set to max number of light sources
uniform float shineDamper;
uniform float reflectivity;
uniform vec3 skyColor;

void main()
{
	vec3 unitNormal = normalize(surNormal);
	vec3 unitVectorToCamera = normalize(toCameraVector);

	vec3 totalDiff = vec3(0.0);
	vec3 totalSpec = vec3(0.0);

	for(int i=0;i<4;i++){
		vec3 unitLightVector = normalize(toLightVector[i]);
		float nDot1 = dot(unitNormal, unitLightVector);
		float bright = max(nDot1, 0.0);
		vec3 lightDir = -unitLightVector;
		vec3 reflectDir = reflect(lightDir, unitNormal);
		float specFactor = dot(reflectDir, unitVectorToCamera);
		specFactor = max(specFactor, 0);
		float dampedFactor = pow(specFactor, shineDamper);
		totalDiff += bright * lightColor[i];
		totalSpec += dampedFactor * reflectivity *lightColor[i]; 
	}
	totalDiff = max(totalDiff, 0.2);

	vec4 TexColor = texture(textureSampler, pTexCoord);
	// if(TexColor.x<0.5){
	// 	discard;
	// }

	FragColor = vec4(totalDiff, 1.0) * texture(textureSampler, pTexCoord) + vec4(totalSpec, 1.0);
	FragColor = mix(vec4(skyColor,1.0), FragColor, visibility);
	// FragColor = texture(textureSampler, pTexCoord);
}