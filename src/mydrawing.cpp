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
