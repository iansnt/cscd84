/*
	CSC D84 - Unit 1 - Search

	This file contains the API function headers for your assignment.
	Please pay close attention to the function prototypes, and
	understand what the arguments are.

	Stubs for implementing each function are to be found in AI_search.c,
	along with clear ** TO DO markers to let you know where to add code.

	You are free to add helper functions within reason. But you must
	provide a prototype *in this file* as well as the implementation
	in the .c program file.

	Starter by: F.J.E., Jul. 2015
	Updated by: F.J.E., Jan. 2018
*/

#ifndef __AI_search_header

#define __AI_search_header

// Generally needed includes
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<malloc.h>

#include "board_layout.h"

// Function prototypes for D84 - Unit 1 - Search assignment solution

void search(double gr[graph_size][4], int path[graph_size][2], int visit_order[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int mode, int (*heuristic)(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4]));
int H_cost(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4]);
int H_cost_nokitty(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4]);
void bfs_search(double gr[graph_size][4], int path[graph_size][2], int visit_order[size_X][size_Y], int mouse_loc[1][2], int cheese_loc[10][2], int cheeses, int cats, int cat_loc[10][2]);
struct Container;
struct Node;
struct Node* createNode(int index, int priority);
struct Container* createStack();
int headIndex(struct Node** head);
void popPQueue(struct Node** head);
void pushPQueue(struct Node** head, int index, int priority);
int isEmptyPQueue(struct Node** node);
int isEmpty (struct Container* container);
void pushStack (struct Container* stack, int index);
int popStack (struct Container* stack);
void dfs_search(double gr[graph_size][4], int path[graph_size][2], int visit_order[size_X][size_Y], int mouse_loc[1][2], int cheese_loc[10][2], int cheeses, int cats, int cat_loc[10][2]);
int checkCat(int index, int cats, int cat_loc[10][2]);
void heuristic_search(double gr[graph_size][4], int path[graph_size][2], int visit_order[size_X][size_Y], int mouse_loc[1][2], int cheese_loc[10][2], int cat_loc[10][2], int cheeses, int cats, int (*heuristic)(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4]));
int bfsCat(double gr[graph_size][4], int x, int y, int cat_loc[10][2], int cats);
// If you need to add any function prototypes yourself, you can do so *below* this line.


#endif

