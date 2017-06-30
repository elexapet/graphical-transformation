/* 
* main.cpp
*
*/

#include "line.h"
#include "triangle.h"
#include "image.h"
#include "x11context.h"
#include "mydrawing.h"
#include "viewcontext.h"
#include <fstream>
#include <unistd.h>

void testImageDraw(double scale, int angle)
{
	GraphicsContext* gc = new X11Context(800, 600, GraphicsContext::BLACK);
	image img;

	//img.add(new triangle(GraphicsContext::WHITE, 50, 50, 50, 120, 120, 50));
	//img.add(new line(GraphicsContext::GREEN, 100, 250, 500, 150));
	//img.add(new line(GraphicsContext::RED, 100, 150, 500, 250));
	//img.add(new triangle(GraphicsContext::BLUE, 50, 150, 120, 220, 50, 220));
	img.add(new line(GraphicsContext::GREEN, 0, 0, 800, 600));

	ViewContext vc(400, 300);
	vc.scale(scale);
	vc.rotate(angle);

	img.draw(gc, &vc);

	sleep(90);
	delete gc;
}

void drawingBoard()
{
	GraphicsContext* gc = new X11Context(800, 600, GraphicsContext::BLACK);
	MyDrawing md(gc);

	gc->runLoop(&md);

	delete gc;
}

int main(int argc, char const *argv[])
{
	drawingBoard();
	//testImageDraw(std::strtod(argv[1], 0), std::stoi(argv[2], 0, 10));
	
	return 0;
}