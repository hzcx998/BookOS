/*************************************************************************
    > File Name: food.cpp
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Sun Sep 13 11:59:49 2020
 ************************************************************************/

/*
MIT License

Copyright (c) 2020 Joker2770

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
*/

#include "food.h"

SDL_Rect* Food::drawSelf(void)
{
	this->m_x_pos = (rand()%SCREEN_WIDTH)/ GRID_UNION_WIDTH * GRID_UNION_WIDTH;
	this->m_y_pos = (rand()%SCREEN_HEIGHT)/ GRID_UNION_HEIGHT * GRID_UNION_HEIGHT;

	this->m_sRec[0] = {
		this->m_x_pos,
		this->m_y_pos,
		GRID_UNION_WIDTH,
		GRID_UNION_HEIGHT
	};

	return this->m_sRec;
}
