#pragma once

const float SIZE = 500.0f;
const float VERTEICES[] = {
    -SIZE, SIZE, -SIZE,
    -SIZE, -SIZE, -SIZE,
    SIZE, -SIZE, -SIZE,
    SIZE, -SIZE, -SIZE,
    SIZE, SIZE, -SIZE,
    -SIZE, SIZE, -SIZE,

    -SIZE, -SIZE, SIZE,
    -SIZE, -SIZE, -SIZE,
    -SIZE, SIZE, -SIZE,
    -SIZE, SIZE, -SIZE,
    -SIZE, SIZE, SIZE,
    -SIZE, -SIZE, SIZE,

    SIZE, -SIZE, -SIZE,
    SIZE, -SIZE, SIZE,
    SIZE, SIZE, SIZE,
    SIZE, SIZE, SIZE,
    SIZE, SIZE, -SIZE,
    SIZE, -SIZE, -SIZE,

    -SIZE, -SIZE, SIZE,
    -SIZE, SIZE, SIZE,
    SIZE, SIZE, SIZE,
    SIZE, SIZE, SIZE,
    SIZE, -SIZE, SIZE,
    -SIZE, -SIZE, SIZE,

    -SIZE, SIZE, -SIZE,
    SIZE, SIZE, -SIZE,
    SIZE, SIZE, SIZE,
    SIZE, SIZE, SIZE,
    -SIZE, SIZE, SIZE,
    -SIZE, SIZE, -SIZE,

    -SIZE, -SIZE, -SIZE,
    -SIZE, -SIZE, SIZE,
    SIZE, -SIZE, -SIZE,
    SIZE, -SIZE, -SIZE,
    -SIZE, -SIZE, SIZE,
    SIZE, -SIZE, SIZE
};

class SkyboxRender {
private:
    float size = 500.0f;
    float VERTICES
public:
    SkyboxRender(/* args */);
    ~SkyboxRender();
};
