#pragma once
#include <glad/glad.h>
#include "Chunk.h"


class MousePicker {
public:
	MousePicker();
	~MousePicker();
	bool initialization(unsigned int WindowWidth, unsigned int WindowHeight);
	void enableWriting();
	void disableWriting();

	PixelInfo ReadPixel(unsigned int x, unsigned int y);

private:
	GLuint m_fbo;
	GLuint m_pickingTexture;
	GLuint m_depthTexture;
};