/*************************************************************************
    > File Name: renderer.cpp
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Tue Sep 22 09:59:44 2020
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

#include "renderer.h"

bool Renderer::loadFromRenderedText(SDL_Renderer* sRenderer, TTF_Font* font, const char* textureText, SDL_Color textColor)
{
	//Get rid of preexisting texture
	this->Free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText, textColor);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		this->mTexture = SDL_CreateTextureFromSurface(sRenderer, textSurface);
		if (mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			this->mWidth = textSurface->w;
			this->mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	//Return success
	return this->mTexture != NULL;
}

void Renderer::Free()
{
	//Free texture if it exists
	if (this->mTexture != NULL)
	{
		SDL_DestroyTexture(this->mTexture);
		this->mTexture = NULL;
		this->mWidth = 0;
		this->mHeight = 0;
	}
}

void Renderer::render(SDL_Renderer* sRenderer, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, this->mWidth, this->mHeight };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(sRenderer, this->mTexture, clip, &renderQuad, angle, center, flip);
}

int Renderer::getWidth()
{
	return this->mWidth;
}

int Renderer::getHeight()
{
	return this->mHeight;
}
