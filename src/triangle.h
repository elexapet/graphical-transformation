/* 
* triangle.h
* 
* 01/10/16
* Petr Elexa
*
*/

#pragma once

#include "shape.h"

class triangle : public shape
{
public:
	triangle(uint color, double x0, double y0, double z0, double x1, double y1, double z1, double x2, double y2, double z2) 
	: shape(color, 4, 3)
	{
		p1[0][0] = x0;
		p1[1][0] = y0;
		p1[2][0] = z0;
		p1[3][0] = 1.;
		p1[0][1] = x1;
		p1[1][1] = y1;
		p1[2][1] = z1;
		p1[3][1] = 1.;
		p1[0][2] = x2;
		p1[1][2] = y2;
		p1[2][2] = z2;
		p1[3][2] = 1.;
	}
	virtual ~triangle() {}
	virtual void draw(GraphicsContext* gcontext, ViewContext * vcontext);
	virtual void out(std::ostream& os);
	virtual void in(std::istream& os);
	virtual shape* clone();
private:
};