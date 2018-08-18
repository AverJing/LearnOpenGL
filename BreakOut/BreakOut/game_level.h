/*
*
*
*@author: Aver Jing
*@description：load game level from file
*@date：Aug 17, 2018
*
*
*/
#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "sprite_renderer.h"
#include "game_object.h"
#include "resource_manager.h"

/// GameLevel holds all Tiles as part of a Breakout level and 
/// hosts functionality to Load/render levels from the harddisk.
class GameLevel {
public:
	//level state
	std::vector<GameObject> Bricks;
	//constructor
	GameLevel() {}
	//load level from file
	void Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight);
	//render level
	void Render(SpriteRenderer &renderer);
	//check if the level is completed (all non-solid tiles are destroyed)
	GLboolean IsComleted();
private:
	//intialize level from tile data
	void Init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);
};