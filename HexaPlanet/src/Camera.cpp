#include "Camera.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <GLFW\glfw3.h>



Camera::Camera() : projection_transformation(glm::mat4x4(1.0f)), view_transformation(glm::mat4x4(1.0f))
{
	orthographic = false;
	translation = glm::vec3(0, 0, 1);
	rotation = glm::vec3(0);

	UpdateProjectionTrans();
	UpdateViewTrans();
}

Camera::~Camera()
{
	
}

void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{
	glm::vec3 zaxis = normalize(at - eye);
	glm::vec3 xaxis = normalize(cross(zaxis, up));
	glm::vec3 yaxis = cross(xaxis, zaxis);

	zaxis = zaxis * -1.0f;
	
	glm::mat4x4 inverse = {
		glm::vec4(xaxis, -glm::dot(xaxis,eye)),
		glm::vec4(xaxis, -glm::dot(yaxis,eye)),
		glm::vec4(xaxis, -glm::dot(zaxis,eye)),
		glm::vec4(0,0,0,1),
	};
	view_transformation = inverse;
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
	view_transformation = glm::inverse(glm::eulerAngleYXZ(rotation[1], rotation[0], rotation[2]) * glm::translate(translation));
}
