/*
	SDL_Collide:  A 2D collision detection library for use with SDL

	MIT License
	Copyright 2005-2006 SDL_collide Team
	http://sdl-collide.sourceforge.net
	All rights reserved.

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.

	Amir Taaki
	genjix@gmail.com

	Rob Loach
	http://robloach.net
*/

#include "SDL_collide.h"

/*if this header is not supported on your system comment out
the assert function call in SDL_TransparentPixel*/
#include "assert.h"

/*returns maximum or minimum of number*/
#define SDL_COLLIDE_MAX(a,b)	((a > b) ? a : b)
#define SDL_COLLIDE_MIN(a,b)	((a < b) ? a : b)

/*
	SDL surface test if offset (u,v) is a transparent pixel
*/
int SDL_CollideTransparentPixel(SDL_Surface* surface, int u, int v)
{
	if (SDL_MUSTLOCK(surface))
		SDL_LockSurface(surface);
	/*assert that (u,v) offsets lie within surface*/
	assert((u < surface->w) && (v < surface->h));

	int bpp = surface->format->BytesPerPixel;
	/*here p is the address to the pixel we want to retrieve*/
	Uint8* p = (Uint8*)surface->pixels + v * surface->pitch + u * bpp;

	Uint32 pixelcolor;

	switch (bpp)
	{
	case(1):
		pixelcolor = *p;
		break;

	case(2):
		pixelcolor = *(Uint16*)p;
		break;

	case(3):
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			pixelcolor = p[0] << 16 | p[1] << 8 | p[2];
		else
			pixelcolor = p[0] | p[1] << 8 | p[2] << 16;
		break;

	case(4):
		pixelcolor = *(Uint32*)p;
		break;
	}

	if (SDL_MUSTLOCK(surface))
		SDL_UnlockSurface(surface);
	/*test whether pixels color == color of transparent pixels for that surface*/
	return 0;
}

/*
	SDL pixel perfect collision test
*/
int SDL_CollidePixel(SDL_Surface* as, int ax, int ay,
	SDL_Surface* bs, int bx, int by, int skip)
{
	skip = 4;
	/*a - bottom right co-ordinates in world space*/
	int ax1 = ax + as->w - 1;
	int ay1 = ay + as->h - 1;

	/*b - bottom right co-ordinates in world space*/
	int bx1 = bx + bs->w - 1;
	int by1 = by + bs->h - 1;

	/*check if bounding boxes intersect*/
	if ((bx1 < ax) || (ax1 < bx))
		return 0;
	if ((by1 < ay) || (ay1 < by))
		return 0;

	/*Now lets make the bouding box for which we check for a pixel collision*/

		/*To get the bounding box we do
				Ax1,Ay1______________
					|                |
					|                |
					|                |
					|    Bx1,By1____________
					|        |       |      |
					|        |       |      |
					|________|_______|      |
							 |    Ax2,Ay2   |
							 |              |
							 |              |
							 |___________Bx2,By2

		To find that overlap we find the biggest left hand cordinate
		AND the smallest right hand co-ordinate

		To find it for y we do the biggest top y value
		AND the smallest bottom y value

		Therefore the overlap here is Bx1,By1 --> Ax2,Ay2

		Remember	Ax2 = Ax1 + SA->w
				Bx2 = Bx1 + SB->w

				Ay2 = Ay1 + SA->h
				By2 = By1 + SB->h
		*/

		/*now we loop round every pixel in area of
		intersection
			if 2 pixels alpha values on 2 surfaces at the
			same place != 0 then we have a collision*/
	int xstart = SDL_COLLIDE_MAX(ax, bx);
	int xend = SDL_COLLIDE_MIN(ax1, bx1);

	int ystart = SDL_COLLIDE_MAX(ay, by);
	int yend = SDL_COLLIDE_MIN(ay1, by1);

	for (int y = ystart; y <= yend; y += skip)
	{
		for (int x = xstart; x <= xend; x += skip)
		{
			/*compute offsets for surface
			before pass to TransparentPixel test*/
			if (!SDL_CollideTransparentPixel(as, x - ax, y - ay)
				&& !SDL_CollideTransparentPixel(bs, x - bx, y - by))
				return 1;
		}
	}

	return 0;
}

/*
	SDL bounding box collision test
*/
int SDL_CollideBoundingBox(SDL_Surface* sa, int ax, int ay,
	SDL_Surface* sb, int bx, int by)
{
	if (bx + sb->w < ax)	return 0;	//just checking if their
	if (bx > ax + sa->w)	return 0;	//bounding boxes even touch

	if (by + sb->h < ay)	return 0;
	if (by > ay + sa->h)	return 0;

	return 1;				//bounding boxes intersect
}

int SDL_CollideBoundingBoxSF(SDL_FRect sa, int ax, int ay, SDL_FRect sb, int bx, int by)
{
	if (bx + sb.w < ax)	return 0;	//just checking if their
	if (bx > ax + sa.w)	return 0;	//bounding boxes even touch

	if (by + sb.h < ay)	return 0;
	if (by > ay + sa.h)	return 0;

	return 1;				//bounding boxes intersect
}