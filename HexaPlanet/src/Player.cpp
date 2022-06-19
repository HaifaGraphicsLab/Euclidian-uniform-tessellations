#include "Player.h"
#include <glm/gtx/intersect.hpp>
#include <glm/gtx/closest_point.hpp>
#include <iostream>

Player::Player(Camera* camera, Planet* activePlanet, float jumpForce, float mass, glm::vec3 pos)
{
	this->activePlanet = activePlanet;
	this->camera = camera;
	this->jumpForce = jumpForce;
	this->mass = mass;
	this->pos = pos;
	this->height = 0.5;
	this->width = 0.2;
	this->mode = PlayerMode::WALKING;
	this->velocity = glm::vec3(0);
	this->jetpack = false;
	this->speed = 200;
	this->thirdPerson = false;
	UpdateBoundaries();
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	updateVertexArray();
	this->activeColor = 0;
}

int Player::boundaryCollision(glm::vec3* boundaries, int length)
{
	glm::vec3* boundaryPointer = boundaries;
	Voxel v;
	for (int i = 0; i < length; i++) {
		glm::vec3 boundary = *boundaryPointer;
		if (activePlanet->CheckCollision(boundary, v)) {
			return i;
		}
		boundaryPointer++;
	}
	return -1;
}

void Player::getNeighborTriangles(std::vector<Vertex>* va) const
{
	Voxel n_v, v = this->activePlanet->getVoxel(this->pos);
	bool isPent;
	bool n_isPent;
	if (activePlanet->isValidVoxel(v)) {
		activePlanet->renderVox(v, va, &isPent);
		for (int i = 0; i < 5; i++) {
			n_v = activePlanet->getNeighbor(v, i);
			if (activePlanet->isValidVoxel(n_v)) {
				activePlanet->renderVox(n_v, va, &n_isPent);
				if (n_isPent) {
					continue;
				}
				n_v = activePlanet->getNeighbor(n_v, i);
				if (activePlanet->isValidVoxel(n_v)) {
					activePlanet->renderVox(n_v, va);
				}
			}

		}
		if (!isPent) {
			n_v = activePlanet->getNeighbor(v, 5);
			if (activePlanet->isValidVoxel(n_v)) {
				activePlanet->renderVox(n_v, va, &n_isPent);
				if (!n_isPent) {
					n_v = activePlanet->getNeighbor(n_v, 5);
					if (activePlanet->isValidVoxel(n_v)) {
						activePlanet->renderVox(n_v, va);
					}
				}

			}
		}
		for (int i = 6; i < 8; i++) {
			n_v = activePlanet->getNeighbor(v, i);
			if (activePlanet->isValidVoxel(n_v)) {
				activePlanet->renderVox(n_v, va, &n_isPent);
				if (n_isPent) {
					continue;
				}
				n_v = activePlanet->getNeighbor(n_v, i);
				if (activePlanet->isValidVoxel(n_v)) {
					activePlanet->renderVox(n_v, va);
				}
			}
		}
	}
}
//
//bool Player::collided(glm::vec3 stride, glm::vec3* outNewPos) {
//	std::vector<Vertex> va;
//	glm::vec3 newPos;
//	// this->getNeighborTriangles(&va);
//	Voxel v = this->activePlanet->getVoxel(this->pos);
//	ChunkLoc c = this->activePlanet->GetChunkLoc(v);
//	int index = c.index * 5 + c.chunckRoot;
//	va = this->activePlanet->getChunk(index)->vertexArray;
//	assert(va.size() % 3 == 0);
//	glm::vec3 tri[3];
//	for (int i = 0; i < va.size(); i += 3) {
//		tri[0] = va[i].position;
//		tri[1] = va[i+1].position;
//		tri[2] = va[i+2].position;
//		if (this->collided(this->pos, stride, tri, &newPos)) {
//			*outNewPos = newPos;
//			return true;
//		}
//	}
//	return false;
//
//}

int Player::lowerBoundaryCollision(const glm::vec3& pos) {
	glm::vec3 boundary[4];
	glm::vec3 lowPos = pos - glm::normalize(camera->getCameraUp()) * height / 2.0f;
	boundary[0] = lowPos + (rightVector + frontVector) * width;
	boundary[1] = lowPos + (rightVector - frontVector) * width;
	boundary[2] = lowPos + (-rightVector + frontVector) * width;
	boundary[3] = lowPos + (-rightVector - frontVector) * width;
	return boundaryCollision(boundary, 4);
}

glm::vec3 Player::closestPointOnTriangle(glm::vec3* tri, const glm::vec3& p) const{
	glm::vec3 abMin = glm::closestPointOnLine(p, tri[0], tri[1]);
	glm::vec3 bcMin = glm::closestPointOnLine(p, tri[1], tri[2]);
	glm::vec3 caMin = glm::closestPointOnLine(p, tri[2], tri[0]);

	double minDist = glm::length(abMin-p);
	glm::vec3 result = abMin;
	if (glm::length(bcMin - p) < minDist) {
		minDist = glm::length(bcMin - p);
		result = bcMin;
	}

	if (glm::length(caMin - p) < minDist) {
		result = caMin;
	}

	return result;

}
bool Player::collided(const glm::vec3& sphereCenter, std::vector<glm::vec3>& n) const {
	std::vector<Vertex> va;
	std::vector<Vertex>* tmpVa;
	//this->getNeighborTriangles(&va);
	Voxel v = this->activePlanet->getVoxel(this->pos);

	std::vector<ChunkLoc> cls = this->activePlanet->neighboringChunkLocs(v);
	ChunkLoc c = this->activePlanet->GetChunkLoc(v);
	cls.push_back(c);
	for (ChunkLoc cl : cls) {
		int index = cl.index * 5 + cl.chunckRoot;
		tmpVa = &this->activePlanet->getChunk(index)->vertexArray;
		va.insert(va.end(), tmpVa->begin(), tmpVa->end());
	}

	assert(va.size() % 3 == 0);

	glm::vec3 tri[3];
	bool collided = false;
	glm::vec3 tmpN;
	for (int i = 0; i < va.size(); i += 3) {
		tri[0] = elipseToSphere * va[i].position;
		tri[1] = elipseToSphere * va[i + 1].position;
		tri[2] = elipseToSphere * va[i + 2].position;
		if (this->triangleCollided(elipseToSphere*sphereCenter, tri, &tmpN)) {
			collided = true;
			n.push_back(glm::normalize(sphereToElipse*tmpN));
		}
	}
	return collided;
}

bool Player::triangleCollided(const glm::vec3& sphereCenter, glm::vec3* tri, glm::vec3* n) const {
	glm::vec3 normal = glm::normalize(glm::cross(tri[0] - tri[1], tri[2] - tri[1]));
	glm::vec3 TriangToCenter = sphereCenter - tri[0];
	if (glm::dot(TriangToCenter, normal) < 0) {
		normal *= -1;
	}

	if (n) {
		*n = normal;
	}
	glm::vec2 bar;
	float dist;
	if (glm::intersectRayTriangle(sphereCenter, -normal, tri[0], tri[1], tri[2], bar, dist)) {
		if (dist <= 1) {
			return true;
		}
	}
	if (glm::length(closestPointOnTriangle(tri, sphereCenter)-sphereCenter) <= 1) {
		return true;
	}
	return false;

}
//
//bool Player::collided(const glm::vec3& sphereCenter, const glm::vec3& stride, glm::vec3* tri, glm::vec3* outSphereCenter)
//{
//	glm::vec3 normal = glm::normalize(glm::cross(tri[0] - tri[1], tri[2] - tri[1]));
//	glm::vec3 TriangToCenter = sphereCenter - tri[0];
//	if (glm::dot(TriangToCenter, normal) < 0) {
//		normal *= -1;
//	}
//	glm::vec3 sphereCollisionPoint; glm::vec3 triangleCollisionPoint;
//	bool sphereIntersectsPlane = false;
//
//	sphereCollisionPoint = sphereCenter - normal;
//	// check if sphere intersection point on other side
//	glm::vec3 triagToSI = sphereCollisionPoint - tri[0];
//	if (glm::dot(triagToSI, normal) < 0) {
//		sphereCollisionPoint = sphereCenter;
//		sphereIntersectsPlane = true;
//	}
//	glm::vec2 bar;
//	float dist;
//	bool col = glm::intersectRayPlane(sphereCollisionPoint, glm::normalize(stride), tri[0], normal, dist);
//	if (!sphereIntersectsPlane && (!col || dist > glm::length(stride) || dist < 0)) {
//		return false;
//	}
//
//	if (sphereIntersectsPlane) {
//		return false;
//		//std::cout << "b";
//		glm::vec3 planeCollisionPoint;
//		if (glm::intersectRayPlane(sphereCollisionPoint, -normal, tri[0], normal, dist)) {
//			planeCollisionPoint = sphereCollisionPoint + dist * normal;
//			if (dist >= 1) {
//				std::cout << dist << std::endl;
//			}
//		}
//		else {
//			assert(false);
//		}
//		if (glm::intersectRayTriangle(sphereCollisionPoint, normal, tri[0], tri[1], tri[2], bar, dist)) {
//			std::cout << "b2" << std::endl;
//			triangleCollisionPoint = planeCollisionPoint; // sanity check - this equals the bar coords
//		}
//		else {
//			return false;
//
//			//std::cout << 1 << std::endl;
//			glm::vec3 closestPoint = closestPointOnTriangle(tri, planeCollisionPoint);
//			float intersectDist;
//			if (glm::intersectRaySphere(closestPoint, -glm::normalize(stride), sphereCenter, 1.0f, intersectDist)) {
//				std::cout << "b1" << std::endl;
//				triangleCollisionPoint = closestPoint;
//				sphereCollisionPoint = closestPoint + -glm::normalize(stride) * intersectDist;
//			}
//			else {
//				return false;
//			}
//		}
//	}
//	else {
//		//std::cout << "a";
//		glm::vec3 planeCollisionPoint;
//		planeCollisionPoint = sphereCollisionPoint + dist * normal;
//
//		if (glm::intersectRayTriangle(sphereCollisionPoint, glm::normalize(stride), tri[0], tri[1], tri[2], bar, dist)) {
//			//std::cout << 2 << std::endl;
//			assert(dist < glm::length(stride));
//			std::cout << "a2" << std::endl;
//			triangleCollisionPoint = bar.x * tri[0] + bar.y * tri[1] + (1 - bar.x - bar.y) * tri[2];
//		}
//		else {
//			//std::cout << 1 << std::endl;
//			return false;
//			glm::vec3 closestPoint = closestPointOnTriangle(tri, planeCollisionPoint);
//			float intersectDist;
//			if (glm::intersectRaySphere(closestPoint, -glm::normalize(stride), sphereCenter, 1.0f, intersectDist)) {
//				std::cout << "a1" << std::endl;
//				triangleCollisionPoint = closestPoint;
//				sphereCollisionPoint = closestPoint + -glm::normalize(stride) * intersectDist;
//			}
//			else {
//				return false;
//			}
//		}
//	}
//	std::cout << "tri " << triangleCollisionPoint[0] << " " << triangleCollisionPoint[1] << " " << triangleCollisionPoint[2] << " sphere col " << (sphereCenter - sphereCollisionPoint)[0] << " " << (sphereCenter - sphereCollisionPoint)[1] << " " << (sphereCenter - sphereCollisionPoint)[2] << std::endl;
//	*outSphereCenter = triangleCollisionPoint + (sphereCenter - sphereCollisionPoint);
//	return true;
//}

void Player::UpdateBoundaries() {
	glm::vec3 lowPos = pos - glm::normalize(camera->getCameraUp()) * height;
	boundaries[0] = lowPos + (rightVector + frontVector) * width;
	boundaries[1] = lowPos + (rightVector - frontVector) * width;
	boundaries[2] = lowPos + (-rightVector - frontVector) * width;
	boundaries[3] = lowPos + (-rightVector + frontVector) * width;

	glm::vec3 highPos = pos + glm::normalize(camera->getCameraUp()) * height;
	boundaries[4] = highPos + (rightVector + frontVector) * width;
	boundaries[5] = highPos + (rightVector - frontVector) * width;
	boundaries[6] = highPos + (-rightVector - frontVector) * width;
	boundaries[7] = highPos + (-rightVector + frontVector) * width;
}

void Player::UpdatePos(float deltaTime)
{
	this->rightVector = glm::normalize(glm::cross(camera->getCameraFront(), camera->getCameraUp()));
	this->frontVector = -glm::normalize(glm::cross(rightVector, camera->getCameraUp()));

	if (this->mode == PlayerMode::WALKING) {
		float airFriction = this->activePlanet->getAirFriction();
		if (this->onGround) {
			airFriction += this->activePlanet->getGroundFriction();
		}
		if (glm::dot(velocity, rightVector) > 0 || glm::dot(velocity, rightVector) < 0) {
			this->velocity -= rightVector * glm::dot(velocity, rightVector) * deltaTime * airFriction;
		}
		if (glm::dot(velocity, rightVector) < 0) {
			this->velocity -= rightVector * glm::dot(velocity, rightVector) * deltaTime * airFriction;
		}
		if (glm::dot(velocity, frontVector) > 0) {
			this->velocity -= frontVector * glm::dot(velocity, frontVector) * deltaTime * airFriction;
		}
		if (glm::dot(velocity, frontVector) < 0) {
			this->velocity -= frontVector * glm::dot(velocity, frontVector) * deltaTime * airFriction;
		}
		glm::vec3 oldVel = this->velocity;
		this->velocity += -glm::normalize(this->camera->getCameraUp()) * deltaTime * this->activePlanet->getGravity() / mass;
		glm::vec3 oldPos = this->pos;

		// check collision
		glm::vec3 newPos;
		bool hasCollided = false;
		bool groundCollided = false;
		glm::vec3 stride = velocity * deltaTime;

		// new collision

		this->sphereToElipse[0] = width * this->frontVector;
		this->sphereToElipse[2] = width * this->rightVector;
		this->sphereToElipse[1] = height * this->camera->getCameraUp();
		// this->sphereToElipse = glm::transpose(this->sphereToElipse);
		this->elipseToSphere = glm::inverse(sphereToElipse);


		newPos = oldPos + stride;
		std::vector<glm::vec3> n;
		if (collided(newPos, n)) {
			// assert(!collided(oldPos, n));
			newPos = oldPos;
			hasCollided = true;
			for (glm::vec3 tmpN : n) {
				if (glm::length(tmpN - this->camera->getCameraUp()) < 0.2f) {
					// std::cout << " ground collided" << std::endl;
					groundCollided = true;
				}
			}
		}

		if (this->onGround) {
			if (!groundCollided) {
				this->onGround = false;
			}
		}

		for (glm::vec3 tmpN : n) {
			if (glm::length(tmpN - this->camera->getCameraUp()) < 0.2f) {
				this->onGround = true;
			}
			this->velocity -= glm::dot(velocity, tmpN) * tmpN;
		}

		stride = velocity * deltaTime;
		this->pos = oldPos + stride;



		//if (this->collided(stride, &newPos)) {
		//	this->pos = newPos;
		//	this->velocity = glm::vec3(0.0);
		//	// this->pos += stride;
		//	std::cout << "collided" << std::endl;
		//}
		//else {
		//	this->pos += stride;
		//}

	}



	
	//int collided = lowerBoundaryCollision(this->pos);
	//bool collision = collided != -1;

	//if (this->onGround) {
	//	if (!collision) {
	//		this->onGround = false;
	//	}
	//	else {
	//		this->velocity -= glm::dot(velocity, camera->getCameraUp()) * camera->getCameraUp();
	//		this->pos = oldPos;
	//		this->pos += velocity * deltaTime;
	//		collided = lowerBoundaryCollision(this->pos);
	//		collided = lowerBoundaryCollision(this->pos);
	//		collision = collided != -1;
	//	}
	//}

	//int sideCollision = -1;
	//if (collision) {
 //		if (this->onGround) {
	//		sideCollision = collided;
	//	}
	//	else {
	//		if (lowerBoundaryCollision(this->pos + camera->getCameraUp() * 0.05f) == -1) {
	//			this->onGround = true;
	//			this->velocity -= glm::dot(velocity, camera->getCameraUp()) * camera->getCameraUp();
	//			this->pos = oldPos;
	//			this->pos += velocity * deltaTime;
	//		}
	//		else {
	//			sideCollision = collided;
	//		}
	//	}
	//}
	//sideCollision = -1;
	//if (sideCollision != -1) {
	//	glm::vec3 direction;
 //		switch (sideCollision) {
	//	case 0:
	//		direction = glm::normalize(rightVector + frontVector);
	//		break;
	//	case 1:
	//		direction = glm::normalize(rightVector - frontVector);
	//		break;
	//	case 2:
	//		direction = glm::normalize(-rightVector - frontVector);
	//		break;
	//	case 3:
	//		direction = glm::normalize(-rightVector + frontVector);
	//		break;
	//	}
	//	this->velocity += glm::dot(velocity, direction) * direction;
	//	this->pos = oldPos;
	//	this->pos += velocity * deltaTime;
	//}

	UpdateBoundaries();
	UpdateCamera();
	updateVertexArray();
}

void Player::setActivePlanet(Planet* planet)
{
	activePlanet = planet;
	UpdateCameraUp();
}

Planet* Player::getActivePlanet() const
{
	return activePlanet;
}

void Player::setJumpForce(float jumpForce)
{
	this->jumpForce = jumpForce;

}

void Player::setMass(float mass)
{
	this->mass = mass;
}

void Player::setSpeed(float speed)
{
	this->speed = speed;
}

glm::vec3 Player::getVelocity() const
{
	return velocity;
}

void Player::setThirdPerson(bool thirdPerson)
{
	this->thirdPerson = thirdPerson;
}

bool Player::isThirdPerson()
{
	return thirdPerson;
}

float Player::getSpeed() const
{
	return speed;
}

float Player::getJumpForce() const
{
	return jumpForce;
}

bool Player::hasJetpack() const
{
	return jetpack;
}

void Player::setJetpack(bool b)
{
	jetpack = b;
}

float Player::getMass() const
{
	return mass;
}

bool Player::isOnGround() const
{
	return onGround;
}

PlayerMode Player::getMode() const
{
	return mode;
}

void Player::setMode(PlayerMode m)
{
	mode = m;
}

bool Player::isColliding(std::vector<glm::vec3>& n) const
{
	return collided(this->pos, n);
}
bool Player::isCollidingWithVoxel(Voxel v, std::vector<glm::vec3>& n) const
{
	std::vector<Vertex> va;
	glm::vec3 tri[3];
	activePlanet->renderVox(v, &va);
	glm::vec3 tmpN;
	bool collided = false;

	for (int i = 0; i < va.size(); i += 3) {
		tri[0] = elipseToSphere * va[i].position;
		tri[1] = elipseToSphere * va[i + 1].position;
		tri[2] = elipseToSphere * va[i + 2].position;
		if (this->triangleCollided(elipseToSphere * this->pos, tri, &tmpN)) {
			collided = true;
			n.push_back(glm::normalize(sphereToElipse * tmpN));
		}
	}
	return collided;
}

void Player::UpdateCameraUp()
{
	glm::vec3 up;
	if (mode == PlayerMode::WALKING) {
		up = glm::normalize(pos - activePlanet->getCenter());
	}
	else {
		up = glm::vec3(0, 1, 0);
	}
	camera->setCameraUp(up);
}

void Player::UpdateCameraPos()
{
	if (this->thirdPerson) {
		camera->setPosition(pos - camera->getCameraFront() * 5.0f);
	}
	else {
		camera->setPosition(pos + camera->getCameraUp() * height);
	}
}

void Player::UpdateCamera()
{
	UpdateCameraPos();
	UpdateCameraUp();
}


void Player::move(Direction d, float deltaTime)
{
	glm::vec3 directionVector;
	glm::vec3 rightVector, frontVector; 
	if (mode == PlayerMode::FLYING) {/*
		frontVector = camera->getCameraFront();
		rightVector = glm::normalize(glm::cross(camera->getCameraFront(), camera->getCameraUp()));*/
		this->move2(d, deltaTime);
		return;
	}
	else if (mode == PlayerMode::WALKING) {
		rightVector = glm::normalize(glm::cross(camera->getCameraFront(), camera->getCameraUp()));
		frontVector = -glm::normalize(glm::cross(rightVector, camera->getCameraUp()));
		float stride = this->speed * deltaTime;
		float jumpForce = this->jumpForce / 10;

		switch (d) {
		case Direction::JUMP:
			if (this->onGround) {
				jumpForce = this->jumpForce;
			}
			if (this->jetpack || this->onGround) {
				velocity += camera->getCameraUp() * deltaTime * jumpForce;
			}
			return;

		case Direction::FORWARD:
			directionVector = frontVector;
			break;
		case Direction::BACKWARD:
			directionVector = frontVector;
			stride = -stride;
			break;
		case Direction::RIGHT:
			directionVector = rightVector;
			break;
		case Direction::LEFT:
			directionVector = rightVector;
			stride = -stride;
			break;
		case Direction::UP:
			directionVector = camera->getCameraUp();
			break;
		case Direction::DOWN:
			directionVector = camera->getCameraFront();
			stride = -stride;
			break;
		}
		float oldVelSize = glm::dot(velocity, directionVector);
		velocity -= oldVelSize * directionVector;


		velocity += directionVector * stride;

		UpdateCamera();
	}

}

void Player::move2(Direction d, float deltaTime)
{
	glm::vec3 directionVector;
	float stride = deltaTime * this->speed;
	switch (d) {
	case Direction::FORWARD:
		directionVector = camera->getCameraFront();
		break;
	case Direction::BACKWARD:
		directionVector = camera->getCameraFront();
		stride = -stride;
		break;
	case Direction::RIGHT:
		directionVector = glm::normalize(glm::cross(camera->getCameraFront(), camera->getCameraUp()));
		break;
	case Direction::LEFT:
		directionVector = glm::normalize(glm::cross(camera->getCameraFront(), camera->getCameraUp()));
		stride = -stride;
		break;
	case Direction::UP:
		directionVector = camera->getCameraFront();
		break;
	case Direction::DOWN:
		directionVector = camera->getCameraUp();
		stride = -stride;
		break;
	default:
		return;
	}
	this->pos += directionVector * stride;
	UpdateBoundaries();
	UpdateCamera();
	updateVertexArray();
}

GLuint Player::getVAO()
{
	return vao;
}

GLuint Player::getVBO()
{
	return vbo;
}

GLsizei Player::getNumOfVertices() const
{
	return vertexArray.size();
}

void Player::updateVertexArray()
{
	vertexArray = std::vector<Vertex>();
	int b[12][3] = { {0,1,2}, {2,3,0},{4,5,6}, {6,7,4},
		{0,1,4}, {4,5,1}, {1,2,5}, {5,6,2}, {2,3,6}, {6,7,3}, {3,0,7}, {7,4,0} };
	Voxel temp;


	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 3; j++) {
			Vertex v;
			glm::vec3 pos = boundaries[b[i][j]];
			v.position = pos;
			if (activePlanet->CheckCollision(pos, temp)) {
				v.colorIndex = 4;
			}
			else {
				v.colorIndex = 4;
			}
			if (i > 4) {
				v.ambientOcclusion = 3;
			}
			else {
				v.ambientOcclusion = 2;
			}
			vertexArray.push_back(v);
		}
	}

	glBindVertexArray(getVAO());
	glBindBuffer(GL_ARRAY_BUFFER, getVBO());

	glBufferData(GL_ARRAY_BUFFER, getNumOfVertices() * sizeof(Vertex), &vertexArray[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribIPointer(1, 1, GL_UNSIGNED_SHORT, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribIPointer(2, 1, GL_UNSIGNED_SHORT, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat) + sizeof(GLint)));
	glEnableVertexAttribArray(2);

	// unbind to make sure other code does not change it somewhere else
	glBindVertexArray(0);
}

int Player::getActiveColor() const
{
	return activeColor;
}

void Player::setActiveColor(int color)
{
	this->activeColor = (color+16) % 16;
}
