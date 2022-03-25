#pragma once
#include <glm/glm.hpp>

struct view_volume {
	float l = -1.0f;
	float r = 1.0f;
	float b = -1.0f;
	float t = 1.0f;
	float n = 0.5f;
	float f = 10.0f;
};


class Camera
{
public:
	Camera();
	virtual ~Camera();
	void SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);
	void SetViewVolume(float left, float right, float bottom, float top, float n, float f);

	const view_volume& GetViewVolume();
	glm::mat4x4 GetInverseTransformation() const;
	bool IsOrthographic() const;
	void SetOrthograpic(bool isOrthographic);

	glm::vec3 translation;
	glm::vec3 rotation;

	void UpdateProjectionTrans();
	void UpdateViewTrans();
	const glm::mat4x4& GetProjectionTransformation() const;
	const glm::mat4x4& GetViewTransformation() const;

private:
	bool orthographic;

	glm::mat4x4 view_transformation;
	glm::mat4x4 projection_transformation;
	view_volume view_vol;
};
