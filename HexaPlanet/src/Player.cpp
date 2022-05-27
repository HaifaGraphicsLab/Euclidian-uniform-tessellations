#include "Player.h"

Player::Player(Camera* camera, Planet* activePlanet, float jumpForce, float mass, glm::vec3 pos)
{
	this->activePlanet = activePlanet;
	this->camera = camera;
	this->jumpForce = jumpForce;
	this->mass = mass;
	this->pos = pos;
	this->height = 0.6;
	this->width = 0.2;
	this->mode = PlayerMode::WALKING;
	this->velocity = glm::vec3(0);
	speed = 40;
	UpdateBoundaries();
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	updateVertexArray();
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

int Player::lowerBoundaryCollision(const glm::vec3& pos) {
	glm::vec3 boundary[4];
	glm::vec3 lowPos = pos - glm::normalize(camera->getCameraUp()) * height / 2.0f;
	boundary[0] = lowPos + (rightVector + frontVector) * width;
	boundary[1] = lowPos + (rightVector - frontVector) * width;
	boundary[2] = lowPos + (-rightVector + frontVector) * width;
	boundary[3] = lowPos + (-rightVector - frontVector) * width;
	return boundaryCollision(boundary, 4);
}

void Player::UpdateBoundaries() {
	glm::vec3 lowPos = pos - glm::normalize(camera->getCameraUp()) * height / 2.0f;
	boundaries[0] = lowPos + (rightVector + frontVector) * width;
	boundaries[1] = lowPos + (rightVector - frontVector) * width;
	boundaries[2] = lowPos + (-rightVector - frontVector) * width;
	boundaries[3] = lowPos + (-rightVector + frontVector) * width;

	glm::vec3 highPos = pos + glm::normalize(camera->getCameraUp()) * height / 2.0f;
	boundaries[4] = highPos + (rightVector + frontVector) * width;
	boundaries[5] = highPos + (rightVector - frontVector) * width;
	boundaries[6] = highPos + (-rightVector - frontVector) * width;
	boundaries[7] = highPos + (-rightVector + frontVector) * width;
}

void Player::UpdatePos(float deltaTime)
{
	float airFriction = 5.0f;
	this->rightVector = glm::normalize(glm::cross(camera->getCameraFront(), camera->getCameraUp()));
	this->frontVector = -glm::normalize(glm::cross(rightVector, camera->getCameraUp()));


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

	this->velocity += (this->activePlanet->getCenter() - this->pos) * deltaTime / mass;

	glm::vec3 oldPos = this->pos;
	this->pos += velocity * deltaTime;
	
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

float Player::getSpeed() const
{
	return speed;
}

float Player::getJumpForce() const
{
	return jumpForce;
}

float Player::getMass() const
{
	return mass;
}

bool Player::isOnGround() const
{
	return onGround;
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
	// camera->setPosition(pos + camera->getCameraUp()*height);
	camera->setPosition(pos- camera->getCameraFront() * 5.0f);

}

void Player::UpdateCamera()
{
	UpdateCameraPos();
	UpdateCameraUp();
}


void Player::move(Direction d, float stride)
{
	glm::vec3 directionVector;
	glm::vec3 rightVector, frontVector; 
	if (mode == PlayerMode::FLYING) {
		frontVector = camera->getCameraFront();
		rightVector = glm::normalize(glm::cross(camera->getCameraFront(), camera->getCameraUp()));
	}
	else if (mode == PlayerMode::WALKING) {
		rightVector = glm::normalize(glm::cross(camera->getCameraFront(), camera->getCameraUp()));
		frontVector = -glm::normalize(glm::cross(rightVector, camera->getCameraUp()));
	}

	switch (d) {
	case Direction::JUMP:
		velocity += camera->getCameraUp() * stride;
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

	velocity = directionVector * stride + glm::dot(velocity, glm::cross(frontVector, -rightVector)) * glm::cross(frontVector, -rightVector);
	//switch (d) {
	//case Direction::FORWARD:
	//	directionVector = frontVector;
	//	break;
	//case Direction::BACKWARD:
	//	directionVector = frontVector;
	//	stride = -stride;
	//	break;
	//case Direction::RIGHT:
	//	directionVector = rightVector;
	//	break;
	//case Direction::LEFT:
	//	directionVector = rightVector;
	//	stride = -stride;
	//	break;
	//case Direction::UP:
	//	directionVector = camera->getCameraUp();
	//	break;
	//case Direction::DOWN:
	//	directionVector = camera->getCameraFront();
	//	stride = -stride;
	//	break;
	//}
	//this->pos += directionVector * stride;
	UpdateCamera();
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
				v.colorIndex = 1;

			}
			else {
				v.colorIndex = 0;
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
