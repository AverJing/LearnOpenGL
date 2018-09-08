/*
*
*
*@author: Aver Jing
*@description：ball class 
*@date：Aug 20, 2018
*
*
*/
#pragma once
#include "game_object.h"

class BallObject : public GameObject {
public:
	//ball state
	GLfloat Radius;
	GLboolean Stuck, PassThrough, Sticky;
	//constructors
	BallObject();
	BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite);
	// Moves the ball, keeping it constrained within the window bounds (except bottom edge); returns new position
	glm::vec2 Move(GLfloat dt, GLuint width);
	// Resets the ball to original state with given position and velocity
	void Reset(glm::vec2 pos, glm::vec2 velocity);
};
