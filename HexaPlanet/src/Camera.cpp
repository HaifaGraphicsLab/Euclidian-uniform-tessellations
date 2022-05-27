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
	translation = glm::vec3(0, 0, 0);
	yaw = 0;
	pitch = 0;
	renderDistance = 400.0f;
	m_pole = glm::vec3(0, 0, 1);
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

glm::vec3 Camera::GetTranslation() const {
	return translation;
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
	UpdateCameraFront();
	// UpdateViewTrans();
}

float Camera::getYaw() const
{
	return yaw;
}

float Camera::getPitch() const
{
	return pitch;
}

void Camera::setPosition(glm::vec3 pos)
{
	this->translation = pos;
	UpdateViewTrans();
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
	SetViewVolume(0.1, renderDistance);
}

void Camera::setCameraUp(glm::vec3 up)
{
	cameraUp = up;
	UpdateCameraFront();
}
glm::vec3 Camera::getCameraUp() const
{
	return cameraUp;
}

glm::vec3 Camera::getPole() const
{
	return m_pole;
}

void Camera::setCameraFront(glm::vec3 front)
{
	cameraFront = front;
	UpdateViewTrans();
}
glm::vec3 Camera::getCameraFront() const
{
	return cameraFront;
}
void Camera::setAspectRatio(float aspectRatio)
{
	this->aspectRatio = aspectRatio;
	SetViewVolume(0.1, renderDistance);

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


	glm::quat world_axes_rotation = glm::angleAxis(-yaw/180.0f*3.1415926f, glm::vec3(0.0f, 1.0f, 0.0f));
	world_axes_rotation = glm::normalize(world_axes_rotation);
	world_axes_rotation = glm::rotate(world_axes_rotation, pitch / 180.0f * 3.1415926f, glm::vec3(1.0f, 0.0f, 0.0f));

	glm::vec3 new_pole = (m_pole - glm::dot(cameraUp, m_pole) * cameraUp);
	if (glm::length(new_pole) != 0)
		m_pole = glm::normalize(new_pole);

	glm::mat4 local_transform;

	local_transform[0] = glm::vec4(m_pole.x, m_pole.y, m_pole.z, 0.0f);
	local_transform[1] = glm::vec4(cameraUp.x, cameraUp.y, cameraUp.z, 0.0f);
	glm::vec3 tmp = glm::cross(m_pole, cameraUp);
	local_transform[2] = glm::vec4(tmp.x, tmp.y, tmp.z, 0.0f);
	local_transform[3] = glm::vec4(translation.x, translation.y, translation.z, 1.0f);

	world_axes_rotation = glm::normalize(world_axes_rotation);
	glm::mat4 view = local_transform * glm::mat4_cast(world_axes_rotation);
	direction = -1.0f * glm::vec3(view[2]);
	//m_up = glm::vec3(m_view[1]);
	//m_right = glm::vec3(m_view[0]);

	//m_view = glm::inverse(m_view);




	//glm::vec3 a = glm::vec3(0, 1, 0);
	//glm::vec3 b = cameraUp;

	//glm::vec3 v = glm::cross(b, a);
	//float angle = acos(glm::dot(b, a) / (glm::length(b) * glm::length(a)));
	//glm::mat4 rotmat = glm::rotate(angle, v);


	//direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	//direction.y = sin(glm::radians(pitch));
	//direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	
	//if (angle != 0)
	//	direction = glm::vec4(direction, 0) * rotmat;

	//glm::mat4 r = glm::eulerAngleYXZ(-yaw/180.0f*3.14f, pitch / 180.0f * 3.14f, 0.0f);
	//direction = glm::vec3(r * glm::vec4(cameraUp, 0));
	
	this->cameraFront = glm::normalize(direction);
	UpdateViewTrans();
}
