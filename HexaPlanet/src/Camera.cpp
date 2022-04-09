#include "Camera.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <GLFW\glfw3.h>

#include <iostream>

Camera::Camera() : projection_transformation(glm::mat4x4(1.0f)), view_transformation(glm::mat4x4(1.0f))
{
	aspectRatio = 1;
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	orthographic = false;
	translation = glm::vec3(0, 0, 1);
	yaw = 0;
	pitch = 0;
	setFov(40);

	UpdateCameraFront();
	UpdateProjectionTrans();
	UpdateViewTrans();
}

Camera::~Camera()
{
	
}

void Camera::SetViewVolume(float near, float far)
{
	if (!orthographic) {
		float halfLength = tan(glm::radians(fov)) * near;
		SetViewVolume(-halfLength, halfLength, -halfLength * aspectRatio, halfLength * aspectRatio, near, far);
	}
	else {
		std::cout << "Not Implemented" << std::endl;
	}

}



void Camera::SetViewVolume(float left, float right, float bottom, float top, float n, float f)
{
	view_vol.l = left;
	view_vol.r = right;
	view_vol.b = bottom;
	view_vol.t = top;
	view_vol.n = n;
	view_vol.f = f;
	UpdateProjectionTrans();
}


const view_volume& Camera::GetViewVolume()
{
	return view_vol;
}

glm::mat4x4 Camera::GetInverseTransformation() const
{
	return (projection_transformation * view_transformation);
}

bool Camera::IsOrthographic() const
{
	return orthographic;
}

void Camera::SetOrthograpic(bool isOrthographic) {
	orthographic = isOrthographic;
	UpdateProjectionTrans();
}

void Camera::move(Direction d, float stride)
{
	glm::vec3 directionVector;
	switch (d) {
	case Direction::FORWARD:
		directionVector = cameraFront;
		break;
	case Direction::BACKWARD:
		directionVector = cameraFront;
		stride = -stride;
		break;
	case Direction::RIGHT:
		directionVector = glm::normalize(glm::cross(cameraFront, cameraUp));
		break;
	case Direction::LEFT:
		directionVector = glm::normalize(glm::cross(cameraFront, cameraUp));
		stride = -stride;
		break;
	case Direction::UP:
		directionVector = cameraUp;
		break;
	case Direction::DOWN:
		directionVector = cameraFront;
		stride = -stride;
		break;
	}
	this->translation += directionVector * stride;
	UpdateViewTrans();
}

float Camera::getYaw() const
{

	return yaw;
}

float Camera::getPitch() const
{
	return pitch;
}

void Camera::setYaw(float yaw)
{
	this->yaw = yaw;
	UpdateCameraFront();
}

void Camera::setPitch(float pitch)
{
	if (pitch > 89.0f) {
		this->pitch = 89.0f;
	}
	else if (pitch < -89.0f) {
		this->pitch = -89.0f;
	}
	else {
		this->pitch = pitch;
	}
	UpdateCameraFront();
}

float Camera::getAspectRatio() const
{
	return aspectRatio;
}

void Camera::setFov(float fov)
{
	if (fov < 1.0f)
		this->fov = 1.0f;
	else if (fov > 60.0f)
		this->fov = 60.0f;
	else
		this->fov = fov;
	SetViewVolume(0.1, 100);
}

void Camera::setAspectRatio(float aspectRatio)
{
	this->aspectRatio = aspectRatio;
	SetViewVolume(0.1, 100);

}

float Camera::getFov() const
{
	return fov;
}

const glm::mat4x4& Camera::GetProjectionTransformation() const
{
	return projection_transformation;
}

const glm::mat4x4& Camera::GetViewTransformation() const
{
	return view_transformation;
}

void Camera::UpdateProjectionTrans()
{
	view_volume v = view_vol;
	if (orthographic) {
		glm::vec3 translate = glm::vec3(-(v.r + v.l) / 2, -(v.b + v.t) / 2, (v.n + v.f) / 2);
		glm::vec3 scale = glm::vec3(2 / (v.r - v.l), 2 / (v.t - v.b), 2 / (v.n - v.f));
		glm::mat4x4 T = glm::translate(translate);
		glm::mat4x4 S = glm::scale(scale);
		projection_transformation = S * T;
	}
	else {
		float P[16] =  {2*v.n/(v.r-v.l),0,0,0, 0,2*v.n/(v.t-v.b),0,0,(v.l+v.r)/(v.r-v.l), (v.t+v.b)/(v.t-v.b), -(v.f+v.n)/(v.f-v.n), -1, 0,0,-2*v.f*v.n/(v.f-v.n),0};
		projection_transformation = glm::make_mat4(P);
	}
}

void Camera::UpdateViewTrans()
{
	view_transformation = glm::lookAt(translation, translation + cameraFront, cameraUp);
}

void Camera::UpdateCameraFront()
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	this->cameraFront = glm::normalize(direction);
	UpdateViewTrans();
}
