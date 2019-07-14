#include "sprite_renderer.h"
#include "resource_manager.h"

SpriteRenderer::SpriteRenderer(const Shader & s)
{
	InitRenderData();
	shader = s;
}

SpriteRenderer::~SpriteRenderer()
{
	glDeleteVertexArrays(1, &this->quaVAO);
}

void SpriteRenderer::DrawSprite(const Texture2D & texture, glm::vec2 position, glm::vec2 size, GLfloat rotate, glm::vec3 color)
{
	//prepare transformations
	this->shader.use();

	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));

	//model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	//model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
	//model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

	model = glm::scale(model, glm::vec3(size, 1.0f));

	this->shader.setMat4("model", model);
	this->shader.setVec3("spriteColor", color);

	glActiveTexture(GL_TEXTURE0);
	texture.Bind();

	glBindVertexArray(this->quaVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//glBindTexture(GL_TEXTURE, 0);
	//glBindVertexArray(0);
}
//use instance rendering
void SpriteRenderer::DrawSprite(const vector<GLuint>& texture, const vector<glm::mat4>& model,
 const vector<glm::vec3>& color)
{
	//prepare transformations
	ResourceManager::GetShader("level").use();

	glActiveTexture(GL_TEXTURE0);
	//ResourceManager::GetTexture("block").Bind();
	glBindTexture(GL_TEXTURE_2D, ResourceManager::GetTexture("block").ID);
	glActiveTexture(GL_TEXTURE1);
	//ResourceManager::GetTexture("block_solid").Bind();
	glBindTexture(GL_TEXTURE_2D, ResourceManager::GetTexture("block_solid").ID);

	glBindVertexArray(this->quaVAO);

	unsigned int instanceColor;
	glGenBuffers(1, &instanceColor);
	glBindBuffer(GL_ARRAY_BUFFER, instanceColor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * color.size(), &color.front(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glVertexAttribDivisor(1, 1);

	GLuint vec4Size = sizeof(glm::vec4);
	unsigned int instanceModel;
	glGenBuffers(1, &instanceModel);
	glBindBuffer(GL_ARRAY_BUFFER, instanceModel);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * model.size(), &model.front(), GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
	
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
	
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2*vec4Size));

	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3*vec4Size));

	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);

	unsigned int index;
	glGenBuffers(1, &index);
	glBindBuffer(GL_ARRAY_BUFFER, index);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLuint) * texture.size(), &texture.front(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 1, GL_INT, GL_FALSE, sizeof(GLuint), (void*)(0));
	glVertexAttribDivisor(6, 1);


	//glBindVertexArray(this->quaVAO);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, model.size());
	//glBindTexture(GL_TEXTURE, 0);
	glBindVertexArray(0);
}

void SpriteRenderer::InitRenderData()
{
	GLuint VBO;
	GLfloat vertices[] = {
		// position     // texture
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->quaVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->quaVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid *)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
