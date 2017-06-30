#include "triangle.h"

void triangle::draw(GraphicsContext* gcontext, ViewContext * vcontext)
{
	//convert to device coordinates
	matrix device(vcontext->modelToDevice(p1));
	gcontext->setColor(color);
	//draw the triangle
	for (int i = 0; i < 2; ++i)
	{
		int x0 = device[0][i];
		int y0 = device[1][i];
		int x1 = device[0][i+1];
		int y1 = device[1][i+1];
		gcontext->drawLine(x0,y0,x1,y1);
	}

	int x0 = device[0][2];
	int y0 = device[1][2];
	int x1 = device[0][0];
	int y1 = device[1][0];
	gcontext->drawLine(x0,y0,x1,y1);
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