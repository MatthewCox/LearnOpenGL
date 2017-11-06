#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <glm/glm.hpp>

enum CameraMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.05f;

class Camera
{
private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw;
	float pitch;

	float movementSpeed;
	float sensitivity;

public:
	Camera(glm::vec3 & position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 & up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	glm::mat4 getViewMatrix() const;
	const glm::vec3 & getPosition() const;
	void processKeyboard(CameraMovement direction, float deltaTime);
	void processMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);

private:
	void updateCameraVectors();
};

#endif // !__CAMERA_H__
