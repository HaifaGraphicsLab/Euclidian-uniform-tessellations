#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Face.h"

class Utils
{
public:
	static glm::vec3 Vec3fFromStream(std::istream& issLine);
	static glm::vec2 Vec2fFromStream(std::istream& issLine);
	// static std::shared_ptr<MeshModel> LoadMeshModel(const std::string& filePath);
	static std::vector<glm::vec3> CalculateNormals(std::vector<glm::vec3> vertices, std::vector<Face> faces);
	static std::string GetFileName(const std::string& filePath);
	// static std::vector<glm::vec3> CalculateNormals(std::vector<glm::vec3> vertices, std::vector<Face> faces);

};
