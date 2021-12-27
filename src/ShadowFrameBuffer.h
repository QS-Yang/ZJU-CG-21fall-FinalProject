#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class ShadowFrameBuffer {
private:
    void initialiseFrameBuffer() {
        // configure depth map FBO
        glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        // create depth texture
        glGenTextures(1, &shadowMap);
        glBindTexture(GL_TEXTURE_2D, shadowMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WIDTH, HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // attach depth texture as FBO's depth buffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);

        unbindFrameBuffer();
	}


public:
    int WIDTH;
    int HEIGHT;
    unsigned int fbo;
    unsigned int shadowMap;
    ShadowFrameBuffer(){}
    ShadowFrameBuffer(int width, int height) {
        this->WIDTH = width;
        this->HEIGHT = height;
        initialiseFrameBuffer();
    }

    void cleanup() {
        glDeleteFramebuffers(1, &fbo);
        glDeleteTextures(1, &shadowMap);
    }
    
    void bindFrameBuffer() {
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
		glViewport(0, 0, WIDTH, HEIGHT);
	}

    void unbindFrameBuffer() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

};