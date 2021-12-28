#version 330 core

in vec2 pTexCoord;
in vec3 surNormal;
in vec3 toLightVector[4];
in vec3 toCameraVector;
in float visibility;
in vec4 shadowCoords;

out vec4 FragColor;

// uniform sampler2D textureSampler;
uniform sampler2D backgroundTexture;
uniform sampler2D rTexture;
uniform sampler2D gTexture;
uniform sampler2D bTexture;
uniform sampler2D blendMap;
uniform sampler2D shadowMap;

uniform vec3 lightColor[4];
uniform vec3 attenuation[4];
uniform float shineDamper;
uniform float reflectivity;
uniform vec3 skyColor;
uniform vec3 lightDirection[4];
uniform float lightCutoff[4];

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 shadowCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    shadowCoords = shadowCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, shadowCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = shadowCoords.z;
    // check whether current frag pos is in shadow
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    //float bias = 0.001;
	float shadow = currentDepth - bias > closestDepth  ? 0.7 : 1.0; 
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.

    return shadow;
}

void main()
{
	vec4 blendMapColor = texture(blendMap, pTexCoord);
	float backTextureAmount = 1 - (blendMapColor.r + blendMapColor.g + blendMapColor.b);
	vec2 tiledCoords = pTexCoord * 40.0;
	vec4 backgroundTextureColor = texture(backgroundTexture, tiledCoords) * backTextureAmount;
	vec4 rTextureColor = texture(rTexture, tiledCoords) * blendMapColor.r;
	vec4 gTextureColor = texture(gTexture, tiledCoords) * blendMapColor.g;
	vec4 bTextureColor = texture(bTexture, tiledCoords) * blendMapColor.b;

	vec4 totalColor = backgroundTextureColor + rTextureColor + gTextureColor + bTextureColor;
	// totalColor = totalColor * shadowCoords.z;

	vec3 unitNormal = normalize(surNormal);
	vec3 unitVectorToCamera = normalize(toCameraVector);

	vec3 totalDiff = vec3(0,0,0);
	vec3 totalSpec = vec3(0,0,0);

	for(int i=0;i<4;i++){
		float distance = length(toLightVector[i])/5;
		float attFactor = attenuation[i].x+(attenuation[i].y*distance)+(attenuation[i].z*distance*distance);
		vec3 unitLightVector = normalize(toLightVector[i]);
		float nDot1 = dot(unitNormal, unitLightVector);
		float bright = max(nDot1, 0.0);
		vec3 lightDir = -unitLightVector;
		vec3 reflectDir = reflect(lightDir, unitNormal);
		float specFactor = dot(reflectDir, unitVectorToCamera);
		specFactor = max(specFactor, 0);
		float dampedFactor = pow(specFactor, shineDamper);
		
		float theta = dot(lightDir, normalize(-lightDirection[i]));
		if(theta < lightCutoff[i] ){
			totalDiff += (bright * lightColor[i])/attFactor;
			totalSpec += (dampedFactor * reflectivity *lightColor[i])/attFactor;
		}
	}
	float lightFactor = ShadowCalculation(shadowCoords);
	totalDiff = max(totalDiff, 0.2) * lightFactor;

	 FragColor = vec4(totalDiff, 1.0) * totalColor + vec4(totalSpec, 1.0);
	 FragColor = mix(vec4(skyColor,1.0), FragColor, visibility);
	// FragColor = texture(textureSampler, pTexCoord);
}