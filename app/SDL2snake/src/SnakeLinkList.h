/*************************************************************************
    > File Name: SnakeLinkList.h
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Sat Sep 19 14:39:21 2020
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

#ifndef __SNAKELINKLIST_H__
#define __SNAKELINKLIST_H__

#include <stdio.h>

typedef struct snake_node
{
	int x_pos;
	int y_pos;
	struct snake_node* next;
}SnakeNode, *SnakeList;

int countSnakeLength(SnakeList L);
void printList(SnakeList L);
void cleanSnakeNode(SnakeList L);
//idx is the position which will be insert, start with 1
SnakeList insertNode(SnakeList L, int idx, int x_pos, int y_pos);
//idx start with 1
SnakeList foundNode(SnakeList L, int idx);
//idx start with 1
SnakeList deleteNode(SnakeList L, int idx);

#endif

