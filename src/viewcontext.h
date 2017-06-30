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