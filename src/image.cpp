#include "image.h"

image::~image()
{
	erase();
}

image::image(const image& other)
{
	for(shape* someShape : other.myShapes)
	{
		myShapes.push_back(someShape->clone());
	}
}

image& image::operator=(const image& other)
{
	if (this == &other) return *this;
	
	erase();
	for(shape* someShape : other.myShapes)
	{
		myShapes.push_back(someShape->clone());
	}

	return *this;
}

void image::add(shape* someShape)
{
	myShapes.push_back(someShape);
}

void image::draw(GraphicsContext* gcontext, ViewContext * vcontext)
{
	for(shape* someShape : myShapes)
	{
		someShape->draw(gcontext, vcontext);
	}
}

void image::out(std::ostream& os)
{
	for(shape* someShape : myShapes)
	{
		someShape->out(os);
	}
}

bool image::in(std::istream& is)
{
	std::string cmd;
	shape* someShape;

	erase();

	while(is.good())
	{
		is >> cmd;
		if (cmd != "begin"){
			break;
		}
		is >> cmd;
		if (cmd == "line"){
			someShape = new line(-1,0,0,0,0);
		}
		else if (cmd == "triangle"){
			someShape = new triangle(-1,0,0,0,0,0,0);
		}
		else{
			is.setstate (std::ios::failbit);
			return false;
		}
		someShape->in(is);
		if (is.fail()){	
			delete someShape;
			return false;
		}
		myShapes.push_back(someShape);
	}
	return true;
}

void image::erase()
{
	for(shape* someShape : myShapes)
	{
		delete someShape;
	}
	myShapes.clear();
}