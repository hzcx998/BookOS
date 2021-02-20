/*************************************************************************
    > File Name: snake.cpp
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Sun Sep 13 11:58:56 2020
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

#include "snake.h"

#include <new>

void Snake::initSelf(int ix, int iy)
{
	this->m_snake = NULL;
	this->m_snake = insertNode(this->m_snake, 1, ix, iy);
	this->m_snake = insertNode(this->m_snake, 2, ix + GRID_UNION_WIDTH, iy);

	this->m_iLength = countSnakeLength(this->m_snake);
	this->isEating = false;
	this->m_drag = DRAG;
	printList(this->m_snake);

	memset(this->m_sRec, 0, sizeof(this->m_sRec));

	switch (rand() % 4)
	{
	case 0:
		this->m_CurDirection = UP;
		break;
	case 1:
		this->m_CurDirection = DOWN;
		break;
	case 2:
		this->m_CurDirection = LEFT;
		break;
	case 3:
		this->m_CurDirection = RIGHT;
		break;
	default:
		this->m_CurDirection = LEFT;
		break;
	}
}

bool Snake::isAlive(void)
{
	if (this->m_snake->x_pos > SCREEN_WIDTH || this->m_snake->x_pos < 0 || this->m_snake->y_pos > SCREEN_HEIGHT || this->m_snake->y_pos < 0)
		return false;

	if (this->isEatSelf())
		return false;

	return true;
}

void Snake::haltSelf(void)
{
	printf("drag: %d\n", this->m_drag);
#ifdef _WIN32
	Sleep((int)((this->m_drag) / 1000));
#else
	usleep(this->m_drag);
#endif
}

void Snake::moveSelf(void)
{
	//Update length
	this->m_iLength = countSnakeLength(this->m_snake);

	printList(this->m_snake);

	int tmp = 0;
	switch (this->m_CurDirection)
	{
	case UP:
		tmp = this->m_snake->y_pos - GRID_UNION_HEIGHT;
		this->m_snake = insertNode(this->m_snake, 1, this->m_snake->x_pos, tmp);
		break;
	case DOWN:
		tmp = this->m_snake->y_pos + GRID_UNION_HEIGHT;
		this->m_snake = insertNode(this->m_snake, 1, this->m_snake->x_pos, tmp);
		break;
	case LEFT:
		tmp = this->m_snake->x_pos - GRID_UNION_WIDTH;
		this->m_snake = insertNode(this->m_snake, 1, tmp, this->m_snake->y_pos);
		break;
	case RIGHT:
		tmp = this->m_snake->x_pos + GRID_UNION_WIDTH;
		this->m_snake = insertNode(this->m_snake, 1, tmp, this->m_snake->y_pos);
		break;
	default:
		break;
	}
	//Update length
	this->m_iLength = countSnakeLength(this->m_snake);

	if (!this->isEating)
		this->m_snake = deleteNode(this->m_snake, this->m_iLength);
	else
	{
		printf("Snake is eating ... \n");
		//Change flag
		this->isEating = false;
	}

	printList(this->m_snake);

	//Update length
	this->m_iLength = countSnakeLength(this->m_snake);
}

void Snake::changeDirection(DRIVER_DIRECTION direction)
{
	switch (direction)
	{
	case UP:
		if (DOWN != this->m_CurDirection)
		{
			if (UP == this->m_CurDirection)
			{
				if (this->m_drag > MIXDRAG)
				{
					this->m_drag -= MIXDRAG;
				}
			}
			this->m_CurDirection = UP;
		}
		else
		{
			if (MAXDRAG > this->m_drag)
			{
				this->m_drag += MIXDRAG;
			}
		}
		break;
	case DOWN:
		if (UP != this->m_CurDirection)
		{
			if (DOWN == this->m_CurDirection)
			{
				if (this->m_drag > MIXDRAG)
				{
					this->m_drag -= MIXDRAG;
				}
			}
			this->m_CurDirection = DOWN;
		}
		else
		{
			if (MAXDRAG > this->m_drag)
			{
				this->m_drag += MIXDRAG;
			}
		}
		break;
	case LEFT:
		if (RIGHT != this->m_CurDirection)
		{
			if (LEFT == this->m_CurDirection)
			{
				if (this->m_drag > MIXDRAG)
				{
					this->m_drag -= MIXDRAG;
				}
			}
			this->m_CurDirection = LEFT;
		}
		else
		{
			if (MAXDRAG > this->m_drag)
			{
				this->m_drag += MIXDRAG;
			}
		}
		break;
	case RIGHT:
		if (LEFT != this->m_CurDirection)
		{
			if (RIGHT == this->m_CurDirection)
			{
				if (this->m_drag > MIXDRAG)
				{
					this->m_drag -= MIXDRAG;
				}
			}
			this->m_CurDirection = RIGHT;
		}
		else
		{
			if (MAXDRAG > this->m_drag)
			{
				this->m_drag += MIXDRAG;
			}
		}
		break;
	default:
		break;
	}
}

bool Snake::isEatSelf(void)
{
	SnakeList pS = this->m_snake->next;
	while (NULL != pS)
	{
		if (pS->x_pos == this->m_snake->x_pos && pS->y_pos == this->m_snake->y_pos)	return true;
		pS = pS->next;	
	}

	return false;
}

SDL_Rect* Snake::drawSelf(void)
{
	SnakeList pSnake = this->m_snake;
	for (int i = 0; pSnake != NULL; i++)
	{
		this->m_sRec[i] = { pSnake->x_pos, pSnake->y_pos, GRID_UNION_WIDTH, GRID_UNION_HEIGHT };
		pSnake = pSnake->next;
	}
	return this->m_sRec;
}

int Snake::getLength(void)
{
	return this->m_iLength;
}

int Snake::getDrag(void)
{
	return this->m_drag;
}
