/*
*
*
*@author: Aver Jing
*@description：BreakOut setting-up
*@date：Aug 15, 2018
*
*
*/
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "sprite_renderer.h"
#include "resource_manager.h"
#include "game_level.h"
#include "ball_object_collisions.h"
#include "power_up.h"

#include <vector>
#include <tuple>

// Represents the current state of the game
enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

//base direction for accumulating target vector direction
enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;


// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
	// Game state
	GameState              State;
	GLboolean              Keys[1024];
	GLuint                 Width, Height;
	std::vector<GameLevel> Levels;
	GLuint				   Level;

	//powerups
	std::vector<PowerUp> PowerUps;

	// Constructor/Destructor
	Game(GLuint width, GLuint height);
	~Game();
	// Initialize game state (load all shaders/textures/levels)
	void Init();
	// GameLoop
	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render();
	//check collision
	GLboolean checkCollision(GameObject&, GameObject&);
	//GLboolean checkCollision(BallObject&, GameObject&);
	Collision checkCollision(BallObject&, GameObject&);
	void doCollision();
	Direction VectorDirection(glm::vec2);

	//manage PowerUps
	void spawnPowerUps(GameObject& block);
	void updatePowerUps(GLfloat dt);

	// Reset
	void ResetLevel();
	void ResetPlayer();
};
