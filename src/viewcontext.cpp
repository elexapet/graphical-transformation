#include "viewcontext.h"

ViewContext::~ViewContext()
{

}

matrix ViewContext::modelToDevice(matrix& model)
{
	return _compTrans * model;
}

matrix ViewContext::deviceToModel(matrix& device)
{
	return _invCompTrans * device;
}

void ViewContext::rotate(double angle)
{
	//convert to radians
	_angle += angle / 57.2958;
	updateComposite();
}

void ViewContext::scale(double scale)
{
	_scale *= scale;
	updateComposite();
}

void ViewContext::translate(int x, int y)
{
	_x += x;
	_y += y;
	updateComposite();
}

void ViewContext::reset()
{
	_angle = 0;
	_scale = 1;
	_x = 0;
	_y = 0;
	updateComposite();
}

void ViewContext::updateComposite()
{
	matrix rotation(matrix::identity(3));
	matrix translation(matrix::identity(3));
	matrix fromOrigin(matrix::identity(3));
	matrix toOrigin(matrix::identity(3));
	matrix scaling(matrix::identity(3));
	matrix flip(matrix::identity(3));

	flip[1][1] = -1;

	rotation[0][0] = rotation[1][1] = std::cos(_angle);
	rotation[0][1] = std::sin(_angle);
	rotation[1][0] = -std::sin(_angle);

	toOrigin[0][2] = -_xOrigin;
	toOrigin[1][2] = -_yOrigin;

	fromOrigin[0][2] = _xOrigin;
	fromOrigin[1][2] = _yOrigin;

	translation[0][2] =  _x;
	translation[1][2] =  _y;

	scaling[0][0] = _scale;
	scaling[1][1] = _scale;

	//calculate composite transformation
	_compTrans = translation * fromOrigin * flip * rotation * scaling;
	//std::cout << _compTrans << std::endl;

	//invert each elementary transformation
	rotation[0][1] *= -1;
	rotation[1][0] *= -1;

	toOrigin[0][2] *= -1;
	toOrigin[1][2] *= -1;

	fromOrigin[0][2] *= -1;
	fromOrigin[1][2] *= -1;

	translation[0][2] *= -1;
	translation[1][2] *= -1;

	scaling[0][0] = 1/scaling[0][0];
	scaling[1][1] = 1/scaling[1][1];

	//calculate composite inverse transformation
	_invCompTrans = scaling * rotation * flip * toOrigin * translation;
}