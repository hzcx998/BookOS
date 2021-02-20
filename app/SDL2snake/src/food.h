/*************************************************************************
    > File Name: food.h
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Tue Sep  8 13:42:54 2020
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

#ifndef __FOOD_H__
#define __FOOD_H__

#ifdef _WIN32
//Windows
extern "C"
{
#include "SDL.h"
};
#else
//Linux
//Linux C++
#ifdef __cplusplus
extern "C"
{
#endif
	
#include<SDL2/SDL.h>

//Linux C++
#ifdef __cplusplus
};
#endif
#endif

#include<stdlib.h>
#include"config.h"

class Food
{
	public:
		Food():m_x_pos((rand()%SCREEN_WIDTH)/ GRID_UNION_WIDTH * GRID_UNION_WIDTH), m_y_pos((rand()%SCREEN_HEIGHT)/ GRID_UNION_HEIGHT * GRID_UNION_HEIGHT)
		{
			this->m_sRec[0] = {};
			this->m_sRec[0] = {
				this->m_x_pos,
				this->m_y_pos,
				GRID_UNION_WIDTH,
				GRID_UNION_HEIGHT
			};
			this->beEaten = false;
		}
		~Food()
		{
			this->m_sRec[0] = {};
			this->m_x_pos = 0;
			this->m_y_pos = 0;
		}

	public:
		SDL_Rect* drawSelf(void);

	public:
		int m_x_pos;
		int m_y_pos;
		SDL_Rect m_sRec[1];
		bool beEaten;
};

#endif

