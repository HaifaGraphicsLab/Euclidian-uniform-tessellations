#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Face.h"

class Utils
{
public:
	static std::string GetFileName(const std::string& filePath);
	// static std::vector<glm::vec3> CalculateNormals(std::vector<glm::vec3> vertices, std::vector<Face> faces);

};
