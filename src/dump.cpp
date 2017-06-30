/* 
* Lab assigment #7
* 3D transformations
* 
* main.cpp
* 
* controls:
* q = -10 FOV
* w = +10 FOV
* r = reset
* l = load image.stl
* n = clear screen
* , = rotate(-) around x 
* . = rotate(+) around x 
* ; = rotate(-) around y
* ' = rotate(+) around y
* [ = rotate(-) around z
* ] = rotate(+) around z
* - = scale down
* + = scale up
* and mouse
*/

#include "triangle.h"
#include "image.h"
#include "x11context.h"
#include "mydrawing.h"
#include "viewcontext.h"
#include <fstream>
#include <unistd.h>


void modelViewer()
{
	GraphicsContext* gc = new X11Context(800, 600, GraphicsContext::BLACK);
	MyDrawing md(gc);

	gc->runLoop(&md);

	delete gc;
}

int main(int argc, char const *argv[])
{
	modelViewer();	
	return 0;
}



/* 
* viewcontext.h
* 
* 01/31/17
* Petr Elexa
*
*/


#pragma once

#include "matrix.h"
#include <cmath>
#include <vector>

class ViewContext
{
public:
	//constructor
	ViewContext(int xOrigin, int yOrigin) : 
		_compTrans(matrix::identity(MatrixSize)),
		_compTrans3D(matrix::identity(MatrixSize)),
		_perspTrans(matrix::identity(MatrixSize)),
		_projTrans(matrix::identity(MatrixSize)),
		_pointOrigin(4,1),
		_pointOriginDef(4,1),
		_pointRef(4,1),
		_xOrigin(xOrigin), _yOrigin(yOrigin)
	{
		reset();
	}
	//convert device coordinate to model
	matrix modelToDevice(matrix& model);
	//horizontal and vertical orbiting
	void hvOrbit(int horizontal, int vertical);
	//3D rotation
	void rotate(double angleX, double angleY, double angleZ);
	//change field of view
	void FOV(double scale);
	//2D scaling
	void scale(double scale);
	//3D model translation
	void translate(int x, int y, int z);
	void reset();
private:
	const static int MatrixSize = 4;
	//composite transformation matrices
	matrix _compTrans;
	matrix _compTrans3D;
	matrix _perspTrans;
	matrix _projTrans;
	//transformation parameters
	double _angleX;
	double _angleY;
	double _angleZ;
	double _scale;
	double _scale3D;
	int _x;
	int _y;
	int _z;
	double _fov;
	matrix _pointOrigin;
	matrix _pointOriginDef; //default
	matrix _pointRef;
	//device origin
	int _xOrigin;
	int _yOrigin;

	void updateProjection();
	void updatePerspective();
	void updateComposite3D();
	void updateComposite();
};



#include "viewcontext.h"

matrix ViewContext::modelToDevice(matrix& model)
{
	//apply all transformations
	matrix device = _projTrans * _compTrans3D * model;
	device = _perspTrans * device;
	for (int i = 0; i < 3; ++i)
	{
		//normalize
		device[0][i] /= device[3][i];
		device[1][i] /= device[3][i];
		device[2][i] /= device[3][i];
		device[3][i] = 1;
	}
	return _compTrans * device;
}

void ViewContext::hvOrbit(int horizontal, int vertical)
{
	double angle;
	//canonical rotation
	matrix rotationY(matrix::identity(4));

	angle = horizontal / 100.;

	rotationY[0][0] = cos(angle);
	rotationY[0][2] = -sin(angle);
	rotationY[2][0] = sin(angle);
	rotationY[2][2] = cos(angle);

	matrix rotationX(matrix::identity(4));

	angle = vertical / 100.;

	rotationX[1][1] = cos(angle);
	rotationX[1][2] = sin(angle);
	rotationX[2][1] = -sin(angle);
	rotationX[2][2] = cos(angle);

	_pointOrigin = rotationY * rotationX * _pointOriginDef;

	updateProjection();
}

void ViewContext::rotate(double angleX, double angleY, double angleZ)
{
	//convert to radians
	_angleX += angleX / 57.2958;
	if (_angleX > 6.28) _angleX -= 6.28;
	if (_angleX < 6.28) _angleX += 6.28;

	_angleY += angleY / 57.2958;
	if (_angleY > 6.28) _angleY -= 6.28;
	if (_angleY < 6.28) _angleY += 6.28;

	_angleZ += angleZ / 57.2958;
	if (_angleZ > 6.28) _angleZ -= 6.28;
	if (_angleZ < 6.28) _angleZ += 6.28;

	updateComposite3D();
}

void ViewContext::FOV(double angle)
{
	_fov += angle;
	if (_fov <= 0) _fov = 1;
	if (_fov > 180) _fov = 180;

	updatePerspective();
}

void ViewContext::scale(double scale)
{
	_scale *= scale;
	updateComposite();
}

void ViewContext::translate(int x, int y, int z)
{
	_x += x;
	_y += y;
	_z += z;
	updateComposite3D();
}

void ViewContext::reset()
{
	_angleX = 0;
	_angleY = 0;
	_angleZ = 0;//-1.57;
	_scale = 4;
	_scale3D = 4;
	_x = 0;
	_y = 0;
	_z = 0;
	_fov = 90;
	_pointOrigin[0][0] = 0;
	_pointOrigin[1][0] = 0;
	_pointOrigin[2][0] = 25;
	_pointOrigin[3][0] = 1;
	_pointOriginDef = _pointOrigin;

	updateComposite3D();
	updateComposite();
	updatePerspective();
	updateProjection();
}

void ViewContext::updateProjection()
{
	matrix rotation(matrix::identity(4));
	matrix translation(matrix::identity(4));
	matrix vectorV(3,1);
	matrix vectorU(3,1);

	//calculate vector N
	matrix vectorN = _pointOrigin - _pointRef;
	
	//normalize vector N
	vectorN = vectorN * (1 / sqrt(vectorN[0][0] * vectorN[0][0] 
	+ vectorN[1][0] * vectorN[1][0]
	+ vectorN[2][0] * vectorN[2][0]));

	vectorV[1][0] = 1;

	//simplified cross product (assuming V = (0,1,0))
	vectorU[0][0] = vectorN[2][0];
	vectorU[1][0] = 0;
	vectorU[2][0] = -vectorN[0][0];

	//normalize vector U
	vectorU = vectorU * (1 / sqrt(vectorU[0][0] * vectorU[0][0] 
	+ vectorU[1][0] * vectorU[1][0]
	+ vectorU[2][0] * vectorU[2][0]));

	//fill in matrices

	translation[0][3] = -_pointOrigin[0][0];
	translation[1][3] = -_pointOrigin[1][0];
	translation[2][3] = -_pointOrigin[2][0];

	rotation[0][0] = vectorU[0][0];
	rotation[0][1] = vectorU[1][0];
	rotation[0][2] = vectorU[2][0];
	
	rotation[1][0] = vectorV[0][0];
	rotation[1][1] = vectorV[1][0];
	rotation[1][2] = vectorV[2][0];

	rotation[2][0] = vectorN[0][0];
	rotation[2][1] = vectorN[1][0];
	rotation[2][2] = vectorN[2][0];

	//calculate projection matrix
	_projTrans = rotation * translation;
}

void ViewContext::updatePerspective()
{
	//scale from FOV angle
	double scale = 1.0 / tan((_fov * 3.14 * 0.5) / 180.0);
	double near = 0.1; 
    double far = 100; 
    //create perspective matrix
	_perspTrans[0][0] = scale;
	_perspTrans[1][1] = scale;
	_perspTrans[2][2] = -far / (far - near);
	_perspTrans[3][2] = (far * near) / (far - near);
	_perspTrans[2][3] = -1;
	_perspTrans[3][3] = 0;
}

void ViewContext::updateComposite3D()
{
	matrix rotationX(matrix::identity(4));
	matrix rotationY(matrix::identity(4));
	matrix rotationZ(matrix::identity(4));
	matrix translation(matrix::identity(4));
	matrix scaling(matrix::identity(4));

	//rotation for x axis
	rotationX[1][1] = cos(_angleX);
	rotationX[1][2] = -sin(_angleX);
	rotationX[2][1] = sin(_angleX);
	rotationX[2][2] = cos(_angleX);

	//rotation for y axis
	rotationY[0][0] = cos(_angleY);
	rotationY[0][2] = sin(_angleY);
	rotationY[2][0] = -sin(_angleY);
	rotationY[2][2] = cos(_angleY);

	//rotation for z axis
	rotationZ[0][0] = cos(_angleZ);
	rotationZ[0][1] = -sin(_angleZ);
	rotationZ[1][0] = sin(_angleZ);
	rotationZ[1][1] = cos(_angleZ);

	//translation matrix
	translation[0][3] = _x;
	translation[1][3] = _y;
	translation[2][3] = _z;

	//scaling matrix
	scaling[0][0] = _scale3D;
	scaling[1][1] = _scale3D;
	scaling[2][2] = _scale3D;

	//calculate composite 3D transformation
	_compTrans3D = translation * scaling * rotationZ * rotationY * rotationX;
}	

void ViewContext::updateComposite()
{
	matrix toCenter(matrix::identity(4));
	matrix scaling(matrix::identity(4));
	matrix flip(matrix::identity(4));

	flip[1][1] = -1;

	toCenter[0][3] = _xOrigin;
	toCenter[1][3] = _yOrigin;

	scaling[0][0] = _scale;
	scaling[1][1] = _scale;
	scaling[2][2] = 0;

	//calculate composite 2D transformation
	_compTrans = toCenter * flip * scaling;
}


/* 
* image.h
* 
* 01/10/16
* Petr Elexa
*
*/

#pragma once

#include "shape.h"
#include "triangle.h"
#include "gcontext.h"
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

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

	void eatWhiteSpaces(std::istream& fileStream);
	int parseSTL(std::istream& fileStream);

};


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


/* 
* mydrawing.h
* 
* 01/31/17
* Petr Elexa
*
*/

#pragma once

#include "drawbase.h"
#include "image.h"
#include "viewcontext.h"
#include <fstream>

class MyDrawing : public DrawingBase
{
public:
	//Set origin to center of the window
	MyDrawing(GraphicsContext* gc) : vc(gc->getWindowWidth()/2, gc->getWindowHeight()/2) {}
	virtual ~MyDrawing();
	virtual void paint(GraphicsContext* gc);
	virtual void keyDown(GraphicsContext* gc, unsigned int keycode);
	virtual void keyUp(GraphicsContext* gc, unsigned int keycode);
	virtual void mouseButtonDown(GraphicsContext* gc, 
							unsigned int button, int x, int y);
	virtual void mouseButtonUp(GraphicsContext* gc,
							unsigned int button, int x, int y);
	virtual void mouseMove(GraphicsContext* gc, int x, int y);
private:
	ViewContext vc;
	image theImage;
	int shapeSelector = 0;
	uint color = GraphicsContext::WHITE;
	bool dragging = false;
	int dragOriginX;
	int dragOriginY;
};


#include "mydrawing.h"

#define KEY_n 110
#define KEY_q 113
#define KEY_w 119
#define KEY_l 108
#define KEY_r 114
#define KEY_s 115
#define KEY_ROTXN 44
#define KEY_ROTX 46
#define KEY_ROTYN 59
#define KEY_ROTY 39
#define KEY_ROTZN 91
#define KEY_ROTZ 93
#define KEY_LEFT 65361
#define KEY_UP 65362
#define KEY_RIGHT 65363
#define KEY_DOWN 65364
#define KEY_MINUS 65453
#define KEY_PLUS 65451


MyDrawing::~MyDrawing()
{
	
}

void MyDrawing::paint(GraphicsContext* gc)
{
	gc->clear();
	theImage.draw(gc, &vc);
}

void MyDrawing::keyDown(GraphicsContext* gc, unsigned int keycode)
{
	switch(keycode)
	{
		case KEY_LEFT:
			vc.translate(-10,0,0);
			paint(gc);
			break;
		case KEY_RIGHT:
			vc.translate(10,0,0);
			paint(gc);
			break;
		case KEY_DOWN:
			vc.translate(0,-10,0);
			paint(gc);
			break;
		case KEY_UP:
			vc.translate(0,10,0);
			paint(gc);
			break;
		default:
			break;
	}
}

void MyDrawing::keyUp(GraphicsContext* gc, unsigned int keycode)
{
	//std::cout << keycode << std::endl << std::flush;
	switch(keycode)
	{
		case KEY_MINUS:
			vc.scale(0.5);
			paint(gc);
			break;
		case KEY_PLUS:
			vc.scale(2);
			paint(gc);
			break;
		case KEY_r:
			vc.reset();
			paint(gc);
			break;
		case KEY_ROTXN:
			vc.rotate(-10,0,0);
			paint(gc);
			break;	
		case KEY_ROTX:
			vc.rotate(10,0,0);
			paint(gc);
			break;
		case KEY_ROTYN:
			vc.rotate(0,-10,0);
			paint(gc);
			break;	
		case KEY_ROTY:
			vc.rotate(0,10,0);
			paint(gc);
			break;
		case KEY_ROTZN:
			vc.rotate(0,0,-10);
			paint(gc);
			break;	
		case KEY_ROTZ:
			vc.rotate(0,0,10);
			paint(gc);
			break;
		case KEY_n:
			gc->clear();
			theImage.erase();
			break;
		case KEY_q:
			vc.FOV(-5);
			paint(gc);
			break;
		case KEY_w:
			vc.FOV(5);
			paint(gc);
			break;
		case KEY_s:
			if (!dragging)
			{
				std::ofstream mfile("image.dat");
				theImage.out(mfile);
				mfile.close();
				theImage.draw(gc, &vc);
			}
			break;
		case KEY_l:
			if (!dragging)
			{
				std::ifstream mfile("image.stl");
				theImage.in(mfile);
				mfile.close();
				gc->clear();
				theImage.draw(gc, &vc);
			}
			break;
	}
}


void MyDrawing::mouseButtonDown(GraphicsContext* gc, unsigned int button, int x, int y)
{
	if (!dragging)
	{
		dragging = true;
		dragOriginX = x;
		dragOriginY = y;
	}
}

void MyDrawing::mouseButtonUp(GraphicsContext* gc, unsigned int button, int x, int y)
{
	if (dragging)
	{
		dragging = false;
	}
}

void MyDrawing::mouseMove(GraphicsContext* gc, int x, int y)
{
	if (dragging)
	{
		int ydiff = y - dragOriginY;
		int xdiff = x - dragOriginX;
		vc.hvOrbit(xdiff, ydiff);
		paint(gc);
	}
}


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
	triangle(uint color, double x0, double y0, double z0, double x1,
			 double y1, double z1, double x2, double y2, double z2) 
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

/**
 * matrix.h - declaration of matrix class.  This class will be
 * capable of represting a Matrix as a first-class type.
 * 
 * Do not change any public methods in this file unless otherwise
 * instructed.
 *
 * For CS321.  (c) 2015 Dr. Darrin Rothe
 */
 
 
// compile guard 
#ifndef MATRIX_H
#define MATRIX_H
 
#include <iostream> // for std::ostream
#include <stdexcept>	// for std::runtime_error
#include <string>	// used in exception
 
#define uint unsigned int

// a helper class to bundle a message with any thrown exceptions.
// To use, simply 'throw matrixException("A descriptive message about
// the problem")'.  This will throw the exception object by value.
// Recommendation is to catch by reference (to prevent slicing).
class matrixException:public std::runtime_error
{
	public:
		matrixException(std::string message)
		: std::runtime_error((std::string("Matrix Exception: ")+message).c_str()){}
};

//helper class for accessing individual values (someclass[][])
class proxy 
{
	public:
	    proxy(double* colArray, const double& cols);
	    double& operator[](const uint col) const;
	private:
	    double* colArray;
	    const double cols;
};

class matrix
{
	public:
		// No default (no argument) constructor.  It doesn't really make
		// sense to have one as we cannot rely on a size.  This may trip
		// us up later, but it will lead to a better implementation.
		// matrix();
 
		// Constructor - create Matrix and clear cells.  If rows or
		// cols is < 1, throw a matrixException.  Note, we will not use
		// "exception specifications" as multiple sources report that 
		// the mechanism is not properly supported by most compilers.
		//
		// throw (matrixException)
		//
		matrix(unsigned int rows, unsigned int cols);
 
		// Copy constructor - make a new Matrix just like rhs
		matrix(const matrix& from);
 
		// Destructor.  Free allocated memory
		~matrix();
 
		// Assignment operator - make this just like rhs.  Must function
		// correctly even if rhs is a different size than this.
		matrix& operator=(const matrix& rhs);
 
		// "Named" constructor(s).  This is not a language mechanism, rather
		// a common programming idiom.  The underlying issue is that with
		// overloaded operators, you can loose sight of what various
		// combinations of arguments means.  For our design, constructor
		// arguments set the size of the matrix.  If we want to generate
		// "special" matrices, we are better off with statuc methods that 
		// will generate these and then we can give the metods meaningful
		// names.
		
		// Named Constructor - produce a square identity matrix of the
		// requested size.  Since we do not know how the object produced will
		// be used, we pretty much have to return by value.  A size of 0 
		// would not make sense and should throw an exception.
		//
		// throw (matrixException)
		//
		static matrix identity(unsigned int size);
		
 
 
		// Matrix addition - lhs and rhs must be same size otherwise
		// an exception shall be thrown
		//
		// throw (matrixException)
		//
		matrix operator+(const matrix& rhs) const;
		matrix operator-(const matrix& rhs) const;
 
		// Matrix multiplication - lhs and rhs must be compatible 
		// otherwise an exception shall be thrown
		//
		// throw (matrixException)
		//
		matrix operator*(const matrix& rhs) const;
 
		// Scalar multiplication.  Note, this function will support
		// someMatrixObject * 5.0, but not 5.0 * someMatrixObject.
		matrix operator*(const double scale) const;

		// Transpose of a Matrix - should always work, hence no exception
		matrix operator~() const;
 
		// Clear Matrix to all members 0.0
		void clear();
  
		// Access Operators - throw an exception if index out of range
		//
		// throw (matrixException)
		//
		proxy operator[](uint row);
 
		// const version of above - throws an exception if indices are out of
		// range
		//
		// throw (matrixException)
		//
		proxy operator[](uint row) const;
 
		// I/O - for convenience - this is intended to be called by the global
		// << operator declared below.
		std::ostream& out(std::ostream& os) const;

		//input operator
		friend std::istream & operator>> (std::istream & is, matrix & out);
		
	private:
		// The data - note, per discussion on arrays, you can store these data
		// as a 1-D dynamic array, thus the double* below.  Alternatively, and
		// perhaps preferred, you could store the data as an array of arrays
		// which would require the_Matrix to be changed to a double**.
		double** the_matrix;
		unsigned int rows;
		unsigned int cols;

		/** routines **/

		//free memory allocated for the_matrix
		void deleteInternal();
		
		//allocate the_matrix
		void allocClearInternal();

		//allocate the_matrix copy data
		void allocAndCopyInternal(const matrix& from);

};

/** Some Related Global Functions **/
 
// Overloaded global << with std::ostream as lhs, Matrix as rhs.  This method
// should generate output compatible with an ostream which is commonly used
// with console (cout) and files.
//
// Since this is a global function, it does not have access to the private
// data of a Matrix object.  So, it will need to use the public interface of
// Matrix to do its job.  The method Matrix::out was added to Matrix
// specifically for this purpose.  The other option would have been to make
// it a "friend"
 
std::ostream& operator<<(std::ostream& os, const matrix& rhs);


//friend operator>>
std::istream& operator>> (std::istream& is, matrix& out);
 
// We would normally have a corresponding >> operator, but
// will defer that exercise that until a later assignment.


// Scalar multiplication with a global function.  Note, this function will
// support 5.0 * someMatrixObject, but not someMatrixObject * 5.0
matrix operator*(const double scale, const matrix& rhs);

 
#endif //MATRIX_H



/* 
* implementation for matrix.h 
* 
* 12/12/16
* Petr Elexa
*
*/


#include "matrix.h"
#include <string>
#include <cmath>

proxy::proxy(double* colArray, const double& cols) 
			: colArray(colArray), cols(cols) {}

double& proxy::operator[](const uint col) const
{
	if (col >= cols || col < 0) throw matrixException("out of range"); 
	
	return colArray[col];
}

void matrix::deleteInternal()
{
	for (uint i = 0; i < rows; ++i)
	{
		delete [] the_matrix[i];
	}
	delete [] the_matrix;
}

void matrix::allocClearInternal()
{
	the_matrix = new double*[rows];
	for (uint i = 0; i < rows; ++i)
	{
		the_matrix[i] = new double[cols];
		//clear
		for (uint j = 0; j < cols; ++j)
		{
			the_matrix[i][j] = 0.;
		}
	}
}

void matrix::allocAndCopyInternal(const matrix& from)
{
	the_matrix = new double*[rows];
	for (uint i = 0; i < rows; ++i)
	{
		the_matrix[i] = new double[cols];
		//copy
		for (uint j = 0; j < cols; ++j)
		{
			the_matrix[i][j] = from.the_matrix[i][j];
		}
	}
}

// Parameterized constructor
matrix::matrix(unsigned int rows, unsigned int cols) : rows(rows),cols(cols) 
{  
	if (rows < 1 || cols < 1)
	{
		throw matrixException("p-constructor bad arguments");
	}

	allocClearInternal();
}

// Copy constructor
matrix::matrix(const matrix& from) : rows(from.rows), cols(from.cols)
{
	allocAndCopyInternal(from);
}

// Destructor
matrix::~matrix()
{
	deleteInternal();
}

// Assignment operator
matrix& matrix::operator=(const matrix& rhs)
{
	if (this == &rhs) return *this;

	if (rows != rhs.rows  || cols != rhs.cols)
	{
		deleteInternal();

		rows = rhs.rows;
		cols = rhs.cols;

		allocAndCopyInternal(rhs);
	}
	else
	{
		for (uint i = 0; i < rows; ++i)
			for (uint j = 0; j < cols; ++j)
			{
				the_matrix[i][j] = rhs.the_matrix[i][j];
			}
	}

	return *this;
}

// Named constructor (static)
matrix matrix::identity(unsigned int size)
{
	if (size < 1) throw matrixException("out of range");
	
	// use p-constructor
	matrix product = matrix(size,size);
	// make it identity matrix
	for (uint i = 0; i < size; ++i)
	{
		product.the_matrix[i][i] = 1.;
	}
	
	return product;
}


// Binary operations
matrix matrix::operator+(const matrix& rhs) const
{
	if (rows != rhs.rows  || cols != rhs.cols) 
		throw matrixException("size does not match |+|");

	matrix product(rows, cols);

	for (uint i = 0; i < rows; ++i)
		for (uint j = 0; j < cols; ++j)
		{
			product.the_matrix[i][j] = the_matrix[i][j] + rhs.the_matrix[i][j];
		}

	return product;
}

matrix matrix::operator-(const matrix& rhs) const
{
	if (rows != rhs.rows  || cols != rhs.cols) 
		throw matrixException("size does not match |-|");

	matrix product(rows, cols);

	for (uint i = 0; i < rows; ++i)
		for (uint j = 0; j < cols; ++j)
		{
			product.the_matrix[i][j] = the_matrix[i][j] - rhs.the_matrix[i][j];
		}

	return product;
}


matrix matrix::operator*(const matrix& rhs) const
{
	if (cols != rhs.rows)
		throw matrixException("size does not not match |*|");

	matrix product(rows, rhs.cols);
	double tmp;

	for (uint i = 0; i < rows; ++i)
		for (uint j = 0; j < rhs.cols; ++j)
		{
			tmp = 0;
			for (uint k = 0; k < cols; ++k)
			{
				tmp += the_matrix[i][k] * rhs.the_matrix[k][j];
			}
			product.the_matrix[i][j] = tmp;
		}
	return product;
}

matrix matrix::operator*(const double scale) const
{
	matrix product(rows, cols);

	for (uint i = 0; i < rows; ++i)
		for (uint j = 0; j < cols; ++j)
		{
			product.the_matrix[i][j] = the_matrix[i][j] * scale;
		}

	return product;
}


// Unary operations
matrix matrix::operator~() const
{
	matrix product(cols, rows);

	for (uint i = 0; i < cols; ++i)
		for (uint j = 0; j < rows; ++j)
		{
			product.the_matrix[i][j] = the_matrix[j][i];
		}

	return product;
}
	

void matrix::clear()
{
	for (uint i = 0; i < rows; ++i)
		for (uint j = 0; j < cols; ++j)
		{
			the_matrix[i][j] = 0.0;
		}
	return;
}

proxy matrix::operator[](uint row)
{
	if (row >= rows || row < 0) 
		throw matrixException("out of range");
	
	return proxy(the_matrix[row], cols);
}

proxy matrix::operator[](uint row) const
{
	if (row >= rows || row < 0) 
		throw matrixException("out of range");

	return proxy(the_matrix[row], cols);
}


std::ostream& matrix::out(std::ostream& os) const
{
	os << rows << ' ' << cols << std::endl;
	os << "{\n";
	for (uint i = 0; i < rows; ++i)
	{
		os << "{";
		for (uint j = 0; j < cols; ++j)
		{
			os << the_matrix[i][j];
			os << (j+1 < cols ? "," : "");
		}
		os << (i+1 < rows ? "},\n" : "}\n");
	}	
	os << "}";
	return os;
}

std::istream & operator>> (std::istream & is, matrix & out)
{
	uint unpaired = 0, r = 0, c, refc = 0;
	double num;
	char token;

	//freeMatrix(out.the_matrix, out.rows);
	out.deleteInternal();

	//read number of rows and columns
	is >> out.rows >> out.cols;

	//allocate new matrix
	out.the_matrix = new double* [out.rows];
	for (uint i = 0; i < out.rows; ++i){
		out.the_matrix[i] = new double [out.cols];
	}
	
	is >> token;
	if (token != '{') {
		is.setstate (std::ios::failbit);
		return is;
	}
	do
	{
		c = 0;
		is >> token;
		if (token == '{')
		{
			unpaired++;
			do
			{
				is >> num;
				if (!is) return is;
				out.the_matrix[r][c] = num;
				c++;
				is >> token;
			} while ( token == ',' );
			if ( token == '}' )
			{
				if (!refc) refc = c;
				else if (refc != c) break;
				unpaired--;
			} 
			else break;
		}
		else break;
		r++;
	} while (is.get() == ',');

	if (unpaired || r != out.rows || refc != out.cols) {
		is.setstate (std::ios::failbit);
		return is;
	}
	is >> token;
	if (token != '}'){
		is.setstate (std::ios::failbit);
		return is;
	}

	return is;
}

// Global insertion and operator
std::ostream& operator<<(std::ostream& os, const matrix& rhs)
{
	rhs.out(os);
	return os;
}

// Global scalar multiplication
matrix operator*(const double scale, const matrix& rhs)
{
	return rhs*scale;
}


#Makefile
CCP		= 	g++
CFLAGS	=	-Wall -pedantic -std=c++11 -g
LIBS	=	-lX11
BIN		=	draw-app
SRC		=	main.cpp shape.cpp triangle.cpp matrix.cpp \
			image.cpp x11context.cpp mydrawing.cpp gcontext.cpp \
			viewcontext.cpp
OBJ		=	$(SRC:.cpp=.o)
HEAD 	=   

#to avoid a conflict with a file of the same name
.PHONY: all clean compile run

all: compile

compile: $(BIN)

run: $(BIN)
	./$(BIN)

clean: 
	rm -f $(BIN) $(OBJ)

#links objects to binary	
$(BIN) : $(OBJ)
	$(CCP) $(OBJ) -o $(BIN) $(LIBS)

#all dependencies from header files
$(OBJ) : $(HEAD)

#makes any xxx.o from xxx.cpp
%.o : %.cpp
	$(CCP) -c $(CFLAGS)  $< -o $@