#pragma once
#include "Camera.h"
#include "Planet.h"
enum PlayerMode {
	WALKING,
	FLYING, 
};

class Player {
public:
	Player(Camera* camera, Planet* activePlanet, float jumpForce, float mass, glm::vec3 pos);
	void UpdatePos(float deltaTime);
	void UpdateCameraUp();
	void UpdateCameraPos();
	void UpdateCamera();
	void move(Direction d, float stride);
	void setActivePlanet(Planet* planet);
	Planet* getActivePlanet() const;
	void setJumpForce(float jumpForce);
	void setMass(float mass);
	void setSpeed(float speed);
	glm::vec3 getVelocity() const;
	void setThirdPerson(bool thirdPerson);
	bool isThirdPerson();
	float getSpeed() const;
	float getJumpForce() const;
	bool hasJetpack() const;
	void setJetpack(bool b);
	float getMass() const;
	bool isOnGround() const;
	PlayerMode getMode() const;
	void setMode(PlayerMode m);

	GLuint getVAO();
	GLuint getVBO();
	GLsizei getNumOfVertices() const;
	void updateVertexArray();

private:
	int boundaryCollision(glm::vec3* boundaries, int length);
	void getNeighborTriangles(std::vector<Vertex>* vertexArray) const;
	//bool collided(glm::vec3 stride, glm::vec3* outNewPos);
	int lowerBoundaryCollision(const glm::vec3& pos);
	glm::vec3 closestPointOnTriangle(glm::vec3* tri, const glm::vec3& p) const;
	bool collided(const glm::vec3& sphereCenter, std::vector<glm::vec3>& n) const;
	bool triangleCollided(const glm::vec3& sphereCenter, glm::vec3* tri, glm::vec3* n = NULL) const;
	//bool collided(const glm::vec3& sphereCenter, const glm::vec3& vel, glm::vec3* tri, glm::vec3* outSphereCenter);
	void move2(Direction d, float stride);
	void UpdateBoundaries();
	Camera* camera;
	Planet* activePlanet;
	float jumpForce;
	float mass;
	float speed;
	bool onGround;
	bool jetpack;
	glm::vec3 pos;
	glm::vec3 rightVector;
	glm::vec3 frontVector;
	glm::vec3 boundaries[8];
	glm::mat3 sphereToElipse;
	glm::mat3 elipseToSphere;

	PlayerMode mode;
	float height;
	float width;

	glm::vec3 velocity;

	std::vector<Vertex> vertexArray;
	GLuint vbo;
	GLuint vao;

	bool thirdPerson;
};