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
	Planet* p = new Planet(20, 20);
	addPlanet(p);
	Player* player = new Player(c, p, 10, 10, glm::vec3(30,0,0));
	// player->setActivePlanet(p);
	addPlayer(player);
	//LightSource* l2 = new LightSource();

	//AddLightSource(shared_ptr<LightSource>(l2));

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
