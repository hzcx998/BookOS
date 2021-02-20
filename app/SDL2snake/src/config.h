/*************************************************************************
    > File Name: config.h
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Tue Sep  8 16:02:36 2020
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

#ifndef __CONFIG_H__
#define __CONFIG_H__

#ifdef _WIN32
#define TTF_PATH "./lazy.ttf"
#else
// #define TTF_PATH "/usr/local/bin/lazy.ttf"
#define TTF_PATH "/res/SDL2snake/lazy.ttf"
#endif

typedef enum driver_direction
{
	UP = 1,
	DOWN = -1,
	LEFT = -2,
	RIGHT= 2
}DRIVER_DIRECTION;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Button constants
const int BUTTON_WIDTH = SCREEN_WIDTH;
const int BUTTON_HEIGHT = SCREEN_HEIGHT / 2;

const long DRAG = 2000000;
const long MAXDRAG = 3000000;
const long MIXDRAG = 250000;

const int GRID_UNION_WIDTH = 10;
const int GRID_UNION_HEIGHT = 10;

#endif
