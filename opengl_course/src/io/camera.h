#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Pass in direction to camera.
// Used to direct the movement of camera.
enum class CameraDirection
{
	NONE = 0,
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};


class Camera
{
public:

	static Camera defaultCamera;
	static Camera secondary;
	static bool usingDefault;

	glm::vec3 cameraPos;

	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::vec3 cameraRight;

	glm::vec3 worldUp;

	float yaw; // x-axis.
	float pitch; // y-axis.
	float speed;
	float sensitivity;
	float zoom;

	
	Camera(glm::vec3 position);

	void updateCameraDirection(double dx, double dy); // Moving Mouse.
	void updateCameraPos(CameraDirection dir, double dt); // Moving Keyboard.
	void updateCameraZoom(double dy); // scroll wheel.

	glm::mat4 getViewMatrix();

private:

	void updateCameraVectors();

};
