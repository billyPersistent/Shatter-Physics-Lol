#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

#include "iostream"

class Transform
{
private:
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;

	glm::mat4 m_TranslationMat;
	glm::mat4 m_RotationMat;
public:
	Transform(glm::vec3 pos, glm::vec3 rot);

	void setPos(glm::vec3 pos);
	void setRotation(glm::vec3 rot);

	void move(glm::vec3 dist, float frameDuration);
	void rotate(glm::vec3 rot, float frameDuration);

	glm::vec3 getPos();
	glm::vec3 getRotation();

	float* getTranslationM();
	float* getRotationM();

	glm::mat4 getTranslationMat4();
	glm::mat4 getRotationMat4();
	glm::mat4 getTransform();
};

struct TransformComponent
{
	Transform c;

	TransformComponent() = default;
	TransformComponent(glm::vec3 pos, glm::vec3 rot);
};