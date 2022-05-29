#pragma once
#include <string>
#include <vector>
#include "Grid.h"
#include "Planet.h"

class TerrainStorage {
public:
	static std::vector<std::string> getSavedTerrainNames();
	static void loadTerrain(Planet* p, std::string name);
	static bool isInNames(const std::string& name);
	static void deleteTerrain(const std::string& name);
	static void saveTerrain(Grid* const* terrain, std::string name);
private:
	static void updateSavedTerrainNames();
	static std::string path;
	static std::vector<std::string> names;
	static bool isUpdated;
};