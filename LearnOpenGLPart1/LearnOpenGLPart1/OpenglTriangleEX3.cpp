/*
*
*@author£ºAver Jing
*@description£ºCreate two shader programs where the second program uses a different fragment shader that outputs the color yellow.
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
const char* firstFragShaderSource = {
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n"
};
const char* secondFragShaderSource = {
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
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
	float fisrstTriangle[] = {
		//first triangle
		0.5f, 0.5f, 0.0f,// top right
		0.5f, -0.5f, 0.0f,//buttom right
		-0.5f, 0.5f, 0.0f,//top right
	};
	float secondTriangle[] = {
		//second triangle
		0.5f, -0.5f, 0.0f,  // buttom right
		-0.5f, -0.5f, 0.0f, // buttom left
		-0.5f, 0.5f, 0.0f   // top left
	};

	//set VBO
	unsigned int VBOs[2], VAOs[2];
	glGenBuffers(2, VBOs);
	glGenVertexArrays(2, VAOs); // we can also generate multiple VAOs or buffers at the same time

								//first triangle setup
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fisrstTriangle), fisrstTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//second triangle setup
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
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

	//create first fragment shader
	unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &firstFragShaderSource, NULL);
	glCompileShader(fragShader);
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT_COMPILATION_FAILED\n" << infoLog << endl;
	}

	//create second fragment shader
	unsigned int fragShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShaderYellow, 1, &secondFragShaderSource, NULL);
	glCompileShader(fragShaderYellow);
	glGetShaderiv(fragShaderYellow, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragShaderYellow, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT_COMPILATION_FAILED\n" << infoLog << endl;
	}

	//link the first program object
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

	// then link the second program object using a different fragment shader (but same vertex shader)
	// this is perfectly allowed since the inputs and outputs of both the vertex and fragment shaders are equally matched.
	unsigned int shaderProgramYellow = glCreateProgram();
	glAttachShader(shaderProgramYellow, vertexShader);
	glAttachShader(shaderProgramYellow, fragShaderYellow);
	glLinkProgram(shaderProgramYellow);
	//If errors occurr, print error messages.
	glGetProgramiv(shaderProgramYellow, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgramYellow, 512, NULL, infoLog);
		cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << endl;
	}

	//release shader resource
	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);
	glDeleteShader(fragShaderYellow);

	//render loop
	while (!glfwWindowShouldClose(window)) {
		//Input
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw our first triangle
		glUseProgram(shaderProgram);
		// draw first triangle using the data from the first VAO
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// then we draw the second triangle using the data from the second VAO
		// when we draw the second triangle we want to use a different shader program so we switch to the shader program with our yellow fragment shader.
		glUseProgram(shaderProgramYellow);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//glfw: swap buffers and poll IO evenets
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);

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