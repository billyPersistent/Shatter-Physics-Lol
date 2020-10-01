#pragma once

#include <vector>
#include <string.h>
#include <fstream>

#include "vertex array.h"
#include "glm.hpp"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
};

class Mesh
{
private:
	std::vector<Vertex> m_Mesh;

	unsigned int m_VArray;

public:
	Mesh() = default;

	unsigned int getVArray();
	unsigned int getVertexCount();

	void loadSTL(std::string path);
};

struct MeshComponent
{
	Mesh c;
};