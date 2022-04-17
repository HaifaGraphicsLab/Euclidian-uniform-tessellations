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

enum Direction {
	UP, DOWN, FORWARD, BACKWARD, LEFT, RIGHT
};


class Camera
{
public:
	Camera();
	virtual ~Camera();
	void SetViewVolume(float left, float right);
	void SetViewVolume(float left, float right, float bottom, float top, float n, float f);

	const view_volume& GetViewVolume();
	glm::mat4x4 GetInverseTransformation() const;
	bool IsOrthographic() const;
	void SetOrthograpic(bool isOrthographic);
	void move(Direction d, float stride);

	float getYaw() const;
	float getPitch() const;
	void setYaw(float yaw);
	void setPitch(float pitch);
	float getAspectRatio() const;
	void setFov(float fov);
	void setAspectRatio(float aspectRatio);
	float getFov() const;

	const glm::mat4x4& GetProjectionTransformation() const;
	const glm::mat4x4& GetViewTransformation() const;

private:

	void UpdateProjectionTrans();
	void UpdateViewTrans();
	void UpdateCameraFront();
	float yaw;
	float pitch;
	float renderDistance;

	bool orthographic;
	glm::vec3 translation;

	glm::mat4x4 view_transformation;
	glm::mat4x4 projection_transformation;
	view_volume view_vol;

	glm::vec3 cameraFront;
	glm::vec3 cameraUp;

	float fov;
	float aspectRatio;
};
