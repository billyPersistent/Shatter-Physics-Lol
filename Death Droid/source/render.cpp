#include "render.h"

Renderer::Renderer(Camera* cam, int winWidth, int winHeight)
	:m_WinWidth(winWidth), m_WinHeight(winHeight), m_LastAtomic(0)
{
	m_Camera = cam;

	//shaders
	m_MeshShader = createShader(loadShader("shaders/meshv.shader"), loadShader("shaders/meshf.shader"));
	m_ShatterMeshShader = createShader(loadShader("shaders/shattermeshv.shader"), loadShader("shaders/shattermeshf.shader"));
	m_ShatterMeshShader1 = createShader(loadShader("shaders/shattermesh1v.shader"), loadShader("shaders/shattermesh1g.shader"), loadShader("shaders/shattermesh1f.shader"));
	m_ShatterMeshShader2 = createShader(loadShader("shaders/shattermesh2v.shader"), loadShader("shaders/shattermesh2f.shader"));
	m_ShatterMeshShader3 = createShader(loadShader("shaders/shattermesh3v.shader"), loadShader("shaders/shattermesh3f.shader"));

	//create plane frame buffer
	glGenFramebuffers(1, &m_PlaneFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_PlaneFrameBuffer);

	//create plane depth texture
	glGenTextures(1, &m_PlaneDepthTexture);
	glBindTexture(GL_TEXTURE_2D, m_PlaneDepthTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_WinWidth, m_WinHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	//attach plane depth texture
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_PlaneDepthTexture, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//atomic counter
	glGenBuffers(1, &m_AtomicBuffer);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 1, m_AtomicBuffer);
	glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(unsigned int), NULL, GL_STATIC_DRAW);
}

void Renderer::render(Mesh& mesh, Transform& transform)
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//bind
	glBindVertexArray(mesh.getVArray());
	glUseProgram(m_MeshShader);

	//set rotation matrix
	int rotationLocation = glGetUniformLocation(m_MeshShader, "u_Rotation");
	glUniformMatrix4fv(rotationLocation, 1, GL_FALSE, transform.getRotationM());

	//set translation matrix
	int translationLocation = glGetUniformLocation(m_MeshShader, "u_Translation");
	glUniformMatrix4fv(translationLocation, 1, GL_FALSE, transform.getTranslationM());

	//set camera matrix
	int cameraLocation = glGetUniformLocation(m_MeshShader, "u_Camera");
	glUniformMatrix4fv(cameraLocation, 1, GL_FALSE, m_Camera->getMatrix());

	//set light pos
	int lightPosLocation = glGetUniformLocation(m_MeshShader, "u_LightPos");
	glUniform3f(lightPosLocation, m_Camera->getPos().x, m_Camera->getPos().y, m_Camera->getPos().z);

	//set light color
	int lightColorLocation = glGetUniformLocation(m_MeshShader, "u_LightColor");
	glUniform3f(lightColorLocation, 1.0f, 1.0f, 1.0f);

	//draw
	glDrawArrays(GL_TRIANGLES, 0, mesh.getVertexCount());

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}

void Renderer::render(ShatterMesh& mesh, Transform& transform)
{
	//bind
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_BUFFER, mesh.getTBuffer());

	glBindVertexArray(mesh.getVArray());
	glUseProgram(m_ShatterMeshShader);

	//set rotation matrix
	int rotationLocation = glGetUniformLocation(m_ShatterMeshShader, "u_Rotation");
	glUniformMatrix4fv(rotationLocation, 1, GL_FALSE, transform.getRotationM());

	//set translation matrix
	int translationLocation = glGetUniformLocation(m_ShatterMeshShader, "u_Translation");
	glUniformMatrix4fv(translationLocation, 1, GL_FALSE, transform.getTranslationM());

	//set camera matrix
	int cameraLocation = glGetUniformLocation(m_ShatterMeshShader, "u_Camera");
	glUniformMatrix4fv(cameraLocation, 1, GL_FALSE, m_Camera->getMatrix());

	//set light pos
	int lightPosLocation = glGetUniformLocation(m_ShatterMeshShader, "u_LightPos");
	glUniform3f(lightPosLocation, m_Camera->getPos().x, m_Camera->getPos().y, m_Camera->getPos().z);

	//set light color
	int lightColorLocation = glGetUniformLocation(m_ShatterMeshShader, "u_LightColor");
	glUniform3f(lightColorLocation, 1.0f, 1.0f, 1.0f);

	//set plane/triangle count
	int planeCountLocation = glGetUniformLocation(m_ShatterMeshShader, "u_PlaneCount");
	glUniform1i(planeCountLocation, mesh.getVertexCount() / 3);

	//set vertex TBO sampler
	int samplerLocation = glGetUniformLocation(m_ShatterMeshShader, "u_TBOSampler");
	glUniform1i(samplerLocation, 0);//slot 0

	//draw
	glDrawArrays(GL_TRIANGLES, 0, mesh.getVertexCount());

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	/*//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	if (mesh.resolutionDivide <= 1.0f)
	{
		//bind
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_BUFFER, mesh.getTBuffer());
		glBindVertexArray(mesh.getVArray());
		glUseProgram(m_ShatterMeshShader);
		glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 1, m_AtomicBuffer);

		//set rotation matrix
		int rotationLocation = glGetUniformLocation(m_ShatterMeshShader, "u_Rotation");
		glUniformMatrix4fv(rotationLocation, 1, GL_FALSE, transform.getRotationM());

		//set translation matrix
		int translationLocation = glGetUniformLocation(m_ShatterMeshShader, "u_Translation");
		glUniformMatrix4fv(translationLocation, 1, GL_FALSE, transform.getTranslationM());

		//set camera matrix
		int cameraLocation = glGetUniformLocation(m_ShatterMeshShader, "u_Camera");
		glUniformMatrix4fv(cameraLocation, 1, GL_FALSE, m_Camera->getMatrix());

		//set light pos
		int lightPosLocation = glGetUniformLocation(m_ShatterMeshShader, "u_LightPos");
		glUniform3f(lightPosLocation, m_Camera->getPos().x, m_Camera->getPos().y, m_Camera->getPos().z);

		//set light color
		int lightColorLocation = glGetUniformLocation(m_ShatterMeshShader, "u_LightColor");
		glUniform3f(lightColorLocation, 1.0f, 1.0f, 1.0f);

		//set plane/triangle count
		int planeCountLocation = glGetUniformLocation(m_ShatterMeshShader, "u_PlaneCount");
		glUniform1i(planeCountLocation, mesh.getVertexCount() / 3);

		//set vertex TBO sampler
		int samplerLocation = glGetUniformLocation(m_ShatterMeshShader, "u_TBOSampler");
		glUniform1i(samplerLocation, 0);//slot 0

		//draw
		glDrawArrays(GL_TRIANGLES, 0, mesh.getVertexCount());
	}
	else
	{
		//plane depth pass
		{
			//bind
			glBindFramebuffer(GL_FRAMEBUFFER, m_PlaneFrameBuffer);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_BUFFER, mesh.getTBuffer());
			glBindVertexArray(mesh.getVArray());
			glUseProgram(m_ShatterMeshShader2);
			glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 1, m_AtomicBuffer);

			//set resolution divide
			int resolutionLocation = glGetUniformLocation(m_ShatterMeshShader2, "u_ResolutionDivide");
			glUniform1f(resolutionLocation, mesh.resolutionDivide);

			//set rotation matrix
			int rotationLocation = glGetUniformLocation(m_ShatterMeshShader2, "u_Rotation");
			glUniformMatrix4fv(rotationLocation, 1, GL_FALSE, transform.getRotationM());

			//set translation matrix
			int translationLocation = glGetUniformLocation(m_ShatterMeshShader2, "u_Translation");
			glUniformMatrix4fv(translationLocation, 1, GL_FALSE, transform.getTranslationM());

			//set camera matrix
			int cameraLocation = glGetUniformLocation(m_ShatterMeshShader2, "u_Camera");
			glUniformMatrix4fv(cameraLocation, 1, GL_FALSE, m_Camera->getMatrix());

			//set plane/triangle count
			int planeCountLocation = glGetUniformLocation(m_ShatterMeshShader2, "u_PlaneCount");
			glUniform1i(planeCountLocation, mesh.getVertexCount() / 3);

			//set vertex TBO sampler
			int samplerLocation = glGetUniformLocation(m_ShatterMeshShader2, "u_TBOSampler");
			glUniform1i(samplerLocation, 0);//slot 0

			//draw
			glDrawArrays(GL_TRIANGLES, 0, mesh.getVertexCount());

			//bind screen
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		//draw / lighting pass
		{
			//bind
			glUseProgram(m_ShatterMeshShader3);
			glBindTexture(GL_TEXTURE_2D, m_PlaneDepthTexture);

			//set resolution divide
			int resolutionLocation = glGetUniformLocation(m_ShatterMeshShader3, "u_ResolutionDivide");
			glUniform1f(resolutionLocation, mesh.resolutionDivide);

			//set rotation matrix
			int rotationLocation = glGetUniformLocation(m_ShatterMeshShader3, "u_Rotation");
			glUniformMatrix4fv(rotationLocation, 1, GL_FALSE, transform.getRotationM());

			//set translation matrix
			int translationLocation = glGetUniformLocation(m_ShatterMeshShader3, "u_Translation");
			glUniformMatrix4fv(translationLocation, 1, GL_FALSE, transform.getTranslationM());

			//set camera matrix
			int cameraLocation = glGetUniformLocation(m_ShatterMeshShader3, "u_Camera");
			glUniformMatrix4fv(cameraLocation, 1, GL_FALSE, m_Camera->getMatrix());

			//set light pos
			int lightPosLocation = glGetUniformLocation(m_ShatterMeshShader3, "u_LightPos");
			glUniform3f(lightPosLocation, m_Camera->getPos().x, m_Camera->getPos().y, m_Camera->getPos().z);

			//set light color
			int lightColorLocation = glGetUniformLocation(m_ShatterMeshShader3, "u_LightColor");
			glUniform3f(lightColorLocation, 1.0f, 1.0f, 1.0f);

			//set depth texture sampler2d
			int samplerLocation = glGetUniformLocation(m_ShatterMeshShader3, "u_DepthTexture");
			glUniform1i(samplerLocation, 0);//slot 0

			//set depth texture sampler2d
			int winSizeLocation = glGetUniformLocation(m_ShatterMeshShader3, "u_WindowSize");
			glUniform2f(winSizeLocation, m_WinWidth, m_WinHeight);//slot 0

			//draw
			glDrawArrays(GL_TRIANGLES, 0, mesh.getVertexCount());
		}
	}

	//clear framebuffer
	if (mesh.resolutionDivide > 1.0f)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_PlaneFrameBuffer);
		glClear(GL_DEPTH_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	//read atomic fragment counter
	unsigned int counterRead = 0;
	unsigned int fragCount = 0;
	glGetBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(unsigned int), &counterRead);
	fragCount = counterRead - m_LastAtomic;
	m_LastAtomic = counterRead;

	//adjust meshes resolution divide
	if (fragCount > 25000)
	{
		mesh.resolutionDivide += 0.5f;
	}
	else if (fragCount < 19000 and mesh.resolutionDivide >= 1.1f)
	{
		mesh.resolutionDivide -= 0.5f;
	}

	//reset sometimes (coming soon)

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	//mesh.resolutionDivide = 2.0f;*/
}