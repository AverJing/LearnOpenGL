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
#include <string>
#include "game_object.h"

const glm::vec2 POWERUP_SIZE(60, 20);
const glm::vec2 VELOCITY(0.0f, 150.0f);


class PowerUp : public GameObject{
public:
	//powerup state
	std::string Type;
	GLboolean Activated;
	GLfloat Duration;

	//constructor
	PowerUp(std::string type, glm::vec3 color, GLfloat duration,
		glm::vec2 position, Texture2D texture)
		:GameObject(position, POWERUP_SIZE, texture, color, VELOCITY), Type(type),
		Duration(duration), Activated() {}
};
