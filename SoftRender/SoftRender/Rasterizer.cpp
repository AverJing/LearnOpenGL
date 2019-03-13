#include "Rasterizer.h"

Rasterizer::Rasterizer(RenderContext * pcontext):context(pcontext)
{
}

Rasterizer::~Rasterizer()
{
}

void Rasterizer::DrawPixel(int x, int y, Color & c)
{
	context->backBuffer[x + y * context->width] = (c.GetUintA() << 24) |
		(c.GetUintB() << 16) | (c.GetUintG() << 8) | (c.GetUintR() << 0);
}
