#include "Polygon.h"

Polygons::Polygons(const Shader & shader, const glm::vec2 Position, const glm::vec2 Size, const glm::vec2 velocity,  const std::vector<glm::vec3>& vertices)
	:Basic(shader, Position, Size, velocity), vertices(vertices)
{}

void Polygons::Init()
{
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

void Polygons::Draw()
{
	updateModel();
	//shader.use();
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_LINE_LOOP, 0, vertices.size());
	//glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

void Polygons::Move(GLfloat dt, const GLuint width, const GLuint height)
{

	this->Position += this->Velocity * dt * 0.0001f;
	if (this->Position.x <= 0.0f) {
		this->Velocity.x = -this->Velocity.x;
		this->Position.x = 0.0f;
	}
	else if (this->Position.x + this->Size.x >= width) {
		this->Velocity.x = -this->Velocity.x;
		this->Position.x = width - this->Size.x ;
	}
	if (this->Position.y <= 0.0f)
	{
		this->Velocity.y = -this->Velocity.y;
		this->Position.y = 0.0f;
	}
	else if (this->Position.y + this->Size.y>= height) {
		this->Velocity.y = -this->Velocity.y;
		this->Position.y = height - this->Size.y;
	}
}


