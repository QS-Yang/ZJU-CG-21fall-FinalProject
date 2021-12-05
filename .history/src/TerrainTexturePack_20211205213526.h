#pragma once
#include "TerrainTexture.h"

class TerrainTexturePack {
private:
    TerrainTexture backgroundTexture;
    TerrainTexture rTexture;
    TerrainTexture gTexture;
    TerrainTexture bTexture;

public:
    TerrainTexturePack() {

    }

    TerrainTexturePack(TerrainTexture backgroundTexture, TerrainTexture rTexture, TerrainTexture gTexture, TerrainTexture bTexture) {
        this->backgroundTexture = backgroundTexture;
        this->rTexture = rTexture;
        this->gTexture = gTexture;
        this->bTexture = bTexture;
    }

    TerrainTexture getBackgroundTexture() {
        return backgroundTexture;
    }

    TerrainTexture getRTexture() {
        return rTexture;
    }

    TerrainTexture getGTexture() {
        return gTexture;
    }

    TerrainTexture getBTexture() {
        return bTexture;
    }

};