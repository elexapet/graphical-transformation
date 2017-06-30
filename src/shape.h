/* 
* shape.h
* 
* 01/10/16
* Petr Elexa
*
*/

#pragma once

#include "matrix.h"
#include "gcontext.h"
#include "viewcontext.h"

#define uint unsigned int

class shape
{
public:
	shape(uint color, uint rows, uint cols) : color(color), p1(rows,cols)
	{
		
	}
	shape(const shape& other) : color(other.color), p1(other.p1) {}
	virtual ~shape() {}
	virtual void draw(GraphicsContext* gcontext, ViewContext * vcontext) = 0;
	virtual void out(std::ostream& os);
	virtual void in(std::istream& is);
	virtual shape* clone() = 0;
protected:
	uint color;
	matrix p1;

	shape& operator=(const shape& other);
};