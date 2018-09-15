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

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Shader.h>
#include <vector>

using std::vector;

#include "texture.h"

class SpriteRenderer {
public:
	SpriteRenderer(const Shader &s);
	~SpriteRenderer();

	void DrawSprite(const Texture2D &texture, glm::vec2 position,
		glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f,
		glm::vec3 color = glm::vec3(1.0f));
	void DrawSprite(const vector<GLuint>& texture, const vector<glm::mat4>& model, const vector<glm::vec3>& color);
private:
	Shader shader;
	GLuint quaVAO;

	void InitRenderData();
};