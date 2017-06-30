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
	shape* drawnShape = NULL;
	int shapeSelector = 0;
	uint color = GraphicsContext::WHITE;
	bool dragging = false;
};