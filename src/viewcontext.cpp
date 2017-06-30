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
