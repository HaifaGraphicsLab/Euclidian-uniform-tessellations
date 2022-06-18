# include "GUI.h"
#include <glm\gtx\transform.hpp>
#include <algorithm>

GUI::GUI()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBindVertexArray(vao);

	glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), &this->vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

GUI::~GUI()
{
	viewport_width = 500;
	viewport_height = 500;
}

GLuint GUI::getVAO()
{
	return vao;
}
GLuint GUI::getVBO()
{
	return vbo;
}

void GUI::createNewElement(const std::string& name, glm::vec2 translation, glm::vec2 scale, const std::string& imagePath)
{
	assert(getElement(name) == nullptr);
	GUIelement ge;
	ge.name = name;
	ge.translation = translation;
	ge.scale = scale;
	ge.transformation = this->createTransformationMatrix(translation, scale);
	ge.tex = this->createTex(imagePath);
	this->elements.push_back(ge);
}

GUIelement* GUI::getElement(int i)
{
	if (i >= this->getNumOfElements()) {
		return nullptr;
	}
	return &this->elements[i];
}

GUIelement* GUI::getElement(const std::string& name)
{
	for (int i = 0; i < this->getNumOfElements(); i++) {
		GUIelement* gePtr = &(elements[i]);
		if (strcmp((*gePtr).name.c_str(), name.c_str()) == 0) {
			return gePtr;
		}
	}
	return nullptr;
}

GUIelement* GUI::updateTranslation(const std::string& name, const glm::vec2& translation)
{
	GUIelement* gePtr = this->getElement(name);
	if (!gePtr) {
		return nullptr;
	}
	gePtr->translation = translation;
	gePtr->transformation = this->createTransformationMatrix(translation, gePtr->scale);
	return gePtr;
}

GUIelement* GUI::updateScale(const std::string& name, const glm::vec2& scale)
{
	GUIelement* gePtr = this->getElement(name);
	if (!gePtr) {
		return nullptr;
	}
	gePtr->scale = scale;
	gePtr->transformation = this->createTransformationMatrix(gePtr->translation, scale);
	return gePtr;
}

GUIelement* GUI::updateTex(const std::string& name, const std::string& imagePath)
{
	GUIelement* gePtr = this->getElement(name);
	if (!gePtr) {
		return nullptr;
	}
	gePtr->tex = this->createTex(imagePath);
	return gePtr;
}

GUIelement* GUI::updateActive(const std::string& name, bool active)
{
	GUIelement* gePtr = this->getElement(name);
	if (!gePtr) {
		return nullptr;
	}
	gePtr->active = active;
	return gePtr;
}

int GUI::getNumOfElements() const
{
	return elements.size();
}

bool GUI::deleteElement(const std::string& name)
{
	GUIelement* gePtr = nullptr;
	int i;
	for (i = 0; i < this->getNumOfElements(); i++) {
		GUIelement* tmpGePtr = &(elements[i]);
		if (strcmp((*tmpGePtr).name.c_str(), name.c_str()) == 0) {
			gePtr = tmpGePtr;
			break;
		}
	}
	if (!gePtr) {
		return false;
	}
	delete gePtr->tex;
	this->elements.erase(this->elements.begin() + i);
	return true;
}

glm::mat4 GUI::createTransformationMatrix(const glm::vec2& translation, const glm::vec2& scale) const
{
	glm::mat4 transMatrix = glm::translate(
		glm::mat4(1.0f),
		glm::vec3(translation, 0.0f)
	);
	return glm::scale(				// Scale first
		transMatrix,              // Translate second
		glm::vec3(scale, 1.0f) * glm::vec3(std::max((float)viewport_height / viewport_width, 1.0f), std::max((float)viewport_width / viewport_height, 1.0f),  1.0f)
	);
}

Texture2D* GUI::createTex(const std::string& imagePath) const
{
	Texture2D* tex = new Texture2D();
	tex->loadTexture(path + '\\' + imagePath, false);
	return tex;
}


int GUI::GetViewportWidth() const
{
	return viewport_width;
}

int GUI::GetViewportHeight() const
{
	return viewport_height;
}

void GUI::SetViewport(float width, float height)
{
	viewport_width = width;
	viewport_height = height;

	for (int i = 0; i < elements.size(); i++) {
		elements[i].transformation = createTransformationMatrix(elements[i].translation, elements[i].scale);
	}
}
