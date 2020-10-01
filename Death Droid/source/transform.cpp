#include "transform.h"

#define STANDARD_FPS 60.0f

Transform::Transform(glm::vec3 pos, glm::vec3 rot)
	:m_Position(pos), m_Rotation(rot)
{
	
}

void Transform::rotate(glm::vec3 rot, float frameDuration)
{
	m_Rotation += rot * (frameDuration * STANDARD_FPS);
}

void Transform::setRotation(glm::vec3 rot)
{
	m_Rotation = rot;
}

void Transform::move(glm::vec3 dist, float frameDuration)
{
	m_Position += dist * (frameDuration * STANDARD_FPS);
}

void Transform::setPos(glm::vec3 pos)
{
	m_Position = pos;
}

glm::vec3 Transform::getPos()
{
	return m_Position;
}

glm::vec3 Transform::getRotation()
{
	return m_Rotation;
}

float* Transform::getTranslationM()
{
	m_TranslationMat = glm::translate(glm::mat4(1.0f), m_Position);
	return &m_TranslationMat[0][0];
}

float* Transform::getRotationM()
{
	glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	m_RotationMat = rotZ * rotX * rotY;
	return &m_RotationMat[0][0];
}

glm::mat4 Transform::getTranslationMat4()
{
	m_TranslationMat = glm::translate(glm::mat4(1.0f), m_Position);
	return m_TranslationMat;
}

glm::mat4 Transform::getRotationMat4()
{
	glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	m_RotationMat = rotZ * rotX * rotY;

	return m_RotationMat;
}

glm::mat4 Transform::getTransform()
{
	getTranslationM();
	getRotationM();

	return m_TranslationMat * m_RotationMat;//reverse?
}

TransformComponent::TransformComponent(glm::vec3 pos, glm::vec3 rot)
	:c(pos, rot)
{

}