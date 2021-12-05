
class TerrainTexture {
private:
    unsigned int textureID;

public:
    TerrainTexture(unsigned int textureID) {
        this->textureID = textureID;
    }

    unsigned int getTextureID() {
        return textureID;
    }
};