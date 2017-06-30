#include "shape.h"

void shape::out(std::ostream& os)
{
	os << color << std::endl;
	os << p1;
}

void shape::in(std::istream& is)
{
	is >> color;
	is >> p1;
}

shape& shape::operator=(const shape& other)
{
	color = color;
	p1 = other.p1;

	return *this;
}