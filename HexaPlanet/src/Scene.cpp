#include "Scene.h"
#include <string>
#include <iostream>
Scene::Scene() :
	activeCameraIndex(0),
	activePlanetIndex(0),
	activePlayerIndex(0)
{
	Camera* c = new Camera();
	addCamera(c);
	LightSource* l = new LightSource();
	addLightSource(l);
	Planet* p = new Planet(20, 10);
	addPlanet(p);
	Player* player = new Player(c, p, 16, 1, glm::vec3(30,0,0));
	// player->setActivePlanet(p);
	addPlayer(player);
	gui = new GUI();
	gui->createNewElement("crossHair", glm::vec2(0, 0), glm::vec2(0.015, 0.015), "crossHair.png");
	int pos = 8;
	gui->createNewElement("selector", glm::vec2((float)(pos - 8) / 8, -0.80), glm::vec2(0.6, 0.6), "selector.png");
	gui->createNewElement("colorBar", glm::vec2(0, -0.80), glm::vec2(0.6, 0.6), "colorBar.png");

}

void Scene::addPlanet(Planet* p)
{
	Planet* q = p;
	planets.push_back(q);
}


void Scene::addLightSource(LightSource* lightSource)
{
	lightSources.push_back(lightSource);
}

int Scene::getLightSourceCount() const
{
	return lightSources.size();
}
LightSource& Scene::getLightSource(int index) const
{
	return *lightSources[index];
}

int Scene::getPlanetCount() const
{
	return planets.size();
}

Planet& Scene::getPlanet(int index) const
{
	return *planets[index];
}

Planet& Scene::getActivePlanet() const
{
	return *planets[activePlanetIndex];
}

void Scene::addCamera(Camera* camera)
{
	cameras.push_back(camera);
}

int Scene::getCameraCount() const
{
	return cameras.size();
}

Camera& Scene::getCamera(int index)
{
	return *cameras[index];
}

Camera& Scene::getActiveCamera() const
{
	return *cameras[activeCameraIndex];
}

void Scene::addPlayer(Player* player)
{
	players.push_back(player);
}

int Scene::getPlayerCount() const
{
	return players.size();
}

Player& Scene::getPlayer(int index)
{
	return *players[index];
}

Player& Scene::getActivePlayer() const
{
	return *players[activePlayerIndex];
}

void Scene::setActiveCameraIndex(int index)
{
	activeCameraIndex = index;
}

int Scene::getActiveCameraIndex() const
{
	return activeCameraIndex;
}

void Scene::setActivePlanetIndex(int index)
{
	activePlanetIndex = index;

}

int Scene::getActivePlanetIndex() const
{
	return activePlanetIndex;
}

GUI* Scene::getGUI() const 
{
	return gui;
}
