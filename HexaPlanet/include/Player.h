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
	float getMass() const;
	bool isOnGround() const;

	GLuint getVAO();
	GLuint getVBO();
	GLsizei getNumOfVertices() const;
	void updateVertexArray();

private:
	int boundaryCollision(glm::vec3* boundaries, int length);
	int lowerBoundaryCollision(const glm::vec3& pos);
	void UpdateBoundaries();
	Camera* camera;
	Planet* activePlanet;
	float jumpForce;
	float mass;
	float speed;
	bool onGround;
	glm::vec3 pos;
	glm::vec3 rightVector;
	glm::vec3 frontVector;
	glm::vec3 boundaries[8];

	PlayerMode mode;
	float height;
	float width;

	glm::vec3 velocity;

	std::vector<Vertex> vertexArray;
	GLuint vbo;
	GLuint vao;

	bool thirdPerson;
};