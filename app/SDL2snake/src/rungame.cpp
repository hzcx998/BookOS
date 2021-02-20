/*************************************************************************
    > File Name: rungame.cpp
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Mon Sep  7 08:40:32 2020
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

#ifdef _WIN32
//Windows
extern "C"
{
#include "SDL.h"
#include "SDL_ttf.h"
};
#else
//Linux
//Linux C++
#ifdef __cplusplus
extern "C"
{
#endif
	
#include <SDL.h>
#include <SDL_ttf.h>

//Linux C++
#ifdef __cplusplus
};
#endif
#endif

#include <stdio.h>
#include <time.h>
#include "config.h"
#include "snake.h"
#include "food.h"
#include "renderer.h"

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void closeAll();

//Single player
void singlePlayer(Renderer* LRenderer, Snake* snake, Food* food);

//Double player
void doublePlayer(Renderer* LRenderer, Snake* snake1, Snake* snake2, Food* food);

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Globally used font
TTF_Font *gFont = NULL;

//Global flag of quit
bool BeQuit = false;

int main(int argc, char *argv[])
{
	//Initialize the random number seed for rand()
	srand((unsigned)time(NULL));

	Snake *snake = new Snake();
	Snake *snake1 = new Snake();
	Snake *snake2 = new Snake();
	Food *food = new Food();
	Food *gfood = new Food();
	Renderer *LRenderer = new Renderer();
	
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			int x, y;
			x = y = 0;

			SDL_Event e;
			while (!BeQuit)
			{
				while (0 != SDL_PollEvent(&e))
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						BeQuit = true;
					}
					else if (e.type == SDL_KEYDOWN)
					{
						switch (e.key.keysym.sym)
						{
						case SDLK_q:
							BeQuit = true;
							break;
						case SDLK_ESCAPE:
							BeQuit = true;
							break;
						case SDLK_1:
							singlePlayer(LRenderer, snake, food);
							break;
						case SDLK_2:
							doublePlayer(LRenderer, snake1, snake2, gfood);
							break;
						default:
							break;
						}
					}
					else if (e.type == SDL_MOUSEBUTTONUP)
					{
						SDL_GetMouseState(&x, &y);
						//Mouse click up button
						if (x > 0 && y > 0 && x < SCREEN_WIDTH && y < BUTTON_HEIGHT)
						{
							singlePlayer(LRenderer, snake, food);
						}
						//Mouse click down button
						else if (x > 0 && y > BUTTON_HEIGHT && x < SCREEN_WIDTH && y < SCREEN_HEIGHT)
						{
							doublePlayer(LRenderer, snake1, snake2, gfood);
						}
					}
				}

				//Render text
				SDL_Color textColor = { 0, 0, 0, 0 };
				SDL_RenderClear(gRenderer);

				SDL_Rect sRecUP = { 0, 0, BUTTON_WIDTH, BUTTON_HEIGHT };
				SDL_Rect sRecDOWN = { 0, BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT };
				SDL_SetRenderDrawColor(gRenderer, 193, 0xFF, 193, 0xFF);
				SDL_RenderFillRect(gRenderer, &sRecUP);
				SDL_SetRenderDrawColor(gRenderer, 255, 250, 205, 0xFF);
				SDL_RenderFillRect(gRenderer, &sRecDOWN);

				if (LRenderer->loadFromRenderedText(gRenderer, gFont, "Single player", textColor))
				{
					LRenderer->render(gRenderer, (BUTTON_WIDTH - LRenderer->getWidth()) / 2, (BUTTON_HEIGHT - LRenderer->getHeight()) / 2);
				}
				if (LRenderer->loadFromRenderedText(gRenderer, gFont, "Double player", textColor))
				{
					LRenderer->render(gRenderer, (BUTTON_WIDTH - LRenderer->getWidth()) / 2, BUTTON_HEIGHT + (BUTTON_HEIGHT - LRenderer->getHeight()) / 2);
				}

				SDL_RenderPresent(gRenderer);
				SDL_RenderClear(gRenderer);
			}
		}
	}

	if (NULL != gfood)
	{
		delete gfood;
		gfood = NULL;
	}

	if (NULL != food)
	{
		delete food;
		food = NULL;
	}

	if (NULL != snake)
	{
		delete snake;
		snake = NULL;
	}

	if (NULL != snake1)
	{
		delete snake1;
		snake1 = NULL;
	}

	if (NULL != snake2)
	{
		delete snake2;
		snake2 = NULL;
	}

	if (NULL != LRenderer)
	{
		delete LRenderer;
		LRenderer = NULL;
	}

	closeAll();

	return 0;
}

void singlePlayer(Renderer* LRenderer, Snake* snake, Food* food)
{
	snake->initSelf((SCREEN_WIDTH / 2) / GRID_UNION_WIDTH * GRID_UNION_WIDTH, (SCREEN_HEIGHT / 2) / GRID_UNION_HEIGHT * GRID_UNION_HEIGHT);
	food->drawSelf();
	//Render text
	SDL_Color textColor = { 0, 0, 0, 0 };
	if (LRenderer->loadFromRenderedText(gRenderer, gFont, "Ready...", textColor))
	{
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);

		LRenderer->render(gRenderer, (SCREEN_WIDTH - LRenderer->getWidth()) / 2, (SCREEN_HEIGHT - LRenderer->getHeight()) / 2);
		printf("LRenderer: w = %d, h = %d\n", LRenderer->getWidth(), LRenderer->getHeight());
		SDL_RenderPresent(gRenderer);
		SDL_RenderClear(gRenderer);

#ifdef _WIN32
		Sleep(3000);
#else
		usleep(3000000);
#endif
	}

	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	//While application is running
	while (!quit)
	{
		DRIVER_DIRECTION oldDirection = snake->m_CurDirection;

		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				BeQuit = true;
				quit = true;
			}
			else if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_UP:
					snake->changeDirection(UP);
					break;
				case SDLK_DOWN:
					snake->changeDirection(DOWN);
					break;
				case SDLK_LEFT:
					snake->changeDirection(LEFT);
					break;
				case SDLK_RIGHT:
					snake->changeDirection(RIGHT);
					break;
				case SDLK_q:
					quit = true;
					break;
				case SDLK_ESCAPE:
					quit = true;
					break;
				default:
					break;
				}
			}
		}

		//Avoid to turn around
		if (0 == (oldDirection + snake->m_CurDirection))
		{
			printf("Event queue lead to old direction opposite to new direction, fix it!\n");
			snake->m_CurDirection = oldDirection;
		}

		//Clear screen
		printf("Clear screen!\n");
		SDL_SetRenderDrawColor(gRenderer, 199, 237, 204, 0xFF);
		SDL_RenderClear(gRenderer);

		//Render red filled quad
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
		//Generate food
		bool bFoodOnSnake = false;
		do
		{
			if (food->beEaten || bFoodOnSnake)
				food->drawSelf();
			//Point to m_snake head
			SnakeList pFound = snake->m_snake;
			do
			{
				//Judge if food on snake
				if ((pFound->x_pos == food->m_x_pos) && (pFound->y_pos == food->m_y_pos))
				{
					printf("Food on snake\n");
					bFoodOnSnake = true;
					break;
				}
				else
					bFoodOnSnake = false;

				pFound = pFound->next;
			} while (NULL != pFound);
		} while (bFoodOnSnake);

		food->beEaten = false;

		//Recover food
		if (NULL != food->m_sRec)
			SDL_RenderFillRect(gRenderer, &(food->m_sRec[0]));

		snake->moveSelf();
		snake->drawSelf();
		int iSnakeLength = snake->getLength();
		for (int i = 0; i < iSnakeLength; i++)
		{
			//Render green filled quad
			SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
			if (0 == i)
				SDL_SetRenderDrawColor(gRenderer, 0xB2, 0x22, 0x22, 0xFF);
			SDL_RenderFillRect(gRenderer, &(snake->m_sRec[i]));
			//Render blue outlined quad
			SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF);
			SDL_RenderDrawRect(gRenderer, &(snake->m_sRec[i]));
		}

		if (snake->m_snake->x_pos == food->m_x_pos && snake->m_snake->y_pos == food->m_y_pos)
		{
			snake->isEating = true;
			food->beEaten = true;
		}

		int ilimit = SCREEN_HEIGHT * SCREEN_WIDTH / (GRID_UNION_HEIGHT*GRID_UNION_WIDTH);
		if (iSnakeLength > ilimit - 5 || !snake->isAlive())
		{
			SDL_Event e;
			while (!quit)
			{
				while (0 != SDL_PollEvent(&e))
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						BeQuit = true;
						quit = true;
					}
					else if (e.type == SDL_KEYDOWN)
					{
						switch (e.key.keysym.sym)
						{
						case SDLK_q:
							quit = true;
							break;
						case SDLK_ESCAPE:
							quit = true;
							break;
						default:
							break;
						}
					}
				}
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//Render text
				SDL_Color textColor = { 0, 0, 0, 0 };
				char sText[128] = "\0";
				snprintf(sText, sizeof(sText), "Game Over!You get %d.", iSnakeLength - 2);
				//printf("%s\n", sText);
				if (LRenderer->loadFromRenderedText(gRenderer, gFont, sText, textColor))
				{
					LRenderer->render(gRenderer, (SCREEN_WIDTH - LRenderer->getWidth()) / 2, (SCREEN_HEIGHT - LRenderer->getHeight()) / 2);
					//printf("LRenderer: w = %d, h = %d\n", LRenderer->getWidth(), LRenderer->getHeight());
				}

				//Update screen
				SDL_RenderPresent(gRenderer);
				SDL_RenderClear(gRenderer);
			}
		}

		//Update screen
		SDL_RenderPresent(gRenderer);
		SDL_RenderClear(gRenderer);
		snake->haltSelf();
	}
}

void doublePlayer(Renderer* LRenderer, Snake* snake1, Snake* snake2, Food* food)
{
	snake1->initSelf((SCREEN_WIDTH / 2) / GRID_UNION_WIDTH * GRID_UNION_WIDTH, (SCREEN_HEIGHT / 4) / GRID_UNION_HEIGHT * GRID_UNION_HEIGHT);
	snake2->initSelf((SCREEN_WIDTH / 2) / GRID_UNION_WIDTH * GRID_UNION_WIDTH, (SCREEN_HEIGHT * 3 / 4) / GRID_UNION_HEIGHT * GRID_UNION_HEIGHT);
	food->drawSelf();
	//Render text
	SDL_Color textColor = { 0, 0, 0, 0 };
	SDL_RenderClear(gRenderer);

	SDL_Rect sRecUP = { 0, 0, BUTTON_WIDTH, BUTTON_HEIGHT };
	SDL_Rect sRecDOWN = { 0, BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT };
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderFillRect(gRenderer, &sRecUP);
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderFillRect(gRenderer, &sRecDOWN);

	if (LRenderer->loadFromRenderedText(gRenderer, gFont, "'W', 'S', 'A', 'D' for player 1!", textColor))
	{
		LRenderer->render(gRenderer, (BUTTON_WIDTH - LRenderer->getWidth()) / 2, (BUTTON_HEIGHT - LRenderer->getHeight()) / 2);
	}
	if (LRenderer->loadFromRenderedText(gRenderer, gFont, "'Up', 'Down', 'Left', 'Right' for player 2!", textColor))
	{
		LRenderer->render(gRenderer, (BUTTON_WIDTH - LRenderer->getWidth()) / 2, BUTTON_HEIGHT + (BUTTON_HEIGHT - LRenderer->getHeight()) / 2);
	}

	SDL_RenderPresent(gRenderer);
	SDL_RenderClear(gRenderer);

#ifdef _WIN32
		Sleep(3000);
#else
		usleep(3000000);
#endif

	long iCount = 0;

	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	//While application is running
	while (!quit)
	{
		DRIVER_DIRECTION oldDirection1 = snake1->m_CurDirection;
		DRIVER_DIRECTION oldDirection2 = snake2->m_CurDirection;

		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				BeQuit = true;
				quit = true;
			}
			else if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_UP:
					printf("key up up up ...\n");
					snake2->changeDirection(UP);
					break;
				case SDLK_DOWN:
					printf("key down down down ...\n");
					snake2->changeDirection(DOWN);
					break;
				case SDLK_LEFT:
					printf("key left left left ...\n");
					snake2->changeDirection(LEFT);
					break;
				case SDLK_RIGHT:
					printf("key right right right ...\n");
					snake2->changeDirection(RIGHT);
					break;
				case SDLK_w:
					snake1->changeDirection(UP);
					break;
				case SDLK_s:
					snake1->changeDirection(DOWN);
					break;
				case SDLK_a:
					snake1->changeDirection(LEFT);
					break;
				case SDLK_d:
					snake1->changeDirection(RIGHT);
					break;
				case SDLK_q:
					quit = true;
					break;
				case SDLK_ESCAPE:
					quit = true;
					break;
				default:
					break;
				}
			}
		}

		//Avoid to turn around
		if (0 == (oldDirection1 + snake1->m_CurDirection))
		{
			printf("Event queue lead to old direction1 opposite to new direction, fix it!\n");
			snake1->m_CurDirection = oldDirection1;
		}
		if (0 == (oldDirection2 + snake2->m_CurDirection))
		{
			printf("Event queue lead to old direction2 opposite to new direction, fix it!\n");
			snake2->m_CurDirection = oldDirection2;
		}

		//Clear screen
		printf("Clear screen!\n");
		SDL_SetRenderDrawColor(gRenderer, 199, 237, 204, 0xFF);
		SDL_RenderClear(gRenderer);

		//Render red filled quad
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
		//Generate food
		bool bFoodOnSnake = false;
		do
		{
			if (food->beEaten || bFoodOnSnake)
				food->drawSelf();
			//Point to m_snake head
			SnakeList pFound1 = snake1->m_snake;
			SnakeList pFound2 = snake2->m_snake;
			//Snake1
			do
			{
				//Judge if food on snake
				if (((pFound1->x_pos == food->m_x_pos) && (pFound1->y_pos == food->m_y_pos)) || ((pFound2->x_pos == food->m_x_pos) && (pFound2->y_pos == food->m_y_pos)))
				{
					printf("Food on snake1\n");
					bFoodOnSnake = true;
					break;
				}
				else
					bFoodOnSnake = false;

				pFound1 = pFound1->next;
			} while (NULL != pFound1);
			if (!bFoodOnSnake)
			{
				//Snake2
				do
				{
					//Judge if food on snake
					if ((pFound2->x_pos == food->m_x_pos) && (pFound2->y_pos == food->m_y_pos))
					{
						printf("Food on snake2\n");
						bFoodOnSnake = true;
						break;
					}
					else
						bFoodOnSnake = false;

					pFound2 = pFound2->next;
				} while (NULL != pFound2);
			}
		} while (bFoodOnSnake);

		food->beEaten = false;

		//Recover food
		if (NULL != food->m_sRec)
			SDL_RenderFillRect(gRenderer, &(food->m_sRec[0]));

		//Draw snake1
		if (0 == iCount % snake1->getDrag())
		{
			snake1->moveSelf();
			snake1->drawSelf();
		}
		int iSnakeLength1 = snake1->getLength();
		for (int i = 0; i < iSnakeLength1; i++)
		{
			//Render green filled quad
			SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
			if (0 == i)
				SDL_SetRenderDrawColor(gRenderer, 0xB2, 0x22, 0x22, 0xFF);
			SDL_RenderFillRect(gRenderer, &(snake1->m_sRec[i]));
			//Render blue outlined quad
			SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF);
			SDL_RenderDrawRect(gRenderer, &(snake1->m_sRec[i]));
		}
		//Draw snake2
		if (0 == iCount % snake2->getDrag())
		{
			snake2->moveSelf();
			snake2->drawSelf();
		}
		int iSnakeLength2 = snake2->getLength();
		for (int i = 0; i < iSnakeLength2; i++)
		{
			//Render green filled quad
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0x00, 0xFF);
			if (0 == i)
				SDL_SetRenderDrawColor(gRenderer, 0xB2, 0x22, 0x22, 0xFF);
			SDL_RenderFillRect(gRenderer, &(snake2->m_sRec[i]));
			//Render blue outlined quad
			SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF);
			SDL_RenderDrawRect(gRenderer, &(snake2->m_sRec[i]));
		}
		//Judge snake2 eating
		if (snake1->m_snake->x_pos == food->m_x_pos && snake1->m_snake->y_pos == food->m_y_pos)
		{
			printf("Snake1 is eating ... \n");
			snake1->isEating = true;
			food->beEaten = true;
		}
		//Judge snake1 eating
		else if (snake2->m_snake->x_pos == food->m_x_pos && snake2->m_snake->y_pos == food->m_y_pos)
		{
			printf("Snake2 is eating ... \n");
			snake2->isEating = true;
			food->beEaten = true;
		}

		bool snake1BeCrash = false;
		bool snake2BeCrash = false;
		SnakeList pH1 = snake1->m_snake;
		SnakeList pH2 = snake2->m_snake;
		while (NULL != pH1)
		{
			if (snake2->m_snake->x_pos == pH1->x_pos && snake2->m_snake->y_pos == pH1->y_pos)
			{
				snake1BeCrash = true;
				break;
			}
			pH1 = pH1->next;
		}
		if (!snake1BeCrash)
		{
			while (NULL != pH2)
			{
				if (snake1->m_snake->x_pos == pH2->x_pos && snake1->m_snake->y_pos == pH2->y_pos)
				{
					snake2BeCrash = true;
					break;
				}
				pH2 = pH2->next;
			}
		}

		int ilimit = SCREEN_HEIGHT * SCREEN_WIDTH / (GRID_UNION_HEIGHT*GRID_UNION_WIDTH);
		if (iSnakeLength1 + iSnakeLength2 > ilimit - 5 || !snake1->isAlive() || !snake2->isAlive() || snake1BeCrash || snake2BeCrash)
		{
			SDL_Event e;
			while (!quit)
			{
				while (0 != SDL_PollEvent(&e))
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						BeQuit = true;
						quit = true;
					}
					else if (e.type == SDL_KEYDOWN)
					{
						switch (e.key.keysym.sym)
						{
						case SDLK_q:
							quit = true;
							break;
						case SDLK_ESCAPE:
							quit = true;
							break;
						default:
							break;
						}
					}
				}
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//Render text
				SDL_Color textColor = { 0, 0, 0, 0 };
				char sText[128] = "\0";
				snprintf(sText, sizeof(sText), "Game Over! Snake1 get %d, Snake2 get %d. ", iSnakeLength1 - 2, iSnakeLength2 - 2);
				//printf("%s\n", sText);
				if (iSnakeLength1 != iSnakeLength2)
				{
					//Render background
					iSnakeLength1 > iSnakeLength2 ?
						SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF) :
						SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0x00, 0xFF);
					SDL_RenderClear(gRenderer);
				}
				if (LRenderer->loadFromRenderedText(gRenderer, gFont, sText, textColor))
				{
					LRenderer->render(gRenderer, (SCREEN_WIDTH - LRenderer->getWidth()) / 2, (SCREEN_HEIGHT - LRenderer->getHeight()) / 2);
					//printf("LRenderer: w = %d, h = %d\n", LRenderer->getWidth(), LRenderer->getHeight());
				}

				//Update screen
				SDL_RenderPresent(gRenderer);
				SDL_RenderClear(gRenderer);
			}
		}

		//Update screen
		SDL_RenderPresent(gRenderer);
		SDL_RenderClear(gRenderer);

		if (iCount > MAXDRAG)
		{
			iCount = 0;
		}
		iCount += MIXDRAG;
		
		printf("iCount: %ld\n", iCount);

#ifdef _WIN32
		Sleep(MIXDRAG/1000);
#else
		usleep(MIXDRAG);
#endif
	}
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("SDL2snake v20.20.11-stable", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_SOFTWARE);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
				}
			}

		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Open the font
	gFont = TTF_OpenFont(TTF_PATH, 28);
	if (NULL == gFont)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		gFont = TTF_OpenFont("lazy.ttf", 28);
		if (NULL == gFont)
		{
			printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
			//For snap
			gFont = TTF_OpenFont("/snap/sdl2snake/current/usr/local/bin/lazy.ttf", 28);
			if (NULL == gFont)
			{
				printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
				success = false;
			}
		}
	}

	//Nothing to load
	return success;
}

void closeAll()
{
	//Free global font
	if (NULL != gFont)
	{
		TTF_CloseFont(gFont);
		gFont = NULL;
	}

	//Destroy window
	if (NULL != gRenderer)
	{
		SDL_DestroyRenderer(gRenderer);
		gRenderer = NULL;
	}
	if (NULL != gWindow)
	{
		SDL_DestroyWindow(gWindow);
		gWindow = NULL;
	}

	//Quit SDL subsystems
	TTF_Quit();
	SDL_Quit();
}
