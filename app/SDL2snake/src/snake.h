/*************************************************************************
    > File Name: snake.h
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Tue Sep  8 09:03:57 2020
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

#ifndef __SNAKE_H__
#define __SNAKE_H__

#ifdef _WIN32
//Windows
#include <windows.h>
extern "C"
{
#include "SDL.h"
};
#else
//Linux
//Linux C++
#include <unistd.h>
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "SnakeLinkList.h"

class Snake
{
	public:
		Snake() : m_iLength(0), m_drag(DRAG), isEating(false), m_snake(NULL), m_CurDirection(UP)
		{
		}
		~Snake()
		{
			cleanSnakeNode(this->m_snake);
		}

	public:
		void initSelf(int x, int y);
		void moveSelf(void);
		void haltSelf(void);
		void changeDirection(DRIVER_DIRECTION direction);
		SDL_Rect* drawSelf(void);
		bool isEatSelf(void);
		bool isAlive(void);
		int getLength(void);
		int getDrag(void);

	public:
		SnakeList m_snake;
		bool isEating;
		//At most (SCREEN_WIDTH*SCREEN_HEIGHT / (GRID_UNION_HEIGHT*GRID_UNION_WIDTH)) snake nodes;
		SDL_Rect m_sRec[SCREEN_WIDTH*SCREEN_HEIGHT / (GRID_UNION_HEIGHT*GRID_UNION_WIDTH)];
		DRIVER_DIRECTION m_CurDirection;

	private:
		int m_iLength;
		int m_drag;

};

#endif
