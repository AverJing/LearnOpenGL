/*
*
*
*@author: Aver Jing
*@description：
*@date：
*
*
*/

/*
*
*@author：Aver Jing
*@description：Make sure only the happy looks in the other/reverses direction by changing the fragment shader.
*@date：April 8, 2018
*
*/

#include <iostream>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "TypeDef.h"
#include "Rasterizer.h"
#include "Base/Color.h"
#include "Math/Random.h"

using namespace std;

//setting
const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;


//function declaration
void frameBuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() {
	//glfw initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello", NULL, NULL);
	if (!window) {
		cout << "Failed to create GLFW window. " << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, frameBuffer_size_callback);

	//glad: load all opengl funtion pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed  to initialize GLAD. " << endl;
		return -1;
	}

	RenderContext renderContext;
	renderContext.width = SCR_WIDTH;
	renderContext.height = SCR_HEIGHT;
	renderContext.bpp = sizeof(uint32);
	renderContext.backBuffer = new uint32[renderContext.width * renderContext.height];
	renderContext.depthBuffer = new float[renderContext.width * renderContext.height];

	Rasterizer rasterizer(&renderContext);
	Color tmpColor = Color::black;

	//render loop
	while (!glfwWindowShouldClose(window)) {
		//Input
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		for (int i = 0; i < 100; i++)
		{
			rasterizer.DrawPixel(SCR_WIDTH * Random::Value(), SCR_HEIGHT * Random::Value(), Color::red);
		}

		//glfw: swap buffers and poll IO evenets
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//glfw: teminate, cleaing all previously allocated GLFW resources
	glfwTerminate();

	return 0;
}
//process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

//glfw: whenever the window size changed this callback function executes.
void frameBuffer_size_callback(GLFWwindow* window, int width, int height) {
	//
	glViewport(0, 0, width, height);
}