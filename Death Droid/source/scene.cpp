#include "scene.h"

Scene::Scene(glm::mat4& projection, int winWidth, int winHeight)
	:m_Renderer(&m_Camera, winWidth, winHeight), m_SuckyDickey(true), m_Normal(0.0f, -1.0f, 0.0f), m_Angle(3.141592f), m_MouseDownLast(false)
{
	//set background color
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);

	//intialize camera
	m_Camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), projection);

	//create player
	m_Player = m_Registry.create();

	//log first mouse posiiton
	m_FirstMousePos = getMousePos();

	//magical sandbox scope
	{
		{
			entt::entity entity = m_Registry.create();

			auto& mesh = m_Registry.emplace<MeshComponent>(entity);
			mesh.c.loadSTL("doe.stl");

			auto& transform = m_Registry.emplace<TransformComponent>(entity, glm::vec3(0.0f, -100.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		}

		/*{
			entt::entity entity = m_Registry.create();

			auto& sMesh = m_Registry.emplace<ShatterMeshComponent>(entity);
			sMesh.c.loadSTL("beans.stl");//torus shouldnt work

			auto& transform = m_Registry.emplace<TransformComponent>(entity, glm::vec3(0.0f, -100.0f, 20.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		}*/

		{
			entt::entity entity = m_Registry.create();

			auto& sMesh = m_Registry.emplace<ShatterMeshComponent>(entity);
			sMesh.c.loadSTL("beans.stl");//torus shouldnt work
			//sMesh.c.bodyTruncate(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1.0f, 0.82f, 0.0f));

			auto& transform = m_Registry.emplace<TransformComponent>(entity, glm::vec3(50.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		}
	}
}

void Scene::update(float timeStep)
{
	//camera stuff

	float mouseDrag = 10.0f;
	if (!keyDown(key::LCTRL))
	{
		m_DifMousePos = m_FirstMousePos - getMousePos();

		m_Camera.rotate(glm::vec2(0.0f, m_DifMousePos.x / mouseDrag), timeStep);

		if (m_Camera.getRotation().x + m_DifMousePos.y / mouseDrag > -90.0f and
			m_Camera.getRotation().x + m_DifMousePos.y / mouseDrag < 90.0f)
		{
			m_Camera.rotate(glm::vec2(m_DifMousePos.y / mouseDrag, 0.0f), timeStep);
		}

		setMousePos(m_FirstMousePos);
	}

	float distance = 1.0f;
	glm::vec3 movement = glm::vec3(distance * -sin(glm::radians(m_Camera.getRotation().y)), 0.0f, distance * -cos(glm::radians(m_Camera.getRotation().y)));
	glm::vec3 movementS = glm::vec3(distance * -sin(glm::radians(m_Camera.getRotation().y + 90.0f)), 0.0f, distance * -cos(glm::radians(m_Camera.getRotation().y + 90.0f)));

	if (keyDown(key::W) and !keyDown(key::S))
	{
		m_Camera.move(movement, timeStep);
	}
	if (keyDown(key::S) and !keyDown(key::W))
	{
		m_Camera.move(-movement, timeStep);
	}
	if (keyDown(key::A) and !keyDown(key::D))
	{
		m_Camera.move(movementS, timeStep);
	}
	if (keyDown(key::D) and !keyDown(key::A))
	{
		m_Camera.move(-movementS, timeStep);
	}
	if (keyDown(key::SPACE))
	{
		m_Camera.move(glm::vec3(0.0f, 1.0f, 0.0f), timeStep);
	}
	if (keyDown(key::LSHIFT))
	{
		m_Camera.move(glm::vec3(0.0f, -1.0f, 0.0f), timeStep);
	}

	//meshes
	{
		auto view = m_Registry.view<MeshComponent>();
		for (auto entity : view)
		{
			auto& tc = m_Registry.get<TransformComponent>(entity);
			tc.c.rotate(glm::vec3(0.0f, 3.0f, 0.0f), timeStep);

			m_Renderer.render(m_Registry.get<MeshComponent>(entity).c, tc.c);
		}
	}

	if (leftMouseDown() and !m_MouseDownLast)//mouse pressed
	{
		m_CamVec1 = glm::inverse(m_Camera.getRotationM()) * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
		m_MouseDownLast = true;
		std::cout << "click" << std::endl;
	}
	else if (!leftMouseDown() and m_MouseDownLast)//mouse released
	{
		std::cout << "clock" << std::endl;
		m_CamVec2 = glm::inverse(m_Camera.getRotationM()) * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
		m_MouseDownLast = false;

		auto view = m_Registry.view<ShatterMeshComponent>();
		for (auto entity : view)
		{
			auto& tc = m_Registry.get<TransformComponent>(entity);
			auto& smc = m_Registry.get<ShatterMeshComponent>(entity);

			smc.c.worldTruncate(m_Camera.getPos(), glm::cross(m_CamVec1, m_CamVec2), glm::vec3(1.0f, 0.0f, 0.0f), tc.c);
			//smc.c.worldTruncate(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.82f, 0.0f), tc.c);
		}
	}

	//shatter meshes
	auto view = m_Registry.view<ShatterMeshComponent>();
	for (auto entity : view)
	{
		auto& tc = m_Registry.get<TransformComponent>(entity);
		auto& smc = m_Registry.get<ShatterMeshComponent>(entity);
		tc.c.rotate(glm::vec3(1.0f, 3.0f, 2.0f), timeStep);
		//tc.c.move((m_Camera.getPos() - tc.c.getPos()) * glm::vec3(0.01f, 0.01f, 0.01f));
		//tc.c.move(glm::vec3(1.0f, 0.0f, 0.0f), timeStep);

		for (int i = 0; i < 1; i++)
		{
			m_Renderer.render(smc.c, tc.c);
		}

		if (keyDown(key::ENTER) and m_SuckyDickey)
		{
			smc.c.bodyTruncate(glm::vec3(9.45, 1.69, 0.27), glm::vec3(0.11, -0.49, 0.06), glm::vec3(1.0f, 0.82f, 0.0f));
			m_SuckyDickey = false;
		}
	}

	//std::cout << glGetError() << std::endl;
}