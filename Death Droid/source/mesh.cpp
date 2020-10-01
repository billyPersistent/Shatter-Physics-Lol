#include "mesh.h"

unsigned int Mesh::getVArray()
{
	return m_VArray;
}

unsigned int Mesh::getVertexCount()
{
	return m_Mesh.size();
}

void Mesh::loadSTL(std::string path)
{
	//empty mesh
	m_Mesh.clear();

	//open file
	std::fstream file(path, std::ios::in | std::ios::binary);

	//start past 80 byte header
	file.seekg(80);

	//read triangle count
	unsigned int triangeCount;
	file.read((char*)&triangeCount, 4);
	m_Mesh.resize(triangeCount * 3);

	//populate mesh
	for (unsigned int i = 0; i < triangeCount; i++)
	{
		//set colors
		m_Mesh[i * 3].color = glm::vec3(1.0f, 0.82f, 0.0f);
		m_Mesh[i * 3 + 1].color = glm::vec3(1.0f, 0.82f, 0.0f);
		m_Mesh[i * 3 + 2].color = glm::vec3(1.0f, 0.82f, 0.0f);

		//set normal
		glm::vec3 normal;
		file.read((char*)&normal, 12);

		m_Mesh[i * 3].normal = normal;
		m_Mesh[i * 3 + 1].normal = normal;
		m_Mesh[i * 3 + 2].normal = normal;

		//set positions
		file.read((char*)&m_Mesh[i * 3].position, 12);
		file.read((char*)&m_Mesh[i * 3 + 1].position, 12);
		file.read((char*)&m_Mesh[i * 3 + 2].position, 12 + 2);//there are two extra bytes at the end of triangles
	}

	//opengl stuff

	m_VArray = vertexArray();
	vertexBuffer((float*)&m_Mesh[0], m_Mesh.size() * sizeof(Vertex), true);

	//positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0);

	//normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (const void*)12);

	//colors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (const void*)24);
}