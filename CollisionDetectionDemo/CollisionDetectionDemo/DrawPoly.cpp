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
#include<iostream>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Shader.h>
#include <tuple>
#include <memory>
#include <algorithm>
#include <cmath>
#include "stb_image_impl.h"
#include "Circle.h"
#include "Polygon.h"
//#include "ResourceManager.h"

#include <vector>

//setting
const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

//function declaration
void frameBuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

enum Direction {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

using Collision = std::tuple<GLboolean, glm::vec2, float>;

template<typename T1, typename T2>
void doCollision(const Collision&, const std::shared_ptr<T1>, const std::shared_ptr<T2>);
Collision checkCollision(const Circle& one, const Circle& two);
Collision checkCollision(const Circle& one, const Polygons& two);
Direction VectorDirection(glm::vec2 target);


std::shared_ptr<Shader> ourShader;
std::shared_ptr<Circle> circle1;
std::shared_ptr<Circle> circle2;
std::shared_ptr<Polygons> poly1;
std::shared_ptr<Polygons> poly2;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);													 // glfw window creation
														 // --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, frameBuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader zprogram
	// ------------------------------------

	//ResourceManager::LoadShader("shaders/collision.vs", "shaders/collision.fs", "ourShader");
	ourShader = std::make_shared<Shader>("shaders/collision.vs", "shaders/collision.fs");

	circle1 = std::make_shared<Circle>(*ourShader,
		glm::vec2(100.0f, 100.0f), glm::vec2(50.0f, 50.0f), glm::vec2(50.0f, 50.0f), 1, 50);
	circle2 = std::make_shared<Circle>(*ourShader,
		glm::vec2(200.0f, 300.0f), glm::vec2(75.0f, 75.0f), glm::vec2(70.0f, -60.0f), 1, 50);
	std::vector<glm::vec3> poly_vertex1 = {
		{0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
		{0.8f, 0.5f, 0.0f},
		{0.3f, 1.0f, 0.0f}
	};
	std::vector<glm::vec3> poly_vertex2 = {
		{0.2f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
		{0.8f, 0.0f, 0.0f},
		{1.0f, 1.0f, 0.0f}
	};


	poly1 = std::make_shared<Polygons>(*ourShader, glm::vec2(450.0f, 150.0f),
		glm::vec2(80.0f, 80.0f), glm::vec2(60.0f, -50.0f), poly_vertex1);
	poly2 = std::make_shared<Polygons>(*ourShader, glm::vec2(600.0f, 300.0f),
		glm::vec2(180.0f, 180.0f), glm::vec2(30.0f, 60.0f), poly_vertex2);

	circle1->Init();
	circle2->Init();
	poly1->Init();
	poly2->Init();
	// render loop
	// -----------

	GLfloat deltaTime = 0.0f;
	GLfloat lastTime = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastTime;
		lastTime = deltaTime;

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		doCollision(checkCollision(*circle1, *circle2), circle1, circle2);
		//doCollision(checkCollision(*circle1, *poly1), circle1, poly1);
		//doCollision(checkCollision(*circle1, *poly2), circle1, poly2);
		//doCollision(checkCollision(*circle2, *poly1), circle2, poly1);
		//doCollision(checkCollision(*circle2, *poly2), circle2, poly2);

		//update info
		circle1->Move(deltaTime);
		circle2->Move(deltaTime);
		poly1->Move(deltaTime);
		poly2->Move(deltaTime);

		// render container
		circle1->Draw();
		circle2->Draw();
		poly1->Draw();
		poly2->Draw();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void frameBuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

Collision checkCollision(const Circle& one, const Circle& two) {
	GLfloat difference = (one.Position.x - two.Position.x) * (one.Position.x - two.Position.x)
		+ (one.Position.y - two.Position.y) * (one.Position.y - two.Position.y);
	if (difference <= (one.Size.x + two.Size.x) * (one.Size.x + two.Size.x))
		return std::make_tuple(GL_TRUE, one.Position - two.Position, (one.Size.x + two.Size.x - difference)*0.001);
	else 
		return std::make_tuple(GL_FALSE, glm::vec2(0.0f, 0.0f), 0.0F);
}

Collision checkCollision(const Circle& one, const Polygons& two) {
	std::vector<glm::vec2> points;
	for (auto& e : two.vertices) {
		points.push_back(glm::vec2(e.x * two.Size.x + two.Position.x, 
			e.y * two.Size.y + two.Position.y));
	}
	GLfloat length_min = FLT_MAX; //set FLOAT_MAX is better. 
	GLfloat length_max = FLT_MIN;
	glm::vec2 closet;
	glm::vec2 farest;
	for (auto &e : points) {
		auto tmp = std::pow((one.Position.x - e.x), 2) + std::pow((one.Position.y - e.y), 2);
		if (tmp < length_min) {
			length_min = tmp;
			closet = e;
		}
		if (tmp > length_max) {
			length_max = tmp;
			farest = e;
		}
	}
	auto axes = glm::normalize(closet - one.Position);
	auto poly_min = FLT_MAX;
	auto poly_max = FLT_MIN;
	//find the min and max projection
	for (auto& e : points) {
		auto tmp = glm::dot(e, axes);
		if (tmp < poly_min) poly_min = tmp;
		if (tmp > poly_max) poly_max = tmp;
	}
	auto circle_projection = glm::dot(one.Position, axes);
	auto circle_min = circle_projection - one.Size.x;
	auto circle_max = circle_projection + one.Size.x;

	if (circle_min < poly_max && circle_max > poly_min)
		return std::make_tuple(GL_TRUE, one.Position - closet,
			std::min(circle_max, poly_max) - std::max(poly_min, circle_min));
	else
		return std::make_tuple(GL_FALSE, glm::vec2(0.0f, 0.0f), 0.0f);
}

Collision checkCollision(const Polygons& one, const Polygons& two) {
	std::vector<glm::vec2> points_one;
	for (auto& e : one.vertices) {
		points_one.push_back(glm::vec2(e.x * one.Size.x + one.Position.x,
			e.y * one.Size.y + one.Position.y));
	}
	std::vector<glm::vec2> points_two;
	for (auto& e : two.vertices) {
		points_one.push_back(glm::vec2(e.x * two.Size.x + two.Position.x,
			e.y * two.Size.y + two.Position.y));
	}
	GLfloat length_min = FLT_MAX; //set FLOAT_MAX is better. 
	GLfloat length_max = FLT_MIN;
	glm::vec2 closet;
	glm::vec2 farest;
	for (auto &e : points) {
		auto tmp = std::pow((one.Position.x - e.x), 2) + std::pow((one.Position.y - e.y), 2);
		if (tmp < length_min) {
			length_min = tmp;
			closet = e;
		}
		if (tmp > length_max) {
			length_max = tmp;
			farest = e;
		}
	}
	auto axes = glm::normalize(closet - one.Position);
	auto poly_min = FLT_MAX;
	auto poly_max = FLT_MIN;
	//find the min and max projection
	for (auto& e : points) {
		auto tmp = glm::dot(e, axes);
		if (tmp < poly_min) poly_min = tmp;
		if (tmp > poly_max) poly_max = tmp;
	}
	auto circle_projection = glm::dot(one.Position, axes);
	auto circle_min = circle_projection - one.Size.x;
	auto circle_max = circle_projection + one.Size.x;

	if (circle_min < poly_max && circle_max > poly_min)
		return std::make_tuple(GL_TRUE, one.Position - closet,
			std::min(circle_max, poly_max) - std::max(poly_min, circle_min));
	else
		return std::make_tuple(GL_FALSE, glm::vec2(0.0f, 0.0f), 0.0f);
}

template<typename T1, typename T2>
void doCollision(const Collision& collision, const std::shared_ptr<T1> one, const std::shared_ptr<T2> two) {
	//collision detection for circles
	//Collision collision = checkCollision(*circle1, *circle2);
	if (std::get<0>(collision)) {
		auto axis = std::get<1>(collision);
		Direction div = VectorDirection(axis);
		float difference = std::get<2>(collision);
		/*
		Projection.prototype.overlap = function(projection) {
		return Math.min(this.max, projection.max) - Math.max(this.min, projection.min);
	};

		 axis: (new Vector(c1.x - c2.x, c1.y - c2.y)).normalize(),
			overlap: c1.radius + c2.radius - distace

		dX = mtv.axis.x * mtv.overlap;
        dY = mtv.axis.y * mtv.overlap;
        if (dX * velocity.x > 0) dX = -dX;
        if (dY * velocity.y > 0) dY = -dY;
		*/
		if (div == RIGHT || div == LEFT) {
			one->Velocity.x = -one->Velocity.x;
			two->Velocity.x = -two->Velocity.x;

			GLfloat penetration = difference;
			if (div == LEFT) {
				one->Position.x -= penetration;
				two->Position.x += penetration;
			}
			else {
				one->Position.x += penetration;
				two->Position.x -= penetration;
			}
		}
		else {
			one->Velocity.y = -one->Velocity.y;
			two->Velocity.y = -two->Velocity.y;
			GLfloat penetration = difference;
			if (div == UP) {
				one->Position.y -= penetration;
				two->Position.y += penetration;
			}
			else {
				one->Position.y += penetration;
				two->Position.y -= penetration;
			}
		}
	}
	//collision detection for circle and polygons
	//Collision result = checkCollision(*circle1)
}


Direction VectorDirection(glm::vec2 target)
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