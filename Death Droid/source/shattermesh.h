#pragma once

#include <iostream>

#include "mesh.h"
#include "transform.h"
#include "collide.h"

#include "gtx/quaternion.hpp"

class ShatterMesh
{
private:
	std::vector<Vertex> m_Mesh;

	AABB m_BoundingBox;
	glm::vec3 m_BoundingCenter;
	float m_CornerDistance;

	unsigned int m_VArray;
	unsigned int m_VBuffer;
	unsigned int m_TBuffer;

	void setBuffers();

	void addVertex(Vertex vertex);
	void addDuplicateVertex(Vertex vertex);

public:
	float resolutionDivide;

	ShatterMesh();

	unsigned int getVArray();
	unsigned int getVBuffer();
	unsigned int getTBuffer();

	unsigned int getVertexCount();

	void loadSTL(std::string path);

	const glm::vec3& getPosition(int triangleIndex);
	const glm::vec3& getNormal(int triangleIndex);

	void bodyTruncate(glm::vec3 position, glm::vec3 normal, glm::vec3 color);
	void worldTruncate(glm::vec3 position, glm::vec3 normal, glm::vec3 color, Transform transform);
};

struct ShatterMeshComponent
{
	ShatterMesh c;
};