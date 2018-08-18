#include "game.h"

SpriteRenderer *render;

//initialize the size of the play paddle 
const glm::vec2 PLAYER_SIZE(100, 20);
//initialize the velocity of the play paddle
const GLfloat PLAYER_VELOCITY(500.0f);

glm::vec2 PLAYER_POSITION;
bool  first = true;


GameObject *player;

Game::Game(GLuint width, GLuint height):
	State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{
}

Game::~Game()
{
}

void Game::Init()
{
	//load Shader
	ResourceManager::LoadShader("shaders/sprites.vs", "shaders/sprites.fs", nullptr, "sprite");

	//load Texture
	ResourceManager::LoadTexture("texture/awesomeface.png", GL_TRUE, "face");
	ResourceManager::LoadTexture("texture/background.jpg", GL_FALSE, "background");
	ResourceManager::LoadTexture("texture/block.png", GL_FALSE, "block");
	ResourceManager::LoadTexture("texture/block_solid.png", GL_FALSE, "block_solid");
	ResourceManager::LoadTexture("texture/paddle.png", GL_TRUE, "paddle");

	//load Level
	GameLevel one; one.Load("levels/one.lvl", this->Width, this->Height * 0.5);	//Notice: expression(Height * 0.5) makes sure the block in the top 
	GameLevel two; two.Load("levels/two.lvl", this->Width, this->Height * 0.5);
	GameLevel three; three.Load("levels/three.lvl", this->Width, this->Height * 0.5);
	GameLevel four; four.Load("levels/four.lvl", this->Width, this->Height * 0.5);

	Levels.push_back(one);
	Levels.push_back(two);
	Levels.push_back(three);
	Levels.push_back(four);

	this->Level = 0;

	//set paddle
	/*
	PLAYER_POSITION stores the paddle position in the previous frame to ensure paddle can reach the edge.
	Or when computer renders each frame, the paddle will go back to the center. 
	*/
	if (first) {
		PLAYER_POSITION.x = this->Width / 2 - PLAYER_SIZE.x / 2;
		PLAYER_POSITION.y = this->Height - PLAYER_SIZE.y;
		/*
		glm::vec2 playerPos = {
			this->Width / 2 - PLAYER_SIZE.x / 2,
			this->Height - PLAYER_SIZE.y
		};*/
		first = false;
		player = new GameObject(PLAYER_POSITION, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
	}
	else {
		/*
		glm::vec2 playerPos = {
			PLAYER_POSITION.x,
			this->Height - PLAYER_SIZE.y
		};*/
		player = new GameObject(PLAYER_POSITION, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
	}
	

	//configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width),
		static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").setInt("image", 0);
	ResourceManager::GetShader("sprite").setMat4("projection", projection);

	//set render-specific controls
	render = new SpriteRenderer(ResourceManager::GetShader("sprite"));

}

void Game::ProcessInput(GLfloat dt)
{
	if (this->State == GAME_ACTIVE)
	{
		GLfloat velocity = PLAYER_VELOCITY * dt;
		// move paddle
		if (this->Keys[GLFW_KEY_A])
		{
			if (player->Position.x >= 0)
				player->Position.x -= velocity, PLAYER_POSITION.x = player->Position.x;
		}
		if (this->Keys[GLFW_KEY_D])
		{
			if (player->Position.x <= this->Width - player->Size.x)
				player->Position.x += velocity, PLAYER_POSITION.x = player->Position.x;
		}
	}
}

void Game::Update(GLfloat dt)
{
}

void Game::Render()
{
	//render->DrawSprite(ResourceManager::GetTexture("face"),
		//glm::vec2(200, 200), glm::vec2(300, 400), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	if (State == GAME_ACTIVE) {
		//draw background
		render->DrawSprite(ResourceManager::GetTexture("background"),
			glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f
		);
		//draw level
		this->Levels[Level].Render(*render);
		//draw paddle
		player->Draw(*render);
	}
}
