/*
*
*
*@author: Aver Jing
*@description：
*@date：
*
*
*/
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Shader.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>
#include <string>

//holds all state information relevent to a character as loaded using FreeType
struct Character {
	GLuint TextureID;	//ID handle of the glyph texture
	glm::ivec2 Size;	//size of glyph
	glm::ivec2 Bearing; //offset from baseline to left/top of glyph
	GLuint Advance;	    //offset to advance to next glyph
};

// a render class for rendering text displayed by a font loaded using FreeType
//library. A single font is loaded, processing into a list of characters 
//for later using.
class TextRender {
public:
	//holds a list of pre-compiled characters
	std::map<GLchar, Character> characters;
	//shader used for text rendering
	Shader TextShader;
	//constructor
	TextRender(GLuint width, GLuint height);
	//pre-compiles a list of characters from a given font 
	void Load(const std::string& font, GLuint fontSize);
	//render a string of text using pre-compiled list of characters
	void RenderText(const std::string&, GLfloat, GLfloat, GLfloat, glm::vec3 color = glm::vec3(1.0f));
private:
	GLuint VAO, VBO;
};