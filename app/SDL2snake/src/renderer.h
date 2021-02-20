/*************************************************************************
    > File Name: renderer.h
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Tue Sep 22 09:59:25 2020
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

#ifndef __RENDER_H__
#define __RENDER_H__

#ifdef _WIN32
//Windows
extern "C"
{
#include "SDL.h"
#include "SDL_ttf.h"
};
#else
//Linux...
#ifdef __cplusplus
extern "C" {
#endif

#include <SDL.h>
#include <SDL_ttf.h>

#ifdef __cplusplus
};
#endif
#endif

#include <stdio.h>

class Renderer
{
public:
	Renderer() : mTexture(NULL), mWidth(0), mHeight(0)
	{
	}
	~Renderer() 
	{
		this->Free();
	}

	//Creates image from font string
	bool loadFromRenderedText(SDL_Renderer* Renderer, TTF_Font* font, const char* textureText, SDL_Color textColor);

	//Deallocates texture
	void Free();

	//Renders texture at given point
	void render(SDL_Renderer* Renderer, int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
};

#endif

