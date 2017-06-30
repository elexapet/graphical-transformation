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