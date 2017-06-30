#include "mydrawing.h"

#define KEY_n 110
#define KEY_q 113
#define KEY_w 119
#define KEY_l 108
#define KEY_r 114
#define KEY_s 115
#define KEY_1 49
#define KEY_2 50
#define KEY_3 51
#define KEY_4 52
#define KEY_5 53
#define KEY_6 54
#define KEY_7 55
#define KEY_8 56
#define KEY_9 57
#define KEY_ROTN 44
#define KEY_ROT 46
#define KEY_LEFT 65361
#define KEY_UP 65362
#define KEY_RIGHT 65363
#define KEY_DOWN 65364
#define KEY_MINUS 65453
#define KEY_PLUS 65451


MyDrawing::~MyDrawing()
{
	if (drawnShape != NULL) delete drawnShape;
}

void MyDrawing::paint(GraphicsContext* gc)
{
	gc->clear();
	theImage.draw(gc, &vc);
	if (drawnShape != NULL) drawnShape->draw(gc, &vc);
}

void MyDrawing::keyDown(GraphicsContext* gc, unsigned int keycode)
{
	switch(keycode)
	{
		case KEY_LEFT:
			vc.translate(-10,0);
			paint(gc);
			break;
		case KEY_RIGHT:
			vc.translate(10,0);
			paint(gc);
			break;
		case KEY_DOWN:
			vc.translate(0,-10);
			paint(gc);
			break;
		case KEY_UP:
			vc.translate(0,10);
			paint(gc);
			break;
		default:
			break;
	}
}

void MyDrawing::keyUp(GraphicsContext* gc, unsigned int keycode)
{
	std::cout << keycode << std::endl << std::flush;
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
		case KEY_ROTN:
			vc.rotate(-10);
			paint(gc);
			break;	
		case KEY_ROT:
			vc.rotate(10);
			paint(gc);
			break;
		case KEY_n:
			gc->clear();
			theImage.erase();
			break;
		case KEY_q:
			shapeSelector = 0;
			break;
		case KEY_w:
			shapeSelector = 1;
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
				std::ifstream mfile("image.dat");
				theImage.in(mfile);
				mfile.close();
				gc->clear();
				theImage.draw(gc, &vc);
			}
			break;
		case KEY_1:
			color = GraphicsContext::BLACK;
			break;
		case KEY_2:
			color = GraphicsContext::BLUE;
			break;
		case KEY_3:
			color = GraphicsContext::GREEN;
			break;
		case KEY_4:
			color = GraphicsContext::RED;
			break;
		case KEY_5:
			color = GraphicsContext::CYAN;
			break;
		case KEY_6:
			color = GraphicsContext::MAGENTA;
			break;
		case KEY_7:
			color = GraphicsContext::YELLOW;
			break;
		case KEY_8:
			color = GraphicsContext::GRAY;
			break;
		case KEY_9:
			color = GraphicsContext::WHITE;
			break;
	}
}

void MyDrawing::deviceToModel(int& x, int& y)
{
	matrix device(3,3);
	device[0][0] = x;
	device[1][0] = y;
	device[2][0] = 1;
	matrix model = vc.deviceToModel(device);
	x = device[0][0];
	y = device[1][0];
}

void MyDrawing::mouseButtonDown(GraphicsContext* gc, unsigned int button, int x, int y)
{
	if (!dragging)
	{
		deviceToModel(x,y);
		if (shapeSelector == 1) drawnShape = new triangle(color, x, y, x, y, x, y);
		else if (shapeSelector == 0) drawnShape = new line(color, x, y, x, y);

		gc->setMode(GraphicsContext::MODE_XOR);
		drawnShape->draw(gc, &vc);
		dragging = true;
	}
}

void MyDrawing::mouseButtonUp(GraphicsContext* gc, unsigned int button, int x, int y)
{
	if (dragging)
	{
		drawnShape->draw(gc, &vc);

		deviceToModel(x,y);

		drawnShape->setEndPoint(x, y);
		gc->setMode(GraphicsContext::MODE_NORMAL);
		drawnShape->draw(gc, &vc);

		if (drawnShape->done())
		{	
			theImage.add(drawnShape);
			drawnShape = NULL;
			dragging = false;
		}
		else
		{
			gc->setMode(GraphicsContext::MODE_XOR);
		}
	}
}

void MyDrawing::mouseMove(GraphicsContext* gc, int x, int y)
{
	if (dragging)
	{
		drawnShape->draw(gc, &vc);

		deviceToModel(x,y);

		drawnShape->updateEndPoint(x, y);
		drawnShape->draw(gc, &vc);
	}
}
