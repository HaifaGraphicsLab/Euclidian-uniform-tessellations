#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include <iostream>
#include <random>


#include "Renderer.h"
#include "InitShader.h"
#include <glm/glm.hpp>
#include <algorithm>
#include <gl\GL.h>

#define GLM_SWIZZLE

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
#define Z_INDEX(width,x,y) ((x)+(y)*(width))



Renderer::Renderer(int viewport_width, int viewport_height) :
	viewport_width(viewport_width),
	viewport_height(viewport_height)
{
	InitOpenglRendering();
	// LoadTextures("ao_smile.jpg");
	// LoadTextures("diffuse.png");
	// LoadNormalMap("normalmap.png");
}

Renderer::~Renderer()
{

}

void Renderer::InitOpenglRendering()
{
	shader = ShaderProgram();
	shader.loadShaders("vshader.glsl", "fshader.glsl");

}


void Renderer::Render(const Scene& scene)
{
	
	const Camera camera = scene.getActiveCamera();
	Planet planet = scene.getActivePlanet();
	shader.use();
	// -------------- UNIFORMS -----------------
	glm::mat4 viewTransform = camera.GetViewTransformation();
	glm::mat4 projectionTranform = camera.GetProjectionTransformation();
	shader.setUniform("view", viewTransform);
	shader.setUniform("projection", projectionTranform);

	//// lighting
	//shader.setUniform("viewPos", camera.translation);
	//shader.setUniform("numOfLights", scene.GetLightSourceCount());
	//for (int i = 0; i < scene.GetLightSourceCount(); i++) {
	//	LightSource l = scene.GetLightSource(i);
	//	string light = "lights[" + std::to_string(i) + "]";
	//	shader.setUniform((light + ".pos").c_str(), l.worldFrame_translation);
	//	shader.setUniform((light + ".ambientRGB").c_str(), l.ambientRGB);
	//	shader.setUniform((light + ".diffuseRGB").c_str(), l.diffuseRGB);
	//	shader.setUniform((light + ".specularRGB").c_str(), l.specularRGB);
	//	shader.setUniform((light + ".ambientStrength").c_str(), l.ambientStrength);
	//	shader.setUniform((light + ".diffuseStrength").c_str(), l.diffuseStrength);
	//	shader.setUniform((light + ".specularStrength").c_str(), l.specularStrength);
	//}

	//Material material = model.material;
	//shader.setUniform("material.ambientRGB", material.ambientRGB);
	//shader.setUniform("material.diffuseRGB", material.diffuseRGB);
	//shader.setUniform("material.specularRGB", material.specularRGB);
	//shader.setUniform("material.shininess", model.shininess);


	//// textures
	//texture.bind(0);
	//shader.setUniformSampler("material.textureMap", 0);
	//normalMap.bind(1);
	//shader.setUniformSampler("material.normalMap", 1);
	//shader.setUniform("cylinderMapping", 0);
	//shader.setUniform("hasTexture", 1);
	//if (model.normalMap) {
	//	shader.setUniform("normalMap", 1);
	//}
	//else {
	//	shader.setUniform("normalMap", 0);
	//}

	// -------------- CHUNK RENDERERING -----------------
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	for (int i = 0; i < 20; i++) {
		if (planet.isActive(i)) {
			Chunk* c = planet.getChunk(i);
			GLuint vao = planet.getVAO(i);
			GLsizei count = c->getNumOfVertices();

			// draw chunk
			glBindVertexArray(vao);
			glDrawArrays(GL_TRIANGLES, 0, count);
			glBindVertexArray(0);
		}

	}

}
void Renderer::LoadTextures(std::string name)
{
	if (!texture.loadTexture("bin\\Debug\\" + name, true))
	{
		texture.loadTexture("bin\\Release\\" + name, true);
	}
}
void Renderer::LoadNormalMap(std::string name)
{
	if (!normalMap.loadTexture("bin\\Debug\\" + name, true))
	{
		normalMap.loadTexture("bin\\Release\\" + name, true);
	}
}

glm::vec3 Renderer::WorldToCamera(const glm::vec3& point, const Camera& c) {
	glm::mat4x4 camera_transformation = c.GetInverseTransformation();
	glm::vec4 v4 = camera_transformation * glm::vec4(point, 1.0f);
	if (v4[3] < 0.05){
		v4[3] = 0.05;
	}
	return v4/v4[3];
}
glm::vec2 Renderer::CameraToCoordinates(const glm::vec3& point, bool& OOS) {
	glm::vec2 scale = glm::vec2(viewport_width, viewport_height) * 0.5f;
	OOS = (point[0] < -1 || point[0] > 1 || point[1] < -1 || point[1] > 1);
	return (glm::vec2(point) + glm::vec2(1.0f)) * scale;
}

rectangle Renderer::GetBoundingRectangle(glm::vec3* tri)
{
	rectangle r;
	bool test = false;
	for (int i = 0; i < 3; i++) {
		const glm::vec3 &v = tri[i];
		r.l_x = std::min(r.l_x, (v[0]));
		r.l_y = std::min(r.l_y, (v[1]));
		r.h_x = std::max(r.h_x, (v[0]));
		r.h_y = std::max(r.h_y, (v[1]));
		test = true;
	}
	if (!test) {
		r.l_x = r.l_y = r.h_x = r.h_y = 0;
	}
	return r;
}


glm::vec3 Renderer::GetBarycentricCoords(const glm::vec2& p, glm::vec3* triangle_v)
{
	glm::vec2 v0 = triangle_v[1] - triangle_v[0], v1 = triangle_v[2] - triangle_v[0], v2 = p - glm::vec2(triangle_v[0]);
	float den = v0.x * v1.y - v1.x * v0.y;
	float v = (v2.x * v1.y - v1.x * v2.y) / den;
	float w = (v0.x * v2.y - v2.x * v0.y) / den;
	float u = 1.0f - v - w;
	return glm::vec3(u, v, w);
}



int Renderer::GetViewportWidth() const
{
	return viewport_width;
}

int Renderer::GetViewportHeight() const
{
	return viewport_height;
}

void Renderer::SetViewport(float width, float height)
{
	viewport_width = width;
	viewport_height = height;
}
