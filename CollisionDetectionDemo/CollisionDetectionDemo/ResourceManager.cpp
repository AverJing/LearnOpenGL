#include "ResourceManager.h"

// Instantiate static variables
std::map<std::string, Shader> Shaders;
//std::map<std::string, Circle> Circles;

void ResourceManager::LoadShader(const GLchar * vShaderFile, const GLchar * fShaderFile, std::string name)
{
	Shaders.emplace(name, Shader(vShaderFile, fShaderFile));
}
/*
void ResourceManager::LoadCircle(const Shader & shader, const glm::vec2 Position, const glm::vec2 Size, const glm::vec2 velocity, const float radius, const unsigned int n, std::string name)
{
	Circles.emplace(name, Circle(shader, Position, Size, velocity, radius, n));
}

Circle & ResourceManager::GetCircle(const std::string &name)
{
	return Circles[name];
}*/

Shader & ResourceManager::GetShader(const std::string& name)
{
	return Shaders[name];
}
