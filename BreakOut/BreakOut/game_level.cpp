#include "game_level.h"

#include <fstream>
#include <sstream>
#include <string>

void GameLevel::Load(const GLchar * file, GLuint levelWidth, GLuint levelHeight)
{
	this->Bricks.clear();

	std::ifstream ifile(file);
	std::string line;
	std::vector<std::vector<GLuint>> tileData;
	if (ifile) {
		while (std::getline(ifile, line)) {
			GLuint levelCode;
			std::vector<GLuint> row;
			std::istringstream ss(line);
			while (ss >> levelCode)
				row.push_back(levelCode);
			tileData.push_back(row);
		}
		if (!tileData.empty()) {
			this->Init(tileData, levelWidth, levelHeight);
		}
	}
}

void GameLevel::Render(SpriteRenderer & renderer)
{
	std::vector<glm::mat4> instanceMatrix;
	std::vector<glm::vec3> instanceColor;
	std::vector<GLuint> instanceTexture;
	for (auto& e : Bricks) {
		if (!e.Destroyed) {
			//e.Draw(renderer);
			glm::mat4 model(1.0f);
			model = glm::translate(model, glm::vec3(e.Position, 0.0f));
			model = glm::scale(model, glm::vec3(Size, 1.0f));
			instanceMatrix.push_back(model);
			instanceColor.push_back(e.Color);
			if(e.IsSolid)
				instanceTexture.push_back(1);
			else
				instanceTexture.push_back(0);
		}	
	}
	renderer.DrawSprite(instanceTexture, instanceMatrix, instanceColor);
	//note, when call vector.clear(), this function do not free memory, just let size()==0
	/*
	std::vector<glm::mat4> tempMatrix;
	std::vector<glm::vec3> tempColor;
	std::vector<GLuint> tempTexture;
	std::swap(tempMatrix, instanceMatrix);
	std::swap(tempColor, instanceColor);
	std::swap(tempTexture, instanceTexture);*/
	/*
	instanceTexture.erase(instanceTexture.begin());
	instanceColor.erase(instanceColor.begin());
	instanceMatrix.erase(instanceMatrix.begin());*/
}

GLboolean GameLevel::IsComleted()
{
	for (auto &e : this->Bricks) {
		if (!e.IsSolid && !e.Destroyed)
			return GL_FALSE;
	}
	return GL_TRUE;
}

/*
void GameLevel::Init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight)
{
	//calculate dimensions
	GLuint height = tileData.size();
	GLuint width = tileData[0].size();	//Notice: when we call Init(), it must be not empty. Because we check its size in Load(..) function
	GLfloat unit_height = levelHeight / static_cast<float>(height);
	GLfloat unit_width = levelWidth / static_cast<float>(width);

	//initialize level tiles based on tileData
	for (GLuint y = 0; y < height; ++y) {
		for (GLuint x = 0; x < width; ++x) {
			//check block type from tileData
			if (tileData[y][x] == 1) {//solid
				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				GameObject obj(pos, size, ResourceManager::GetTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
				obj.IsSolid = true;
				Bricks.push_back(obj);
			}
			else if (tileData[y][x] > 1) {//Non-solid: now determine its color based on level data
				glm::vec3 color(1.0f);	//original: white
				if (tileData[y][x] == 2)
					color = glm::vec3(0.2f, 0.6f, 1.0f);
				else if (tileData[y][x] == 3)
					color = glm::vec3(0.0f, 0.7f, 0.0f);
				else if (tileData[y][x] == 4)
					color = glm::vec3(0.8f, 0.8f, 0.4f);
				else if (tileData[y][x] == 5)
					color = glm::vec3(1.0f, 0.5f, 0.0f);
				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				//GameObject obj(pos, size, ResourceManager::GetTexture("block"), color);
				//Bricks.push_back(obj);
				Bricks.push_back(GameObject(pos, size, ResourceManager::GetTexture("block"), color));
			}
		}
	}
}

*/

void GameLevel::Init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight)
{
	//calculate dimensions
	GLuint height = tileData.size();
	GLuint width = tileData[0].size();	//Notice: when we call Init(), it must be not empty. Because we check its size in Load(..) function
	GLfloat unit_height = levelHeight / static_cast<float>(height);
	GLfloat unit_width = levelWidth / static_cast<float>(width);

	this->Size = glm::vec2(unit_width, unit_height);

	//initialize level tiles based on tileData
	for (GLuint y = 0; y < height; ++y) {
		for (GLuint x = 0; x < width; ++x) {
			//check block type from tileData
			if (tileData[y][x] == 1) {//solid
				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				GameObject obj(pos, size, ResourceManager::GetTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
				obj.IsSolid = true;
				Bricks.push_back(obj);
			}
			else if (tileData[y][x] > 1) {//Non-solid: now determine its color based on level data
				glm::vec3 color(1.0f);	//original: white
				if (tileData[y][x] == 2)
					color = glm::vec3(0.2f, 0.6f, 1.0f);
				else if (tileData[y][x] == 3)
					color = glm::vec3(0.0f, 0.7f, 0.0f);
				else if (tileData[y][x] == 4)
					color = glm::vec3(0.8f, 0.8f, 0.4f);
				else if (tileData[y][x] == 5)
					color = glm::vec3(1.0f, 0.5f, 0.0f);
				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				//GameObject obj(pos, size, ResourceManager::GetTexture("block"), color);
				//Bricks.push_back(obj);
				Bricks.push_back(GameObject(pos, size, ResourceManager::GetTexture("block"), color));
			}
		}
	}
}
