
class TextureData {
public:
    int width, height;
    unsigned char *data;

    TextureData(int width, int height, unsigned char *data) {
        this->width = width;
        this->height = height;
        this->data = data;
    }

};