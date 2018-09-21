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

#include <vector>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Basic.h"

const float PI = 3.1415926;
extern const int SCR_WIDTH;
extern const int SCR_HEIGHT;

class Circle : public Basic
{
public:
	Circle() {}
	Circle(const Shader& shader, const glm::vec2 Position, const glm::vec2 Size, const glm::vec2 velocity,const float radius, const unsigned int n):
		Basic(shader, Position, Size, velocity), radius(radius), n(n) {};
	~Circle() {};

	void Init();
	void Draw();	
	void Move(GLfloat dt, const GLuint width = SCR_WIDTH, const GLuint height = SCR_HEIGHT);
private:
	std::vector<glm::vec3> vertices;
	unsigned int n;
	float radius;
	void setPointOfCircle();
};

void Circle::setPointOfCircle()
{	
	for (auto i = 0; i < n; ++i)
		vertices.push_back(
			glm::vec3(radius * cos(2 * PI / n * i),
				radius * sin(2 * PI / n * i), 0.0f));

}

inline void Circle::Init()
{
	setPointOfCircle();
	unsigned int VBO;
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices.front(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	setProjection();
}

inline void Circle::Draw()
{
	updateModel();
	//shader.use();
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_LINE_LOOP, 0, vertices.size());
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

void Circle::Move(GLfloat dt, const GLuint width, const GLuint height)
{
	
	this->Position += this->Velocity * dt * 0.0001f;
	if (this->Position.x <= 0.0f + this->Size.x) {
		this->Velocity.x = -this->Velocity.x;
		this->Position.x = this->Size.x;
	}
	else if (this->Position.x + this->Size.x >= width) {
		this->Velocity.x = -this->Velocity.x;
		this->Position.x = width - this->Size.x;
	}
	if (this->Position.y <= 0.0f + this->Size.y)
	{
		this->Velocity.y = -this->Velocity.y;
		this->Position.y = this->Size.y;
	}
	else if (this->Position.y + this->Size.y >= height) {
		this->Velocity.y = -this->Velocity.y;
		this->Position.y = height - this->Size.y;
	}
}
