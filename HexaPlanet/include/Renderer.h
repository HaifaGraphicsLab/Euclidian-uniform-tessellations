#pragma once
#include "Scene.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include "ShaderProgram.h"
#include "Texture2D.h"


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
	void LoadTextures(std::string name);
	void LoadNormalMap(std::string name);

	glm::vec3 GetBarycentricCoords(const glm::vec2& p, glm::vec3* triangle_v);

	void SwapBuffers();
	void ClearColorBuffer(const glm::vec3& color);
	void ClearZBuffer(float depth);
	int GetViewportWidth() const;
	int GetViewportHeight() const;

	//void ApplyFilter(float* mask , int size_x, int size_y);
	void SetViewport(float width, float height);

	
private:

	glm::vec3 WorldToCamera(const glm::vec3& point, const Camera& c);
	glm::vec2 CameraToCoordinates(const glm::vec3& point, bool& OOS);

	rectangle GetBoundingRectangle(glm::vec3* tri);


	void CreateOpenglBuffer();
	void InitOpenglRendering();


	int viewport_width;
	int viewport_height;

	ShaderProgram shader;
	Texture2D texture;
	Texture2D normalMap;

};
