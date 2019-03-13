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
#include "Base/Color.h"
#include "TypeDef.h"
class Vertex;
class Vertex2D;

class Rasterizer
{
public:
	Rasterizer(RenderContext* pcontext);
	~Rasterizer();

	void DrawPixel(int x, int y, Color& c);
	//void DrawOneLine(Lined2d *line, Color c);
private:
	RenderContext *context;
	RenderState renderState;
	Lined2d *line;
};
