#include "triangle.h"

void triangle::draw(GraphicsContext* gcontext, ViewContext * vcontext)
{
	line(color, p1[0][0], p1[1][0], p1[0][1], p1[1][1]).draw(gcontext, vcontext);
	line(color, p1[0][1], p1[1][1], p1[0][2], p1[1][2]).draw(gcontext, vcontext);
	if ((p1[0][1] != p1[0][2]) || (p1[1][1] != p1[1][2])){
		line(color, p1[0][2], p1[1][2], p1[0][0], p1[1][0]).draw(gcontext, vcontext);
	}
}	

void triangle::out(std::ostream& os)
{
	os << "begin triangle" << std::endl;
	shape::out(os);
	os << std::endl;
}

void triangle::in(std::istream& is)
{
	shape::in(is);
}

shape* triangle::clone()
{
	return new triangle(*this);
}

void triangle::setEndPoint(uint x, uint y)
{
	if (done()) return;
	if (curEndPoint < 3)
	{
		p1[0][curEndPoint] = x;
		p1[1][curEndPoint] = y;
	}
	curEndPoint++;
}

void triangle::updateEndPoint(uint x, uint y)
{
	if (done()) return;
	p1[0][curEndPoint] = x;
	p1[1][curEndPoint] = y;
	p1[0][2] = x;
	p1[1][2] = y;
}

bool triangle::done()
{
	return curEndPoint >= 3;
}