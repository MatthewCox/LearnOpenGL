#include "camera.h"

#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 & position, glm::vec3 & up, float yaw, float pitch)
	: position(position)
	, worldUp(up)
	, yaw(yaw)
	, pitch(pitch)
	, front(glm::vec3(0.0f, 0.0f, -1.0f))
	, movementSpeed(SPEED)
	, sensitivity(SENSITIVITY)
{
	updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime)
{
	float velocity = movementSpeed * deltaTime;
	switch (direction)
	{
	case FORWARD:
		position += front * velocity;
		break;
	case BACKWARD:
		position -= front * velocity;
		break;
	case LEFT:
		position -= right * velocity;
		break;
	case RIGHT:
		position += right * velocity;
		break;
	}
}

void Camera::processMouseMovement(float xOffset, float yOffset, bool constrainPitch)
{
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	yaw += xOffset;
	pitch += yOffset;

	if (constrainPitch)
		pitch = std::clamp(pitch, -89.0f, 89.0f);

	updateCameraVectors();
}

void Camera::updateCameraVectors()
{
	glm::vec3 newFront;
	newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	newFront.y = sin(glm::radians(pitch));
	newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(newFront);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}
