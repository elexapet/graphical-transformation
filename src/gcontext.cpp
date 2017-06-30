/* This is an abstract base class representing a generic graphics
 * context.  Most implementation specifics will need to be provided by
 * a concrete implementation.  See header file for specifics. */

#define _USE_MATH_DEFINES	// for M_PI
#include <cmath>	// for trig functions
#include "gcontext.h"
#include <utility>

/*
 * Destructor - does nothing
 */
GraphicsContext::~GraphicsContext()
{
	// nothing to do
	// here to insure subclasses handle destruction properly
}


/* This is a naive implementation that uses floating-point math
 * and "setPixel" which will need to be provided by the concrete
 * implementation.
 * 
 * Parameters:
 * 	x0, y0 - origin of line
 *  x1, y1 - end of line
 * 
 * Returns: void
 */
void GraphicsContext::drawLine(int x0, int y0, int x1, int y1)
{
	//find slope
	int dx = x1 - x0;
	int dy = y1 - y0;
	
	bool swap = false;

	//wrong octant in given quandrant?
	if (std::abs(dy) > std::abs(dx)){
		swap = true;
		std::swap(x0,y0);
		std::swap(y1,x1);
		//update dx & dy
		std::swap(dx,dy);
	}
	//right to left line?
	if (dx < 0) {
		std::swap(x0,x1);
		std::swap(y0,y1);
		//update dx & dy
		dx = -dx;
		dy = -dy;
	}

	//make sure we actually have a line
	if (dx != 0 || dy !=0)
	{
		int err = 2*dy - dx;
		int y = y0;
		for (int x = x0; x <= x1; ++x)
		{
			//put pixel on screen
			if (swap) setPixel(y, x);
			else setPixel(x, y);

			//downward slope
			if (dy < 0 && err < 0){
				y--;	
				err += dx;
			}
			//upward slope
			else if (err > 0)
			{
				y++;
				err -= dx;
			}
			//update err
			err += dy;
		}
	}
}



/* This is a naive implementation that uses floating-point math
 * and "setPixel" which will need to be provided by the concrete
 * implementation.
 * 
 * Parameters:
 * 	x0, y0 - origin/center of circle
 *  radius - radius of circle
 * 
 * Returns: void
 */
void GraphicsContext::drawCircle(int x0, int y0, unsigned int radius)
{
	int x = radius;
    int y = 0;
    int err = 0;

    //until x == y
    while (x >= y)
    {
    	//mirror to all octants
        setPixel(x0 + x, y0 + y);
        setPixel(x0 + y, y0 + x);
        setPixel(x0 - y, y0 + x);
        setPixel(x0 - x, y0 + y);
        setPixel(x0 - x, y0 - y);
        setPixel(x0 - y, y0 - x);
        setPixel(x0 + y, y0 - x);
        setPixel(x0 + x, y0 - y);

        //time to increment?
        if (err <= 0)
        {
            y++;
            err += 2*y + 1;
        }
        //time to decrement?
        if (err > 0)
        {
            x--;
            err -= 2*x + 1;
        }
    }
}

void GraphicsContext::endLoop()
{
	run = false;
}


