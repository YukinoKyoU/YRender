#ifndef CAMERA_H_
#define CAMERA_H_

#include "Math.h"

class Camera
{
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	float FOV;
	float Aspect;

	float Pitch;
	float Yaw;

	Camera(
		glm::vec3 _position = glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3 lookat = glm::vec3(0.0f, 0.0f, 0.0f),
		float _fov = 60.0f,
		int _w = 800,
		int _h = 600);


	void UpdateFov(float fov = 60.0f);
	void UpdateAspect(int w, int h);
	void MoveForward(float distance);
	void MoveRight(float distance);
	void MoveUp(float distance);
	void RotatePitch(float angle);
	void RotateYaw(float angle);

private:
	void UpdateCameraVectors();
};

#endif // !CAMERA_H_
