
class TextureData {
public:
    int width, height;
    char *data;

    TextureData(int width, int height, char *data) {
        this->width = width;
        this->height = height;
        this->data = data;
    }
    
};