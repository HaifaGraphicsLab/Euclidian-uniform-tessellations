#pragma once
#include <glm/glm.hpp>

class LightSource 
{
public:
	LightSource();
	glm::vec3 objectFrame_translation;
	glm::vec3 objectFrame_rotation;
	glm::vec3 objectFrame_scale;

	glm::vec3 worldFrame_translation;
	glm::vec3 worldFrame_rotation;
	glm::vec3 worldFrame_scale;

	glm::vec3 ambientRGB;
	glm::vec3 diffuseRGB;
	glm::vec3 specularRGB;
	float ambientStrength;
	float diffuseStrength;
	float specularStrength;
	bool display_axes;
	glm::mat4x4 GetObjectTransform() const;
	glm::mat4x4 GetWorldTransform() const;
	glm::mat4x4 GetTransform() const;
};