/*
*
*@author£ºAver Jing
*@description£ºtry to draw 2 triangles next to each other  using glDrawArray.
*@date£ºApril 4, 2018
*
*/

#include<iostream>
#include <glad\glad.h>
#include <GLFW\glfw3.h>

using namespace std;

//setting
const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

//vertex shader
const char* vertexShaderSource = {
	"#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"gl_Position = vec4(aPos, 1.0f);\n"
	"}\n"
};
//fragment shader
const char* fragShaderSource = {
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n"
};

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

	//glad: load all opengl funtion pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed  to initialize GLAD. " << endl;
		return -1;
	}

	//vertex attributes
	//float vertices[] = {
	//	//first triangle
	//	0.5f, 0.5f, 0.0f,// top right
	//	0.5f, -0.5f, 0.0f,//buttom right
	//	-0.5f, 0.5f, 0.0f,//top tight
	//	//second triangle
	//	0.5f, -0.5f, 0.0f,  // buttom right
	//	-0.5f, -0.5f, 0.0f, // buttom left
	//	-0.5f, 0.5f, 0.0f   // top left

	//};
	float vertices[] = {
		0.5f, 0.5f, 0.0f,// top right
		0.5f, -0.5f, 0.0f,//buttom right
		-0.5f, -0.5f, 0.0f, // buttom left
		-0.5f, 0.5f, 0.0f,   // top left
		1.0f, 0.0f, 0.0f

	};

	unsigned int indices[] = {
		0, 1, 4,//first triangle
		0, 1, 2//second triangle
	};

	//set VBO
	unsigned int VBO, VAO, EBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//link vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glfwSetFramebufferSizeCallback(window, frameBuffer_size_callback);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//create vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	//When errors occur, print errors message
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX_COMPILATION_FAILED\n" << infoLog << endl;
	}

	//create fragment shader
	unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragShaderSource, NULL);
	glCompileShader(fragShader);
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT_COMPILATION_FAILED\n" << infoLog << endl;
	}

	//link shaders
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);
	//If errors occurr, print error messages.
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << endl;
	}

	//release shader resource
	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);

	//render loop
	while (!glfwWindowShouldClose(window)) {
		//Input
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw our first triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//glfw: swap buffers and poll IO evenets
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

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