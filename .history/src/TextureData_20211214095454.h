
class TextureData {
public:
    int width, height, nrChannels;
    unsigned char *data;

    TextureData(int width, int height, int nrChannels, unsigned char *data) {
        this->width = width;
        this->height = height;
        this->nrChannels = nrChannels;
        this->data = data;
    }

};