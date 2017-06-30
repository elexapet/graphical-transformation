/* 
* image.h
* 
* 01/10/16
* Petr Elexa
*
*/

#pragma once

#include "shape.h"
#include "line.h"
#include "triangle.h"
#include "gcontext.h"
#include <vector>
#include <string>

class image
{
public:
	image() {}
	image(const image& other);
	~image();
	image& operator=(const image& other);
	void add(shape* someShape);
	void draw(GraphicsContext* gcontext, ViewContext * vcontext);
	void out(std::ostream& os);
	bool in(std::istream& is);
	void erase();
private:
	std::vector<shape*> myShapes;
};
