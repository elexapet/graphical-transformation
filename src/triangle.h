/* 
* triangle.h
* 
* 01/10/16
* Petr Elexa
*
*/

#pragma once

#include "shape.h"
#include "line.h"

class triangle : public shape
{
public:
	triangle(uint color, uint x0, uint y0, uint x1, uint y1, uint x2, uint y2) 
	: shape(color, 3, 3)
	{
		p1[0][0] = x0;
		p1[1][0] = y0;
		p1[2][0] = 1.;
		//p1[3][0] = 1.;
		p1[0][1] = x1;
		p1[1][1] = y1;
		p1[2][1] = 1.;
		//p1[3][1] = 1.;
		p1[0][2] = x2;
		p1[1][2] = y2;
		p1[2][2] = 1.;
		//p1[3][2] = 1.;
	}
	virtual ~triangle() {}
	virtual void draw(GraphicsContext* gcontext, ViewContext * vcontext);
	virtual void out(std::ostream& os);
	virtual void in(std::istream& os);
	virtual shape* clone();
	virtual void setEndPoint(uint x, uint y);
	virtual void updateEndPoint(uint x, uint y);
	virtual bool done();
private:
};