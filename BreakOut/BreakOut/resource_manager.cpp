#include "resource_manager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include <stb_image.h>

// Instantiate static variables
std::map<std::string, Shader>       ResourceManager::Shaders;
std::map<std::string, Texture2D>    ResourceManager::Textures;


Shader& ResourceManager::LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name)
{
	//line 17 is wrong code	//why ?
	//Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
	Shaders.emplace(std::make_pair(name, loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile)));
	//Shaders[name] = Shader(vShaderFile, fShaderFile);
	return Shaders[name];
}

Shader& ResourceManager::GetShader(std::string name)
{
	return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const GLchar *file, GLboolean alpha, std::string name)
{
	//Textures[name] = loadTextureFromFile(file, alpha);
	Textures.emplace(std::make_pair(name, loadTextureFromFile(file, alpha)));
	return Textures[name];
}

Texture2D ResourceManager::GetTexture(std::string name)
{
	return Textures[name];
}

void ResourceManager::Clear()
{
	// (Properly) delete all shaders	
	for (auto iter : Shaders)
		glDeleteProgram(iter.second.ID);
	// (Properly) delete all textures
	for (auto iter : Textures)
		glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile)
{
	/*
	// 1. Retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	try
	{
		// Open files
		std::ifstream vertexShaderFile(vShaderFile);
		std::ifstream fragmentShaderFile(fShaderFile);
		std::stringstream vShaderStream, fShaderStream;
		// Read file's buffer contents into streams
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		// close file handlers
		vertexShaderFile.close();
		fragmentShaderFile.close();
		// Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		// If geometry shader path is present, also load a geometry shader
		if (gShaderFile != nullptr)
		{
			std::ifstream geometryShaderFile(gShaderFile);
			std::stringstream gShaderStream;
			gShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::exception e)
	{
		std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
	}
	const GLchar *vShaderCode = vertexCode.c_str();
	const GLchar *fShaderCode = fragmentCode.c_str();
	const GLchar *gShaderCode = geometryCode.c_str();
	// 2. Now create shader object from source code
	Shader shader;
	shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
	return shader;*/
	
	if (gShaderFile != nullptr)
		return Shader(vShaderFile, fShaderFile, gShaderFile);
	else
		return Shader(vShaderFile, fShaderFile);
}

Texture2D ResourceManager::loadTextureFromFile(const GLchar *file, GLboolean alpha)
{
	// Create Texture object
	Texture2D texture;
	if (alpha)
	{
		texture.Internal_Format = GL_RGBA;
		texture.Image_Format = GL_RGBA;
	}
	// Load image
	int width, height, ncChannels;
	unsigned char* image = stbi_load(file, &width, &height, &ncChannels, 0);
	
	if (ncChannels == 1)// single color
		texture.Internal_Format = GL_RED, texture.Image_Format = GL_RED;
	else if (ncChannels == 3)//jpg
		texture.Internal_Format = GL_RGB, texture.Image_Format = GL_RGB;
	else if (ncChannels == 4)//png
		texture.Internal_Format = GL_RGBA, texture.Image_Format = GL_RGBA;
	
	// Now generate texture
	texture.Generate(width, height, image, texture.Image_Format);
	// And finally free image data
	stbi_image_free(image);
	return texture;
}
