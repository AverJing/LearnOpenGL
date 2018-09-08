#include "ball_object_collisions.h"

BallObject::BallObject() : GameObject(), Radius(12.5f), Stuck(true)
{}

BallObject::BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite)
	: GameObject(pos, glm::vec2(radius * 2, radius * 2), sprite, glm::vec3(1.0f), velocity),Radius(radius), Stuck(true),PassThrough(false)
{}

glm::vec2 BallObject::Move(GLfloat dt, GLuint width)
{
	//move the ball
	if (!this->Stuck) {	
		this->Position += this->Velocity * (dt * 0.5f);
		// Then check if outside window bounds and if so, reverse velocity and restore at correct position
		if (this->Position.x <= 0.0f)
		{
			this->Velocity.x = -this->Velocity.x;
			this->Position.x = 0.0f;
		}
		else if (this->Position.x + this->Size.x >= width)
		{
			this->Velocity.x = -this->Velocity.x;
			this->Position.x = width - this->Size.x;
		}
		if (this->Position.y <= 0.0f)
		{
			this->Velocity.y = -this->Velocity.y;
			this->Position.y = 0.0f;
		}
	}
	return this->Position;
}
// Resets the ball to initial Stuck Position (if ball is outside window bounds)
void BallObject::Reset(glm::vec2 pos, glm::vec2 velocity) {
	this->Position = pos;
	this->Velocity = velocity;
	this->Stuck = true;
	this->PassThrough = false;
}
