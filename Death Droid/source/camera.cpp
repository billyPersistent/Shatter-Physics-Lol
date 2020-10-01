#include "camera.h"

#define STANDARD_FPS 60.0f

Camera::Camera(glm::vec3 pos, glm::vec2 rot, glm::mat4 proj)
{
	position = pos;
	rotation = rot;
	projection = proj;
}

Camera::Camera()
{
	position = glm::vec3(0,0,0);
	rotation = glm::vec2(0,0);
	projection = glm::mat4(1.0f);
}

//returns the camera's view matrix
glm::mat4 Camera::getViewM()
{
	return glm::translate(glm::mat4(1.0f), -1.0f * position);
}


//returns the camera's rotation matrix
glm::mat4 Camera::getRotationM()
{
	glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f * rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f * rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));

	return rotX * rotY;
}

//rotates the camera around the x and y axis (in that order)
void Camera::rotate(glm::vec2 rot, float frameDuration)
{
	rotation += rot;// * (frameDuration * STANDARD_FPS);
}

//changes the camera's position by distance
void Camera::move(glm::vec3 dist, float frameDuration)
{
	position += dist * (frameDuration * STANDARD_FPS);
}

void Camera::setPos(glm::vec3 pos)
{
	position = pos;
}

glm::vec3 Camera::getPos()
{
	return position;
}

glm::vec2 Camera::getRotation()
{
	return rotation;
}

//returns the camera's full transformation matrix
float* Camera::getMatrix()
{
	MVP = (projection * getRotationM()) * getViewM();
	return &MVP[0][0];
}