#pragma once

class Texture {
public:
	int textureID;
	float shineDamper = 1;
	float reflectivity = 0;

	int hasTransparency = 0;
	int useFakeLighting = 0;

	int numberOfRows = 1;
	
	Texture() {
		this->textureID = 0;
	}

	Texture(int textureID) {	
		this->textureID = textureID;
	}

	void setHasTransparency(int b){
		hasTransparency = b;
	}

	void setUseFakeLighting(int b){
		useFakeLighting = b;
	}
};