#pragma once

#include <glew.h>

unsigned int vertexArray(bool bind = true);

unsigned int vertexBuffer(float* data, unsigned int size, bool bind, unsigned int mode = GL_STATIC_DRAW);
unsigned int indexBuffer(unsigned int* indices, unsigned int size, bool bind, unsigned int mode = GL_STATIC_DRAW);