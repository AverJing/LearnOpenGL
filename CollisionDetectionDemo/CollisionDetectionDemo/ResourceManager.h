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

#include <map>
#include <Shader.h>
#include <string>

class ResourceManager {
public:
	static std::map<std::string, Shader> Shaders;
	//static std::map<std::string, Circle> Circles;

	static void LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, std::string name);
	//static void LoadCircle(const Shader& shader, const glm::vec2 Position, const glm::vec2 Size, const glm::vec2 velocity, const float radius, const unsigned int n, std::string name);
	
	//static Circle& GetCircle(const std::string&);
	static Shader& GetShader(const std::string&);

	ResourceManager() = delete;
};
