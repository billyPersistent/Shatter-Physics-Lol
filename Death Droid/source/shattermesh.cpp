#include "shattermesh.h"

void ShatterMesh::setBuffers()
{
	//delete buffers to avoid memory leak
	glDeleteVertexArrays(1, &m_VArray);
	glDeleteBuffers(1, &m_VBuffer);
	glDeleteTextures(1, &m_TBuffer);

	//create vertex array
	m_VArray = vertexArray();
	m_VBuffer = vertexBuffer((float*)&m_Mesh[0], m_Mesh.size() * sizeof(Vertex), true);

	//create texture buffer
	glGenTextures(1, &m_TBuffer);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_BUFFER, m_TBuffer);

	//attach vertex buffer to texture buffer
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, m_VBuffer);

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

void ShatterMesh::addVertex(Vertex vertex)
{
	m_Mesh.push_back(vertex);
}

void ShatterMesh::addDuplicateVertex(Vertex vertex)
{
	m_Mesh.push_back(vertex);
}

ShatterMesh::ShatterMesh()
	:resolutionDivide(1.0f), m_BoundingBox()
{

}

unsigned int ShatterMesh::getVArray()
{
	return m_VArray;
}

unsigned int ShatterMesh::getVBuffer()
{
	return m_VBuffer;
}

unsigned int ShatterMesh::getTBuffer()
{
	return m_TBuffer;
}

unsigned int ShatterMesh::getVertexCount()
{
	return m_Mesh.size();
}

void ShatterMesh::loadSTL(std::string path)
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

	//resize both meshes accordingly
	m_Mesh.resize(triangeCount * 3);

	//populate mesh
	for (unsigned int i = 0; i < triangeCount; i++)
	{
		//set colors
		m_Mesh[i * 3].color = glm::vec3(1.0f, 0.0f, 0.0f);
		m_Mesh[i * 3 + 1].color = glm::vec3(1.0f, 0.0f, 0.0f);
		m_Mesh[i * 3 + 2].color = glm::vec3(1.0f, 0.0f, 0.0f);

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

	//find bounding box
	for (int i = 0; i < m_Mesh.size(); i++)
	{
		if (m_Mesh[i].position.x < m_BoundingBox.x)
		{
			m_BoundingBox.x = m_Mesh[i].position.x;
		}
		else if (m_Mesh[i].position.x > m_BoundingBox.xs)
		{
			m_BoundingBox.xs = m_Mesh[i].position.x;
		}

		if (m_Mesh[i].position.y < m_BoundingBox.y)
		{
			m_BoundingBox.y = m_Mesh[i].position.y;
		}
		else if (m_Mesh[i].position.y > m_BoundingBox.ys)
		{
			m_BoundingBox.ys = m_Mesh[i].position.y;
		}

		if (m_Mesh[i].position.z < m_BoundingBox.z)
		{
			m_BoundingBox.z = m_Mesh[i].position.z;
		}
		else if (m_Mesh[i].position.z > m_BoundingBox.zs)
		{
			m_BoundingBox.zs = m_Mesh[i].position.z;
		}
	}

	m_BoundingCenter = glm::vec3((m_BoundingBox.x + m_BoundingBox.xs) / 2.0f, (m_BoundingBox.y + m_BoundingBox.ys) / 2.0f, (m_BoundingBox.z + m_BoundingBox.zs) / 2.0f);
	m_CornerDistance = glm::distance(glm::vec3(m_BoundingBox.x, m_BoundingBox.y, m_BoundingBox.z), glm::vec3(m_BoundingBox.xs, m_BoundingBox.ys, m_BoundingBox.zs));

	//opengl stuff
	setBuffers();
}

const glm::vec3& ShatterMesh::getPosition(int triangleIndex)
{
	return m_Mesh[triangleIndex * 3].position;
}

const glm::vec3& ShatterMesh::getNormal(int triangleIndex)
{
	return m_Mesh[triangleIndex * 3].normal;
}

void ShatterMesh::bodyTruncate(glm::vec3 position, glm::vec3 normal, glm::vec3 color)
{
	std::cout << position.x << ", " << position.y << ", " << position.z << std::endl;
	std::cout << normal.x << ", " << normal.y << ", " << normal.z << std::endl;

	Vertex vertex;
	vertex.normal = glm::normalize(normal);
	vertex.color = color;

	//find nearest point on plane to bounding center (I honestly don't understand this)
	float distanceToPlane = dot(m_BoundingCenter - position, vertex.normal);
	glm::vec3 newPosition = m_BoundingCenter - (vertex.normal * distanceToPlane);

	//if newPosition isn't inside the bounding box, the plane won't actually cut through the mesh
	if (!collide(m_BoundingBox, newPosition))
	{
		return;
	}

	//define diagonal offsets and default normal with respect to each other
	glm::vec3 defaultNormal(0.0f, 1.0f, 0.0f);

	glm::vec3 diagonalOffset1(m_CornerDistance / 2.0f, 0.0f, m_CornerDistance / 2.0f);
	glm::vec3 diagonalOffset2(m_CornerDistance / 2.0f, 0.0f, -m_CornerDistance / 2.0f);

	if (defaultNormal != vertex.normal)
	{
		//find the rotation matrix that will rotate default normal onto vertex normal
		glm::vec3 axis = glm::cross(defaultNormal, vertex.normal);//backwards?
		float angle = glm::atan(glm::length(axis) / glm::dot(defaultNormal, vertex.normal));

		glm::mat4 rotation = glm::mat4_cast(glm::angleAxis(angle, glm::normalize(axis)));

		//rotate diagonal offset
		diagonalOffset1 = rotation * glm::vec4(diagonalOffset1, 1.0f);
		diagonalOffset2 = rotation * glm::vec4(diagonalOffset2, 1.0f);
	}

	//create quad
	vertex.position = newPosition + diagonalOffset1;
	addVertex(vertex);

	vertex.position = newPosition + diagonalOffset2;
	addVertex(vertex);

	vertex.position = newPosition - diagonalOffset2;
	addVertex(vertex);

	vertex.position = newPosition - diagonalOffset1;
	addVertex(vertex);

	vertex.position = newPosition - diagonalOffset2;
	addDuplicateVertex(vertex);

	vertex.position = newPosition + diagonalOffset2;
	addDuplicateVertex(vertex);

	//
	setBuffers();
}

void ShatterMesh::worldTruncate(glm::vec3 position, glm::vec3 normal, glm::vec3 color, Transform transform)
{
	bodyTruncate(glm::inverse(transform.getTranslationMat4()) * glm::vec4(position, 1.0f), glm::inverse(transform.getRotationMat4()) * glm::vec4(normal, 1.0f), color);
}