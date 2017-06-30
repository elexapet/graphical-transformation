/* 
* line.h
* 
* 01/10/16
* Petr Elexa
*
*/

#pragma once

#include "shape.h"
#include <utility>

class line : public shape
{
public:
	line(uint color, uint x0, uint y0, uint x1, uint y1) 
	: shape(color, 3, 2)
	{	
		p1[0][0] = x0;
		p1[1][0] = y0;
		p1[2][0] = 1.;
		//p1[3][0] = 1.0;
		p1[0][1] = x1;
		p1[1][1] = y1;
		p1[2][1] = 1.;
		//p1[3][1] = 1.0;
	}
	virtual ~line() {}
	virtual void draw(GraphicsContext* gcontext, ViewContext * vcontext);
	virtual void out(std::ostream& os);
	virtual void in(std::istream& is);
	virtual shape* clone();
	virtual void setEndPoint(uint x, uint y);
	virtual void updateEndPoint(uint x, uint y);
	virtual bool done();
};