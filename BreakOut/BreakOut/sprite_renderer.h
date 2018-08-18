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

#include "texture.h"

class SpriteRenderer {
public:
	SpriteRenderer(const Shader &s);
	~SpriteRenderer();

	void DrawSprite(const Texture2D &texture, glm::vec2 position,
		glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f,
		glm::vec3 color = glm::vec3(1.0f));
private:
	Shader shader;
	GLuint quaVAO;

	void InitRenderData();
};