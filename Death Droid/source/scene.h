#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

#include "render.h"
#include "entt.hpp"
#include "input.h"

class Scene
{
private:
	Camera m_Camera;
	entt::entity m_Player;

	Renderer m_Renderer;
	entt::registry m_Registry;

	glm::vec2 m_FirstMousePos;
	glm::vec2 m_DifMousePos;
	bool m_MouseDownLast;

	glm::vec3 m_CamVec1;
	glm::vec3 m_CamVec2;

	bool m_SuckyDickey;
	glm::vec3 m_Normal;
	float m_Angle;
public:
	Scene(glm::mat4& projection, int winWidth, int winHeight);

	void update(float timeStep);
};