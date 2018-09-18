/*
*
*
*@author: Aver Jing
*@description：
*@date：
*
*
*/

#include<iostream>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Shader.h>
#include "stb_image_impl.h"
#include "Circle.h"
#include "Polygon.h"

#include <vector>

//setting
const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

//function declaration
void frameBuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);													 // glfw window creation
														 // --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, frameBuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader zprogram
	// ------------------------------------
	Shader ourShader("shaders/collision.vs", "shaders/collision.fs");
	Circle circle1(ourShader, glm::vec2(50.0f, 0.0f), glm::vec2(100.0f, 100.0f), glm::vec2(50.0f, 50.0f), 0.5, 50);
	Circle circle2(ourShader, glm::vec2(200.0f, 300.0f), glm::vec2(150.0f, 150.0f), glm::vec2(70.0f, -60.0f), 0.5, 50);

	std::vector<glm::vec3> poly_vertex1 = {
		{0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
		{0.8f, 0.5f, 0.0f},
		{0.3f, 1.0f, 0.0f}
	};
	std::vector<glm::vec3> poly_vertex2 = {
		{0.2f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
		{0.8f, 0.0f, 0.0f},
		{1.0f, 1.0f, 0.0f}
	};

	Polygons poly1(ourShader, glm::vec2(450.0f, 150.0f),
		glm::vec2(80.0f, 80.0f), glm::vec2(60.0f, -50.0f), poly_vertex1);
	Polygons poly2(ourShader, glm::vec2(600.0f, 300.0f),
		glm::vec2(180.0f, 180.0f), glm::vec2(30.0f, 60.0f), poly_vertex2);

	circle1.Init();
	circle2.Init();
	poly1.Init();
	poly2.Init();
	// render loop
	// -----------

	GLfloat deltaTime = 0.0f;
	GLfloat lastTime = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastTime;
		lastTime = deltaTime;

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//update info
		circle1.Move(deltaTime);
		circle2.Move(deltaTime);
		poly1.Move(deltaTime);
		poly2.Move(deltaTime);

		// render container
		circle1.Draw();
		circle2.Draw();
		poly1.Draw();
		poly2.Draw();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void frameBuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}