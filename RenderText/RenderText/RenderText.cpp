/*
*
*
*@author: Aver Jing
*@description：Render text
*@date：Sep 10, 2018
*
*
*/
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Shader.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include <map>
#include <string>

// GLFW function declerations
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

//render text
void RenderText(Shader&, std::string, GLfloat, GLfloat, GLfloat, glm::vec3);
// The Width of the screen
const GLuint SCREEN_WIDTH = 800;
// The height of the screen
const GLuint SCREEN_HEIGHT = 600;

struct Character {
	GLuint TextureID;	//ID handle of the glyph texture
	glm::ivec2 Size;	//size of glyph
	glm::ivec2 Bearing; //offset from baseline to left/top of glyph
	GLuint Advance;	    //offset to advance to next glyph
};

std::map<GLchar, Character> characters;

GLuint VAO, VBO;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (!window) {
		std::cout << "Failed to create GLFW window. " << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//glewExperimental = GL_TRUE;
	//glewInit();
	//glGetError(); // Call it once to catch glewInit() bug, all other errors are now from our application.

	//glad: load all opengl funtion pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed  to initialize GLAD. " << std::endl;
		return -1;
	}

	glfwSetKeyCallback(window, key_callback);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//To load a font, all we have to do is initialize the FreeType library 
	//and load the font as a face as FreeType likes to call it
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init freetype Library" << std::endl;

	FT_Face face;
	if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	FT_Set_Pixel_Sizes(face, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);	//disable byte-aligntment restriction
	for (GLubyte c = 0; c < 128; ++c) {
		//load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "ERROR::FREETYPR: Failed to load glyph" << std::endl;
			continue;
		}
		//generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
			face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer);
		//set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		//now store character for later use
		characters.insert(std::make_pair(c, character));
	}

	//be sure to clear FreeType's resources 
	//once you're finished processing the glyphs
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	// DeltaTime variables
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Because we'll be updating the content of the VBO's memory 
	//quite often we'll allocate the memory with GL_DYNAMIC_DRAW
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 6, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Shader shader("shaders/renderText.vs", "shaders/renderText.fs");
	glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
	shader.use();
	shader.setMat4("projection", projection);

	while (!glfwWindowShouldClose(window))
	{
		// Calculate delta time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		//clear colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		RenderText(shader, "This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5f, 0.8f, 0.2f));
		RenderText(shader, "Aver Jing", 540.0f, 570.0f, 0.5f, glm::vec3(0.3f, 0.7f, 0.9f));
		glfwSwapBuffers(window);
	}


	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

//should understand thoroughly
void RenderText(Shader& shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) {
	//active corresponding render state
	shader.use();
	shader.setVec3("textColor", color);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	//iterate throught all characters
	for (auto c = text.begin(); c != text.end(); ++c) {
		Character ch = characters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;

		//update VBO for each character
		GLfloat vertices[6][4] = {
			{xpos,     ypos + h, 0.0, 0.0},
			{xpos,     ypos  ,   0.0, 1.0},
			{xpos + w, ypos,     1.0, 1.0},

			{xpos,     ypos + h, 0.0, 0.0},
			{xpos + w, ypos,     1.0, 1.0},
			{xpos + w, ypos + h, 1.0, 0.0},
		};
		//render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		//update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}