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
#include "Basic.h"
#include <vector>
#include <glm/glm.hpp>

extern const int SCR_WIDTH;
extern const int SCR_HEIGHT;

class Polygons :public Basic {
public:
	Polygons(const Shader & shader, const glm::vec2 Position, const glm::vec2 Size, const glm::vec2 velocity,
		const std::vector<glm::vec3>& vertices);
	void Init();
	void Draw();
	void Move(GLfloat dt, const GLuint width = SCR_WIDTH, const GLuint height = SCR_HEIGHT);
private:
	std::vector<glm::vec3> vertices;
};
