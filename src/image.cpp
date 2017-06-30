#include "image.h"

//parsing states
#define START 0
#define IN_MODEL 1
#define IN_FACET 2
#define IN_LOOP 3

//tokens
#define LOOP_START "outer loop"
#define LOOP_END "endloop"
#define FACET_START "facet normal"
#define FACET_END "endfacet"
#define SOLID_START "solid "
#define SOLID_END "endsolid"
#define VERTEX_KEY "vertex"


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

void image::eatWhiteSpaces(std::istream& fileStream)
{
	//eat whitespaces
	char c;
	do{ 
		c = fileStream.get();
	}while (c == ' ' || c == '\t' || c == '\n');

	fileStream.unget();
}

int image::parseSTL(std::istream& fileStream)
{
	std::string currentLine;

	int currentState = START;

	int curVertex = 0;
	double x[3];
	double y[3];
	double z[3];

	while (getline(fileStream, currentLine)){
		switch (currentState){
			case START:
				//begin solid?
				if (!currentLine.compare(0, sizeof SOLID_START-1, SOLID_START)){
					currentState = IN_MODEL;
					break;
				}else{
					return 1;
				}
			case IN_MODEL:
				//begin facet?
				if (!currentLine.compare(0, sizeof FACET_START-1, FACET_START)){
					currentState = IN_FACET;
				}
				//end solid?
				if (!currentLine.compare(0, sizeof SOLID_END-1, SOLID_END)){
					return 0;
				}
				break;
			case IN_FACET:
				//begin loop?
				if (!currentLine.compare(0, sizeof LOOP_START-1, LOOP_START)){
					currentState = IN_LOOP;
				}
				//end facet?
				if (!currentLine.compare(0, sizeof FACET_END-1, FACET_END)){
					currentState = IN_MODEL;
				}
				break;
			case IN_LOOP:
				//is vertex line?
				if (!currentLine.compare(0, sizeof VERTEX_KEY-1, VERTEX_KEY)){
					//read coordinates
					std::istringstream ssLine(currentLine.substr(sizeof VERTEX_KEY,
						 						currentLine.size()));
					ssLine >> x[curVertex] >> std::skipws 
						>> y[curVertex] >> std::skipws 
						>> z[curVertex];

					curVertex++;
				}
				//end loop?
				if (!currentLine.compare(0, sizeof LOOP_END-1, LOOP_END)){
					currentState = IN_FACET;
					//add triangle to image
					myShapes.push_back(
						new triangle(-1,x[0],y[0],z[0],x[1],y[1],z[1],x[2],y[2],z[2]));
					curVertex = 0;
				}
				break;
			default:
				return 1;
		}
		eatWhiteSpaces(fileStream);
	}
	return 1;
}

bool image::in(std::istream& is)
{
	erase();

	if (parseSTL(is))
	{
		erase();
		return false;
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