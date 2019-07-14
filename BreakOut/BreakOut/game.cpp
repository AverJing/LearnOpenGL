#include "game.h"
#include "particle_generator.h"
#include "post_processor.h"
#include "text_render.h"
#include <algorithm>
#include <string>
#include <irrklang/irrKlang.h>
using namespace irrklang;

//function declaration
GLboolean shouldSpawnPowerUps(GLuint);
void activePowerUp(PowerUp& );
GLboolean isOtherPowerUpActive(std::vector<PowerUp>&, std::string);

//game-related state
SpriteRenderer *render;
GameObject *player;
BallObject *ball;
ParticleGenerator *particles;
PostProcessor     *effects;
ISoundEngine *SoundEngine = createIrrKlangDevice();
TextRender *Text;

GLfloat            ShakeTime = 0.0f;

//initialize the size of the play paddle 
const glm::vec2 PLAYER_SIZE(100, 20);
//initialize the velocity of the play paddle
const GLfloat PLAYER_VELOCITY(500.0f);

glm::vec2 PLAYER_POSITION;
bool  first = true;

//initial velocity of ball
const glm::vec2 BALL_VELOCITY(100.0f, -350.0f);
//initial radius of ball
const GLfloat BALL_RADIUS = 12.5f;

Game::Game(GLuint width, GLuint height):
	State(GAME_ACTIVE), Keys(), Width(width), Height(height), Lives(1)
{
}

Game::~Game()
{
	delete render;
	delete player;
	delete ball;
	delete particles;
	delete effects;
	SoundEngine->drop();
}

void Game::Init()
{
	//load Shader
	ResourceManager::LoadShader("shaders/sprites.vs", "shaders/sprites.fs", nullptr, "sprite");
	ResourceManager::LoadShader("shaders/particle.vs", "shaders/particle.fs", nullptr, "particle");
	ResourceManager::LoadShader("shaders/post_processor.vs", "shaders/post_processor.fs", nullptr, "postprocessor");
	ResourceManager::LoadShader("shaders/spritesInstance.vs", "shaders/spritesInstance.fs", nullptr, "level");
	//configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width),
		static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
	//remember when you set something in shader, please use first.
	//attention
	//attention
	//attention
	//I have been stuck here for few days.  It's hard to debug.....
	ResourceManager::GetShader("sprite").use();
	ResourceManager::GetShader("sprite").setInt("image", 0);
	ResourceManager::GetShader("sprite").setMat4("projection", projection);
	ResourceManager::GetShader("particle").use();
	ResourceManager::GetShader("particle").setInt("sprite", 0);
	ResourceManager::GetShader("particle").setMat4("projection", projection);
	ResourceManager::GetShader("level").use();
	ResourceManager::GetShader("level").setInt("image[0]", 0);
	ResourceManager::GetShader("level").setInt("image[1]", 1);
	ResourceManager::GetShader("level").setMat4("projection", projection);

	//load Texture
	ResourceManager::LoadTexture("texture/awesomeface_sec.png", GL_TRUE, "face");
	ResourceManager::LoadTexture("texture/background.jpg", GL_FALSE, "background");
	ResourceManager::LoadTexture("texture/block.png", GL_FALSE, "block");
	ResourceManager::LoadTexture("texture/block_solid.png", GL_FALSE, "block_solid");
	ResourceManager::LoadTexture("texture/paddle.png", GL_TRUE, "paddle");
	ResourceManager::LoadTexture("texture/particle.png", GL_TRUE, "particle");
	ResourceManager::LoadTexture("texture/powerup_speed.png", GL_TRUE, "powerup_speed");
	ResourceManager::LoadTexture("texture/powerup_sticky.png", GL_TRUE, "powerup_sticky");
	ResourceManager::LoadTexture("texture/powerup_increase.png", GL_TRUE, "powerup_increase");
	ResourceManager::LoadTexture("texture/powerup_confuse.png", GL_TRUE, "powerup_confuse");
	ResourceManager::LoadTexture("texture/powerup_chaos.png", GL_TRUE, "powerup_chaos");
	ResourceManager::LoadTexture("texture/powerup_passthrough.png", GL_TRUE, "powerup_passthrough");
	//set render-specific controls
	render = new SpriteRenderer(ResourceManager::GetShader("sprite"));

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
		//load ball
		glm::vec2 ballPos = PLAYER_POSITION + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -2 * BALL_RADIUS);
		ball = new BallObject(ballPos, BALL_RADIUS, BALL_VELOCITY, ResourceManager::GetTexture("face"));
	}
	else {
		/*
		glm::vec2 playerPos = {
			PLAYER_POSITION.x,
			this->Height - PLAYER_SIZE.y
		};*/
		player = new GameObject(PLAYER_POSITION, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
	}

	particles = new ParticleGenerator(
		ResourceManager::GetShader("particle"),
		ResourceManager::GetTexture("particle"),
		500);
	effects = new PostProcessor(ResourceManager::GetShader("postprocessor"), this->Width, this->Height);
	//effects->Shake = GL_TRUE;
	//effects->Confuse = GL_TRUE;
	//effects->Chaos = GL_TRUE;

	//audio
	SoundEngine->play2D("audio/breakout.mp3", GL_TRUE);

	//load text
	Text = new TextRender(this->Width, this->Height);
	Text->Load("fonts/arial.ttf", 24);
}

void Game::ProcessInput(GLfloat dt)
{
	if (this->State == GAME_MENU)
	{
		if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
		{
			this->State = GAME_ACTIVE;
			this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
		}
		if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W])
		{
			this->Level = (this->Level + 1) % 4;
			this->KeysProcessed[GLFW_KEY_W] = GL_TRUE;
		}
		if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S])
		{
			if (this->Level > 0)
				--this->Level;
			else
				this->Level = 3;
			this->KeysProcessed[GLFW_KEY_S] = GL_TRUE;
		}
	}
	if (this->State == GAME_ACTIVE)
	{
		GLfloat velocity = PLAYER_VELOCITY * dt;
		// move paddle
		if (this->Keys[GLFW_KEY_A])
		{
			if (player->Position.x >= 0) {
				player->Position.x -= velocity, PLAYER_POSITION.x = player->Position.x;
				if (ball->Stuck)
					ball->Position.x -= velocity;
			}
				
		}
		if (this->Keys[GLFW_KEY_D])
		{
			if (player->Position.x <= this->Width - player->Size.x) {
				player->Position.x += velocity, PLAYER_POSITION.x = player->Position.x;
				if (ball->Stuck)
					ball->Position.x += velocity;
			}
				
		}
		if (this->Keys[GLFW_KEY_SPACE])
			ball->Stuck = false;
	}
}

void Game::Update(GLfloat dt)
{
	//Question, when ball and brick collision check is true, should we still 
	//call ball->Move(dt, Width);
	//yeah, look, we reverse the ball->velocity(.x or .y)

	//check for collision
	this->doCollision();
	//update objects
	ball->Move(dt, Width);
	//update particles
	particles->Update(dt, *ball, 10, glm::vec2(ball->Radius / 2));
	//update PowerUps
	this->updatePowerUps(dt);
	// Reduce shake time
	if (ShakeTime > 0.0f)
	{
		ShakeTime -= dt;
		if (ShakeTime <= 0.0f)
			effects->Shake = GL_FALSE;
	}
	if (ball->Position.y >= Height) {
		--this->Lives;
		if (this->Lives == 0) {
			this->ResetLevel();
			this->State = GAME_MENU;
		}
		this->ResetPlayer();
	}
	if (this->State == GAME_ACTIVE && this->Levels[this->Level].IsComleted())
	{
		this->ResetLevel();
		this->ResetPlayer();
		effects->Chaos = GL_TRUE;
		this->State = GAME_WIN;
	}
	if (this->State == GAME_WIN)
	{
		if (this->Keys[GLFW_KEY_ENTER])
		{
			this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
			effects->Chaos = GL_FALSE;
			this->State = GAME_MENU;
		}
	}
}

void Game::Render(GLfloat dt)
{
	//render->DrawSprite(ResourceManager::GetTexture("face"),
		//glm::vec2(200, 200), glm::vec2(300, 400), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	if (State == GAME_ACTIVE || State == GAME_MENU) {
		// Begin rendering to postprocessing quad
		effects->BeginRender();
		//draw background
		render->DrawSprite(ResourceManager::GetTexture("background"),
			glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f
		);
		//draw level
		this->Levels[Level].Render(*render);
		//draw paddle
		player->Draw(*render);
		//PROBLEM:
		//particles have been drawn, but them can not be seen in game
		//when I can draw particles after ball, we can only see the particles(the screen is black, can not see the game display )
		//why?
		//because you do not active the particle shader

		//draw particle
		particles->Draw();
		//draw ball
		ball->Draw(*render);

		for (auto &powerup : PowerUps) {
			if (!powerup.Destroyed) {
				powerup.Draw(*render);
			}
		}

		std::stringstream ss; ss << this->Lives;
		Text->RenderText("Lives:" + ss.str(), 5.0f, 5.0f, 1.0f);

		Text->RenderText("FPS:" + std::to_string(static_cast<int>(1.0f / dt)), 700.0f, 10.0f, 1.0f);

		// End rendering to postprocessing quad
		effects->EndRender();
		effects->Render(glfwGetTime());
	}
	if (this->State == GAME_MENU)
	{
		Text->RenderText("Press ENTER to start", 250.0f, Height / 2, 1.0f);
		Text->RenderText("Press W or S to select level", 245.0f, Height / 2 + 20.0f, 0.75f);
	}
	if (this->State == GAME_WIN)
	{
		Text->RenderText(
			"You WON!!!", 320.0, Height / 2 - 20.0, 1.0, glm::vec3(0.0, 1.0, 0.0)
		);
		Text->RenderText(
			"Press ENTER to retry or ESC to quit", 130.0, Height / 2, 1.0, glm::vec3(1.0, 1.0, 0.0)
		);
	}
}

GLboolean Game::checkCollision(GameObject &one, GameObject &two)
{
	//collision X-asis?
	bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
		two.Position.x + two.Size.x >= one.Position.x;
	bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
		two.Position.y + two.Size.y >= one.Position.y;
	return collisionX && collisionY;
}
/*
GLboolean Game::checkCollision(BallObject &one, GameObject &two)
{
	//get center point circle first
	glm::vec2 center(one.Position + one.Radius);
	// Calculate AABB info (center, half-extents)
	glm::vec2 aabb_half_extents(two.Size.x / 2, two.Size.y /2);
	//glm::vec2 aabb_center(two.Position + aabb_half_extents);
	glm::vec2 aabb_center(
		two.Position.x + aabb_half_extents.x,
		two.Position.y + aabb_half_extents.y
	);

	// Get difference vector between both centers
	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
	// Add clamped value to AABB_center and we get the value of box closest to circle
	glm::vec2 closet = aabb_center + clamped;
	// Retrieve vector between center circle and closest point AABB and check if length <= radius
	difference = closet - center;
	return glm::length(difference) < one.Radius;
}*/

Collision Game::checkCollision(BallObject &one, GameObject &two)
{
	//get center point circle first
	glm::vec2 center(one.Position + one.Radius);
	// Calculate AABB info (center, half-extents)
	glm::vec2 aabb_half_extents(two.Size.x / 2, two.Size.y / 2);
	//glm::vec2 aabb_center(two.Position + aabb_half_extents);
	glm::vec2 aabb_center(
		two.Position.x + aabb_half_extents.x,
		two.Position.y + aabb_half_extents.y
	);

	// Get difference vector between both centers
	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
	// Add clamped value to AABB_center and we get the value of box closest to circle
	glm::vec2 closet = aabb_center + clamped;
	// Retrieve vector between center circle and closest point AABB and check if length <= radius
	difference = closet - center;
	if (glm::length(difference) <= one.Radius)
		return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);
	else
		return std::make_tuple(GL_FALSE, UP, glm::vec2(0.0f, 0.0f));
}

void Game::doCollision()
{
	for (auto &box : this->Levels[this->Level].Bricks) {
		if (!box.Destroyed) {
			Collision collision = checkCollision(*ball, box);
			if (std::get<0>(collision)) {//If collision is true
				//ball->Stuck = ball->Sticky;
				if (!box.IsSolid) {
					//Destroy block if not solid
					box.Destroyed = GL_TRUE;
					//spawn powerups
					this->spawnPowerUps(box);
					SoundEngine->play2D("audio/bleep.mp3", GL_FALSE);
				}
				else
				{   // if block is solid, enable shake effect
					ShakeTime = 0.1f;
					effects->Shake = GL_TRUE;
					SoundEngine->play2D("audio/solid.wav", GL_FALSE);
				}
				//Collision resolution
				Direction div = std::get<1>(collision);
				glm::vec2 diff_vector = std::get<2>(collision);
				if (!(ball->PassThrough && !box.IsSolid)) {//fix ball movement
					if (div == RIGHT || div == LEFT) {// Horizontal collision
						ball->Velocity.x = -ball->Velocity.x;// Reverse horizontal velocity
						// Relocate
						GLfloat penetration = ball->Radius - std::abs(diff_vector.x);
						if (div == LEFT)//fix
							ball->Position.x += penetration;
						else
							ball->Position.x -= penetration;
					}
					else {// Vertical collision
						//ball->Velocity.y = -ball->Velocity.y;// Reverse Vertical velocity
						ball->Velocity.y = -1 * std::abs(ball->Velocity.y);
						// Relocate
						GLfloat penetration = ball->Radius - std::abs(diff_vector.y);
						if (div == UP)
							ball->Position.y -= penetration;
						else
							ball->Position.y += penetration;

					}
				}
			}
				
		}
	}
	Collision result = checkCollision(*ball, *player);
	if (!ball->Stuck && std::get<0>(result)) {
		// Check where it hit the board, and change velocity based on where it hit the board
		GLfloat center_board = player->Position.x + player->Size.x / 2;
		GLfloat distance = ball->Position.x + ball->Radius - center_board;
		GLfloat percent = distance / (player->Size.x / 2);

		// Then move accordingly
		GLfloat strength = 2.0f;
		glm::vec2 oldVelocity = ball->Velocity;
		ball->Velocity.x = BALL_VELOCITY.x * percent * strength;
		ball->Velocity.y = -1 * std::abs(ball->Velocity.y);//fix   Aug 5, 
		ball->Velocity = glm::normalize(ball->Velocity) * glm::length(oldVelocity);
		SoundEngine->play2D("audio/bleep.wav", GL_FALSE);
	}
	//update powerups state
	for (auto &powerup : PowerUps) {
		if (!powerup.Destroyed) {
			if (powerup.Position.y >= this->Height)
				powerup.Destroyed = GL_TRUE;
			if (checkCollision(*player, powerup)) {
				//collided with player, now active powerup
				activePowerUp(powerup);
				powerup.Activated = GL_TRUE;
				powerup.Destroyed = GL_TRUE;
				SoundEngine->play2D("audio/powerup.wav", GL_FALSE);
			}
		}
	}
}

Direction Game::VectorDirection(glm::vec2 target)
{
	glm::vec2 compass[] = {
		glm::vec2(1.0f, 0.0f),//right
		glm::vec2(-1.0f, 0.0f), //left
		glm::vec2(0.0f, 1.0f),	//up
		glm::vec2(0.0f, -1.0f)	//down	
	};
	GLfloat max = 0.0f;
	GLuint best_match = -1;
	for (auto i = 0; i < 4; ++i) {
		auto dot = glm::dot(compass[i], glm::normalize(target));
		if (dot > max) {
			max = dot;
			best_match = i;
		}
	}
	return Direction(best_match);
}

void Game::spawnPowerUps(GameObject & block)
{
	if (shouldSpawnPowerUps(30))
		this->PowerUps.push_back(PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 2.0f, block.Position, ResourceManager::GetTexture("powerup_speed")));
	if (shouldSpawnPowerUps(30))
		this->PowerUps.push_back(PowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, block.Position, ResourceManager::GetTexture("powerup_sticky")));
	if (shouldSpawnPowerUps(30))
		this->PowerUps.push_back(PowerUp("pass-through", glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, block.Position, ResourceManager::GetTexture("powerup_passthrough")));
	if (shouldSpawnPowerUps(30))
		this->PowerUps.push_back(PowerUp("pad-size-increase", glm::vec3(1.0f, 0.6f, 0.4f), 0.8f, block.Position, ResourceManager::GetTexture("powerup_increase")));
	if (shouldSpawnPowerUps(15))
		this->PowerUps.push_back(PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 2.0f, block.Position, ResourceManager::GetTexture("powerup_confuse")));
	if (shouldSpawnPowerUps(15))
		this->PowerUps.push_back(PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 2.0f, block.Position, ResourceManager::GetTexture("powerup_chaos")));
}

void Game::updatePowerUps(GLfloat dt)
{
	for (auto &powerup : PowerUps) {
		powerup.Position += powerup.Velocity * dt;
		if (powerup.Activated) {
			powerup.Duration -= dt;
			//std::cout << powerup.Duration << "\n";
			if (powerup.Duration <= 0.0f) {
				//remove powerup from list
				powerup.Activated = GL_FALSE;
				//deactive effects
				if (powerup.Type == "sticky") {
					if (!isOtherPowerUpActive(this->PowerUps, "sticky")) {
						//only reset if no other PowerUp of type sticky is active
						ball->Stuck = GL_FALSE;
						player->Color = glm::vec3(1.0f);
					}
				}
				else if (powerup.Type == "pass-through") {
					if (!isOtherPowerUpActive(this->PowerUps, "pass-through")) {
						//only reset if no other PowerUp of type pass-through is active
						ball->PassThrough = GL_FALSE;
						ball->Color = glm::vec3(1.0f);
					}
				}
				else if (powerup.Type == "confuse") {
					if (!isOtherPowerUpActive(this->PowerUps, "confuse")) {
						//only reset if no other PowerUp of type confuse is active
						effects->Confuse = GL_FALSE;						
					}
				}
				else if (powerup.Type == "chaos") {
					if (!isOtherPowerUpActive(this->PowerUps, "chaos")) {
						//only reset if no other PowerUp of type chaos is active
						effects->Chaos = GL_FALSE;
						//std::cout << 00000000000 << "\n";
					}
				}
				else if (powerup.Type == "pad-size-increase") {
					if (!isOtherPowerUpActive(this->PowerUps, "pad-size-increase")) {
						player->Size -= 50;
					}
				}
			}
		}
	}
	this->PowerUps.erase(std::remove_if(PowerUps.begin(), PowerUps.end(),
		[](const PowerUp& powerup) { return powerup.Destroyed && !powerup.Activated; })
		, PowerUps.end());
}

void Game::ResetLevel()
{
	if (this->Level == 0) this->Levels[0].Load("levels/one.lvl", this->Width, this->Height * 0.5);
	else if(this->Level == 1) this->Levels[1].Load("levels/two.lvl", this->Width, this->Height * 0.5);
	else if(this->Level == 2) this->Levels[2].Load("levels/three.lvl", this->Width, this->Height * 0.5);
	else if(this->Level == 3) this->Levels[3].Load("levels/four.lvl", this->Width, this->Height * 0.5);
	this->Lives = 3;
}

void Game::ResetPlayer()
{
	first = true;
	// Reset player/ball stats
	player->Size = PLAYER_SIZE;
	player->Position = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
	ball->Reset(player->Position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), BALL_VELOCITY);
}

//non-member function
static GLboolean shouldSpawnPowerUps(GLuint chance) {
	auto random = rand() % chance;
	return random == 0;
}

//non-member function
void activePowerUp(PowerUp& powerup) {
	if (powerup.Type == "speed") {
		ball->Velocity *= 1.2;
	}
	
	else if (powerup.Type == "sticky") {
		ball->Stuck = GL_TRUE;
		player->Color = glm::vec3(1.0f, 0.5f, 1.0f);
	}
	else if (powerup.Type == "pass-through") {
		ball->PassThrough = GL_TRUE;
		ball->Color = glm::vec3(1.0f, 0.5f, 0.5f);
	}
	else if (powerup.Type == "pad-size-increase") {
		player->Size += 50;
	}
	else if (powerup.Type == "confuse") {
		if (!effects->Confuse)
			effects->Confuse = GL_TRUE;//only activate if confuse was not active
	}
	else if (powerup.Type == "chaos") {
		if (!effects->Chaos)
			effects->Chaos = GL_TRUE;
	}
}

//有问题，这个powerups包含比较者自身，也即一定返回true
GLboolean isOtherPowerUpActive(std::vector<PowerUp>& powerups, std::string type) {
	for (auto &e : powerups) {
		//修正
		if (e.Activated && e.Type == type)
			return GL_TRUE;
	}
	return GL_FALSE;
}