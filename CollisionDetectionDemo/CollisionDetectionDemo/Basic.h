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
#include <glad/glad.h>

extern const int SCR_WIDTH;
extern const int SCR_HEIGHT;

class Basic {
public:
	Basic() {}
	Basic(const Shader& shader,const glm::vec2 Position, const glm::vec2 Size, const glm::vec2 velocity) 
		:shader(shader), Position(Position), Size(Size), Velocity(velocity) {}
	virtual ~Basic() { glDeleteVertexArrays(1, &VAO); }
	void updateModel();
	void setProjection();
	glm::vec2 Position;
	glm::vec2 Size;
	glm::vec2 Velocity;
	GLuint VAO;
	Shader shader;
};

inline void Basic::updateModel()
{
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(Position, 0.0f));
	model = glm::scale(model, glm::vec3(Size, 1.0f));
	shader.use();
	shader.setMat4("model", model);
}

inline void Basic::setProjection()
{	//
	//glm::mat4 projection = glm::ortho(0, SCR_WIDTH, SCR_HEIGHT, 0, -1, 1);
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(SCR_WIDTH),
		static_cast<GLfloat>(SCR_HEIGHT), 0.0f, -1.0f, 1.0f);
	shader.use();
	shader.setMat4("projection", projection);
}
