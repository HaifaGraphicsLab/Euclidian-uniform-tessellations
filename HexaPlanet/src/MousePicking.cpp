#include "MousePicking.h"
#include <iostream>

MousePicker::MousePicker()
{
}

MousePicker::~MousePicker()
{
}

bool MousePicker::initialization(unsigned int WindowWidth, unsigned int WindowHeight)
{
	// frame buffer object
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	// textures
	glGenTextures(1, &m_pickingTexture);
	glBindTexture(GL_TEXTURE_2D, m_pickingTexture);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32UI, WindowWidth, WindowHeight,
	//	0, GL_RGBA_INTEGER, GL_UNSIGNED_INT, NULL);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, WindowWidth, WindowHeight,
		0, GL_RGB, GL_FLOAT, NULL);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
		m_pickingTexture, 0);

	glGenTextures(1, &m_depthTexture);
	glBindTexture(GL_TEXTURE_2D, m_depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WindowWidth, WindowHeight,
		0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
		m_depthTexture, 0);

	// disable read
	glReadBuffer(GL_NONE);

	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	// check pipeline
	GLenum res = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (res != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", res);
		return false;
	}
	// bind to default
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void MousePicker::enableWriting()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
}

void MousePicker::disableWriting()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

PixelInfo MousePicker::ReadPixel(unsigned int x, unsigned int y)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
	glReadBuffer(GL_COLOR_ATTACHMENT0);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	PixelInfo p;
	
	float fPixel[4] = {-1, -1, -1, -1};
	glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, fPixel);

	p.gridAndNeighbor = fPixel[0];
	p.x = fPixel[1];
	p.y = fPixel[2];
	p.z = fPixel[3];
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	return p;
}
