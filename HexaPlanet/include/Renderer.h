#pragma once
#include "Scene.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "MousePicking.h"
#include <GUI.h>


struct rectangle {
	float l_x = std::numeric_limits<float>::max();
	float l_y = std::numeric_limits<float>::max();
	float h_x = std::numeric_limits<float>::min();
	float h_y = std::numeric_limits<float>::min();
};

class Renderer
{
public:
	Renderer(int viewportWidth, int viewportHeight);
	virtual ~Renderer();
	void Render(const Scene& scene);

	glm::vec3 GetBarycentricCoords(const glm::vec2& p, glm::vec3* triangle_v);

	int GetViewportWidth() const;
	int GetViewportHeight() const;

	void SetViewport(float width, float height);
	PixelInfo getPixelInfo(unsigned int x, unsigned int y);
	
private:
	void pickingPhase(const Scene& scene);
	void renderingPhase(const Scene& scene);
	void guiPhase(GUI& gui);

	glm::vec3 WorldToCamera(const glm::vec3& point, const Camera& c);
	glm::vec2 CameraToCoordinates(const glm::vec3& point, bool& OOS);

	rectangle GetBoundingRectangle(glm::vec3* tri);

	void InitOpenglRendering();

	int viewport_width;
	int viewport_height;

	ShaderProgram shader;
	ShaderProgram pickingShader;
	ShaderProgram guiShader;
	MousePicker pickingTex;

};
