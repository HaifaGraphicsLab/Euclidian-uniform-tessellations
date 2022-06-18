#pragma once
#include "ShaderProgram.h"
#include <vector>
#include "Texture2D.h"
struct GUIelement {
	bool active = true;
	string name = "";
	Texture2D* tex;
	glm::vec2 translation;
	glm::vec2 scale;
	glm::mat4 transformation;
};

class GUI {
public:
	GUI();
	~GUI();
	GLuint getVAO();
	GLuint getVBO();
	void createNewElement(const std::string& name, glm::vec2 translation, glm::vec2 scale, const std::string& imagePath);
	GUIelement* getElement(int i);
	GUIelement* getElement(const std::string& name);
	GUIelement* updateTranslation(const std::string& name, const glm::vec2& translation);
	GUIelement* updateScale(const std::string& name, const glm::vec2& scale);
	GUIelement* updateTex(const std::string& name, const std::string& imagePath);
	GUIelement* updateActive(const std::string& name, bool active);

	int getNumOfElements() const;
	bool deleteElement(const std::string& name);

	int GetViewportWidth() const;

	int GetViewportHeight() const;

	void SetViewport(float width, float height);


private:
	int viewport_width;
	int viewport_height;

	std::string path = "..\\gui";
	GLuint vao, vbo;
	float vertices[12] = {-1, 1, -1, -1, 1, 1, 1, 1, -1, -1, 1, -1};
	ShaderProgram shader;
	std::vector<GUIelement> elements;
	glm::mat4 createTransformationMatrix(const glm::vec2& translation, const glm::vec2& scale) const;
	Texture2D* createTex(const std::string& imagePath) const;
};