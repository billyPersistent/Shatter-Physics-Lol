#include "vertex array.h"

unsigned int vertexArray(bool bind)
{
	//initialize vertex array

	unsigned int vArray;
	glGenVertexArrays(1, &vArray);

	if (bind)
	{
		glBindVertexArray(vArray);
	}

	return vArray;
}

//initialize vertex and index buffers

unsigned int vertexBuffer(float* data, unsigned int size, bool bind, unsigned int mode)
{
	unsigned int vBuffer;

	glGenBuffers(1, &vBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
	glBufferData(GL_ARRAY_BUFFER, size, data, mode);

	if (!bind)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	return vBuffer;
}

unsigned int indexBuffer(unsigned int* indices, unsigned int size, bool bind, unsigned int mode)
{
	unsigned int iBuffer;

	glGenBuffers(1, &iBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, mode);

	if (!bind)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	return iBuffer;
}