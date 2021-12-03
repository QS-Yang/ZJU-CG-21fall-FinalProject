#pragma once
#include "Model.h"
#include "Texture.h"

class TexturedModel {
public:
	Model rawModel;
	Texture texture;

	TexturedModel() {

	}
	TexturedModel(Model rawModel, Texture texture) {
		this->rawModel = rawModel;
		this->texture = texture;
	}

	bool operator < (TexturedModel const& A) const  {     
		return rawModel.Vid < A.rawModel.Vid;
    } 
};