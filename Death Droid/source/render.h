#pragma once

#include <string>

#include <glew.h>
#include <gtc/type_ptr.hpp>

#include "camera.h"
#include "shattermesh.h"
#include "shader.h"
#include "transform.h"

class Renderer
{
private:
	Camera* m_Camera;

	float m_WinWidth;
	float m_WinHeight;

	unsigned int m_MeshShader;
	unsigned int m_ShatterMeshShader;
	unsigned int m_ShatterMeshShader1;
	unsigned int m_ShatterMeshShader2;
	unsigned int m_ShatterMeshShader3;

	unsigned int m_PlaneFrameBuffer;
	unsigned int m_PlaneDepthTexture;

	unsigned int m_AtomicBuffer;
	unsigned int m_LastAtomic;
public:

	Renderer(Camera* cam, int winWidth, int winHeight);

	void render(Mesh& mesh, Transform& transform);
	void render(ShatterMesh& mesh, Transform& transform);
};