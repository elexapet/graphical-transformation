#include "line.h"

void line::draw(GraphicsContext* gcontext, ViewContext * vcontext)
{
	matrix device(vcontext->modelToDevice(p1));
	uint x0 = device[0][0];
	uint y0 = device[1][0];
	uint x1 = device[0][1];
	uint y1 = device[1][1];

	gcontext->setColor(color);
	gcontext->drawLine(x0,y0,x1,y1);
}

void line::out(std::ostream& os)
{
	os << "begin line" << std::endl;
	shape::out(os);
	os << std::endl;
}

void line::in(std::istream& is)
{
	shape::in(is);
}

shape* line::clone()
{
	return new line(*this);
}

void line::setEndPoint(uint x, uint y)
{
	if (done()) return;
	if (curEndPoint < 2)
	{
		p1[0][curEndPoint] = x;
		p1[1][curEndPoint] = y;
	}
	curEndPoint++;
}

void line::updateEndPoint(uint x, uint y)
{
	if (done()) return;
	p1[0][curEndPoint] = x;
	p1[1][curEndPoint] = y;
}

bool line::done()
{
	return curEndPoint >= 2;
}