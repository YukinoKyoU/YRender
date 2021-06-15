
#include "Camera.h"

Camera::Camera(
	glm::vec3 _position,
	glm::vec3 _up,
	glm::vec3 lookat,
	float _fov,
	int _w,
	int _h) : Position(_position), WorldUp(_up), FOV(glm::radians(_fov)), Aspect((float)_w / _h), Pitch(0), Yaw(0)
{
	Front = glm::normalize(lookat - Position);
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));

	glm::vec3 WorldFront(0, 0, -1);
	glm::vec3 FrontXZ = glm::normalize(glm::vec3(Front.x, 0, Front.z));
	float yd = glm::dot(WorldFront, FrontXZ);
	float pd = glm::dot(Front, FrontXZ);
	if (yd > 1.0)
		yd = 1.0;
	if (yd < -1)
		yd = -1.0;
	if (pd > 1.0)
		pd = 1.0;
	if (pd < -1)
		pd = -1.0;
	Yaw = glm::degrees(acos(yd));
	Pitch = glm::degrees(acos(pd));
}
/*
glm::mat4 Camera::SetViewMatrix()
{		
	return GetViewMatrix(Position, Front, Right, Up);
}

glm::mat4 Camera::SetPerspectiveMatrix()
{
	return GetPerspectiveMatrix(FOV, Aspect, 0.3f, 100);
}
*/
void Camera::UpdateFov(float fov) 
{
		FOV = glm::radians(fov);
}

void Camera::UpdateAspect(int w, int h) 
{
	Aspect = (float)w / h;
}
void Camera::MoveForward(float distance) 
{
	Position += Front * distance;
}
void Camera::MoveRight(float distance) 
{
	Position += Right * distance;
}
void Camera::MoveUp(float distance) 
{
	Position += Up * distance;
}
void Camera::RotatePitch(float angle) 
{
	Pitch += angle;
	if (Pitch > 89.0)
		Pitch = 89.0;
	if (Pitch < -89.0)
		Pitch = -89.0;
	UpdateCameraVectors();
}
void Camera::RotateYaw(float angle) 
{
	Yaw += angle;
	if (Yaw > 360)
		Yaw = 0;
	if (Yaw < 0)
		Yaw = 360;
	UpdateCameraVectors();
}
void Camera::UpdateCameraVectors()
{
	glm::vec3 front;
	front.x = -sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = -cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));
}


