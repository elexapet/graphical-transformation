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

class ViewContext
{
public:
	ViewContext(int xOrigin, int yOrigin) : _compTrans(matrix::identity(3)),
	 _invCompTrans(matrix::identity(3)), _xOrigin(xOrigin), _yOrigin(yOrigin)
	 { }
	~ViewContext();
	matrix modelToDevice(matrix& model);
	matrix deviceToModel(matrix& device);
	void rotate(double angle);
	void scale(double scale);
	void translate(int x, int y);
	void reset();
private:
	//composite transformation matrices
	matrix _compTrans;
	matrix _invCompTrans;
	//elementary transformation
	double _angle = 0;
	double _scale = 1;
	int _x = 0;
	int _y = 0;
	//device origin
	int _xOrigin;
	int _yOrigin;

	void updateComposite();
};