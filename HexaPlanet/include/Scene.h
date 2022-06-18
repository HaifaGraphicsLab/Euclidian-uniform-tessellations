#pragma once

#include <vector>
#include <memory>

#include "Camera.h"
#include "LightSource.h"
#include "Planet.h"
#include <Player.h>
#include "GUI.h"

using namespace std;

class Scene {
public:
	Scene();

	void addLightSource(LightSource* light_source);
	int getLightSourceCount() const;
	LightSource& getLightSource(int index) const;

	void addPlanet(Planet* p);
	int getPlanetCount() const;
	Planet& getPlanet(int index) const;
	Planet& getActivePlanet() const;
	
	void addCamera(Camera* camera);
	int getCameraCount() const;
	Camera& getCamera(int index);
	Camera& getActiveCamera() const;

	void addPlayer(Player* player);
	int getPlayerCount() const;
	Player& getPlayer(int index);
	Player& getActivePlayer() const;

	void setActiveCameraIndex(int index);
	int getActiveCameraIndex() const;

	void setActivePlanetIndex(int index);
	int getActivePlanetIndex() const;

	GUI* getGUI() const;
	
private:
	vector<Planet *> planets;
	vector<Camera *> cameras;
	vector<Player*> players;
	vector<LightSource *> lightSources;
	GUI* gui;

	int activeCameraIndex;
	int activePlanetIndex;
	int activePlayerIndex;
};