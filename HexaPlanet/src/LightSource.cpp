#include "LightSource.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

LightSource::LightSource()
{
	ambientRGB = glm::vec3(1.0, 1.0, 1.0);
	diffuseRGB = glm::vec3(1.0, 1.0, 1.0);
	specularRGB = glm::vec3(1.0, 1.0, 1.0);
	ambientStrength = 0.5;
	diffuseStrength = 0.5;
	specularStrength = 0.5;
	objectFrame_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	objectFrame_translation = glm::vec3(0.0f, 0.0f, 0.0f);
	objectFrame_scale = glm::vec3(500.0f, 500.0f, 500.0f);
	worldFrame_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	worldFrame_translation = glm::vec3(0.0f, 600.0f, -1500.0f);
	worldFrame_scale = glm::vec3(1.0f, 1.0f, 1.0f);
	display_axes = false;
}

glm::mat4x4 LightSource::GetObjectTransform() const
{
	glm::mat4x4 scale = glm::scale(objectFrame_scale);
	glm::mat4x4 rotate = glm::eulerAngleYXZ(objectFrame_rotation[1], objectFrame_rotation[0], objectFrame_rotation[2]);
	glm::mat4x4 translate = glm::translate(objectFrame_translation);
	return  translate * rotate * scale;
}

glm::mat4x4 LightSource::GetWorldTransform() const
{
	glm::mat4x4 scale = glm::scale(worldFrame_scale);
	glm::mat4x4 rotate = glm::eulerAngleYXZ(worldFrame_rotation[1], worldFrame_rotation[0], worldFrame_rotation[2]);
	glm::mat4x4 translate = glm::translate(worldFrame_translation);
	return translate * rotate * scale;
}

glm::mat4x4 LightSource::GetTransform() const
{
	return GetWorldTransform() * GetObjectTransform();
}
