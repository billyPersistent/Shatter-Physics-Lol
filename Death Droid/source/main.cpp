#include <iostream>
#include <chrono>

#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

#include "render.h"
#include "camera.h"
#include "mesh.h"
#include "scene.h"

#include "input.h"//include this last windows.h is evil

int main(void)
{
	GLFWwindow* window;

	//initialize the GLFW library

	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	//create a windowed mode window and its OpenGL context

	const float windowWidth = 1200;
	const float windowHeight = 900;

	window = glfwCreateWindow(windowWidth, windowHeight, "Box Bots", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	//make the window's context current
	glfwMakeContextCurrent(window);

	//check the GLEW library
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Glew IS NOT ok!!" << std::endl;
	}

	//output OpenGL version
	std::cout << "OpenGL version " << glGetString(GL_VERSION) << std::endl;

	//set blend standards
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//create scene
	glm::mat4 projP = glm::perspective(glm::radians(45.0f), windowWidth / windowHeight, 1.0f, 20000.0f);
	Scene scene(projP, windowWidth, windowHeight);

	//initialize variables
	unsigned int FPS = 60;
	auto loopStart = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> loopDur = std::chrono::high_resolution_clock::now() - loopStart;

	//unbind all
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);

	//loop until the user closes the window
	while (!glfwWindowShouldClose(window))
	{
		loopStart = std::chrono::high_resolution_clock::now();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//update scene
		scene.update(loopDur.count());

		//swap front and back buffers
		glfwSwapBuffers(window);

		//poll for and process events
		glfwPollEvents();

		//adjust for framerate
		/*loopDur = std::chrono::high_resolution_clock::now() - loopStart;
		std::cout << (loopDur.count() / (1.0f / FPS)) * 100 << "% of alloted time" << std::endl;
		loopDur = std::chrono::high_resolution_clock::now() - loopStart;

		while (loopDur.count() < 1.0f / FPS)
		{
			loopDur = std::chrono::high_resolution_clock::now() - loopStart;
		}*/
		loopDur = std::chrono::high_resolution_clock::now() - loopStart;
		//std::cout << loopDur.count() << std::endl;
	}

	glfwTerminate();
	return 0;
}