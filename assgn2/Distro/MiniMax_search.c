/*
	CSC D84 - Unit 2 - MiniMax search and adversarial games

	This file contains stubs for implementing a MiniMax search
        procedure with alpha-beta pruning. Please read the assignment
	handout carefully - it describes the game, the data you will
	have to handle, and the search functions you must provide.

	Once you have read the handout carefully, implement your search
	code in the sections below marked with

	**************
	*** TO DO:
	**************

	Make sure to add it to your report.txt file - it will be marked!

	Have fun!

	DO NOT FORGET TO 'valgrind' YOUR CODE - We will check for pointer
	management being done properly, and for memory leaks.

	Starter code: F.J.E. Sep. 15
*/

#include "MiniMax_search.h"

double MiniMax(double gr[graph_size][4], int path[1][2], double minmax_cost[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int mode, double (*utility)(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4]), int agentId, int depth, int maxDepth, double alpha, double beta)
{
 /*
   This function is the interface between your solution for the assignment and the driver code. The driver code
   in MiniMax_search_core_GL will call this function once per frame, and provide the following data
   
   Board and game layout:

	Exactly the same as for Assignment 1 - have a look at your code if you need a reminder of how the adjacency
	list and agent positions are stored.	

	Note that in this case, the path will contain a single move - at the top level, this function will provide
	an agent with the 'optimal' mini-max move given the game state.


   IMPORTANT NOTE: Mini-max is a recursive procedure. This function will need to fill-in the mini-max values for 
	 	   all game states down to the maximum search depth specified by the user. In order to do that,
		   the function needs to be called with the correct state at each specific node in the mini-max
		   search tree.

		   The game state is composed of:

			* Mouse, cat, and cheese positions (and number of cats and cheeses)
			
		   At the top level (when this function is called by the mini-max driver code), the game state
		   correspond to the current situation of the game. But once you start recursively calling
		   this function for lower levels of the search tree the positions of agents will have changed.
		   
		   Therefore, you will need to define local variables to keep the game state at each node of the
		   mini-max search tree, and you will need to update this state when calling recursively so that
		   the search does the right thing.

		   This function *must check* whether:
			* A candidate move results in a terminal configuration (cat eats mouse, mouse eats cheese)
			  at which point it calls the utility function to get a value
	 		* Maximum search depth has been reached (depth==maxDepth), at which point it will also call
			  the utility function to get a value
			* Otherwise, call recursively using the candidate configuration to find out what happens
			  deeper into the mini-max tree.

   Arguments:
		gr[graph_size][4]   		- This is an adjacency list for the maze
		path[1][2] 			- Your function will return the optimal mini-max move in this array.
		minmax_cost[size_X][size_Y]	- An array in which your code will store the
						  minimax value for maze locations expanded by
						  the search *when called for the mouse, not
						  for the cats!*

						  This array will be used to provide a visual 
						  display of minimax values during the game.

		cat_loc[10][2], cats   - Location of cats and number of cats (we can have at most 10,
					 but there can be fewer). Only valid cat locations are 0 to (cats-1)
		cheese_loc[10][2], cheeses - Location and number of cheese chunks (again at most 10,
					     but possibly fewer). Valid locations are 0 to (cheeses-1)
		mouse_loc[1][2] - Mouse location - there can be only one!
		mode - Search mode selection:
					mode = 0 	- No alpha-beta pruning
					mode = 1	- Alpha-beta pruning

		(*utility)(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4]);
				- This is a pointer to the utility function which returns a value for a specific game configuration

				   NOTE: Unlike the search assignment, this utility function also gets access to the graph so you can do any processing 					 that requires knowledge of the maze for computing the utility values.

				  * How to call the utility function from within this function : *
					- Like any other function:
						u = utility(cat_loc, cheese_loc, mouse_loc, cats, cheeses, depth, gr);
						
		agentId: Identifies which agent we are doing MiniMax for. agentId=0 for the mouse, agentId in [1, cats] for cats. Notice that recursive calls
                         to this function should increase the agentId to reflect the fact that the next level down corresponds to the next agent! For a game
                         with two cats and a mouse, the agentIds for the recursion should look like 0, 1, 2, 0, 1, 2, ...
	
		depth: Current search depth - whether this is a MIN or a MAX node depends both on depth and agentId.
		
		maxDepth: maximum desired search depth - once reached, your code should somehow return
			  a minimax utility value for this location.

		alpha. beta: alpha and beta values passed from the parent node to constrain search at this
			     level.

   Return values:
		Your search code will directly update data passed-in as arguments:
		
		- Mini-Max value	: Notice this function returns a double precision number. This is
					  the minimax value at this level of the tree. It will be used 
					  as the recursion backtracks filling-in the mini-max values back
					  from the leaves to the root of the search tree. 

		- path[1][2]		: Your MiniMax function will return the location for the agent's 
					  next location (i.e. the optimal move for the agent). 
		- minmax_cost[size_X][size_Y] 	:  Your search code will update this array to contain the
						   minimax value for locations that were expanded during
						   the search. This must be done *only* for the mouse.

						   Values in this array will be in the range returned by
						   your utility function.

		* Your code MUST NOT modify the locations or numbers of cats and/or cheeses, the graph,
	     	  or the location of the mouse - if you try, the driver code will know it *
			
		That's that, now, implement your solution!
 */

 /********************************************************************************************************
 * 
 * TO DO:	Implement code to perform a MiniMax search. This will involve a limited-depth BFS-like
 *              expansion. Once nodes below return values, your function will propagate minimax utilities
 *		as per the minimax algorithm.
 *	
 *		Note that if alpha-beta pruning is specified, you must keep track of alphas and betas
 *		along the path.
 *
 *		You can use helper functions if it seems reasonable. Add them to the MiniMax_search.h
 *		file and explain in your code why they are needed and how they are used.
 *
 *		Recursion should appear somewhere.
 *
 *		MiniMax cost: If the agentId=0 (Mouse), then once you have a MiniMax value for a location
 *		in the maze, you must update minmax_cost[][] for that location.
 *
 *		How you design your solution is up to you. But:
 *
 *		- Document your implementation by adding concise and clear comments in this file
 *		- Document your design (how you implemented the solution, and why) in the report
 *
 ********************************************************************************************************/

 // Stub so that the code compiles/runs - This will be removed and replaced by your code!
 path[0][0] = mouse_loc[0][0];
 path[0][1] = mouse_loc[0][1];
 int prev_loc = mouse_loc[0][0]+mouse_loc[0][1]*32;
 double best_utility = helper(gr, path, minmax_cost, cat_loc, cats, cheese_loc, cheeses, mouse_loc, mode, utility, 0, 0, maxDepth, alpha, beta, prev_loc);
 return best_utility;
}

int getMoveIndex(int direction,int x, int y){
	if(direction == 0){
		return x + (y-1)*32;
	}
	else if(direction == 1){
		return (x+1) + y*32;
	}
	else if(direction == 2){
		return x + (y+1)*32;
	}
	else {
		return (x-1) + y*32;
	}
}

double helper(double gr[graph_size][4], int path[1][2], double minmax_cost[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int mode, double (*utility)(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4]), int agentId, int depth, int maxDepth, double alpha, double beta, int prev_loc){
 if(depth == 0){
 	alpha = -1000;
 	beta = 1000;
 }
 // if it is a terminal or max depth we want to now see what the utility is for the mouses location
 if(maxDepth == depth || checkForTerminal(mouse_loc, cat_loc, cheese_loc, cats, cheeses)){
	 double loc_utility = utility(cat_loc, cheese_loc, mouse_loc, cats, cheeses, depth, gr);
	 minmax_cost[mouse_loc[0][0]][mouse_loc[0][1]] = loc_utility;
	 return loc_utility;
 }
 else if(agentId != 0){
	int next_move = (agentId == cats)? 0:agentId++;
	int curr_cat_loc = cat_loc[agentId-1][0] + (cat_loc[agentId-1][1]*size_X);
	int cat_loc_temp[10][2];
	double utility_results = -1000;
	memcpy(cat_loc_temp, cat_loc, sizeof(int)*20);
	for(int i = 0; i < 4; i++){
		int new_loc = getMoveIndex(i, cat_loc[agentId-1][0], cat_loc[agentId-1][1]);
		if(gr[curr_cat_loc][i] == 1){
			cat_loc_temp[agentId-1][0] = new_loc%32;
			cat_loc_temp[agentId-1][1] = new_loc/32;
			utility_results = helper(gr, path, minmax_cost, cat_loc_temp, cats, cheese_loc, cheeses, mouse_loc, mode, utility, next_move, depth + 1, maxDepth, alpha, beta, prev_loc);
			if(utility_results < beta) beta = utility_results;
			if(mode == 1 && beta <= alpha){
				return beta;
			}
		}
	}
	return beta;
 }
 else { 
	int mouse_loc_index = mouse_loc[0][0] + mouse_loc[0][1]*32;
	double utility_results = -1000;
	int mouse_loc_temp[1][2];
	for(int i = 0; i < 4; i++){
		int new_loc = getMoveIndex(i, mouse_loc[0][0], mouse_loc[0][1]);
		if(gr[mouse_loc_index][i] == 1){
			mouse_loc_temp[0][0] = new_loc%32;
			mouse_loc_temp[0][1] = new_loc/32;
			utility_results = helper(gr, path, minmax_cost, cat_loc, cats, cheese_loc, cheeses, mouse_loc_temp, mode, utility, 1, depth + 1, maxDepth, alpha, beta, mouse_loc_index);
			if(new_loc == prev_loc){
				utility_results-=15;
			}
			if(utility_results > alpha){
				alpha = utility_results;
				if(depth == 0){
					path[0][0] = mouse_loc_temp[0][0];
					path[0][1] = mouse_loc_temp[0][1];
				}
			}
			if(mode == 1 && beta <= alpha){
				minmax_cost[mouse_loc[0][0]][mouse_loc[0][1]] = alpha;
				return alpha;
			}
		}
	}
	// if(depth == 0){
	// 	path[0][0] = mouse_loc_temp[0][0];
	// 	path[0][1] = mouse_loc_temp[0][1];
	// }
	minmax_cost[mouse_loc[0][0]][mouse_loc[0][1]] = alpha;
	return alpha;
 }
}

double utility(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4])
{
 /*
	This function computes and returns the utility value for a given game configuration.
	As discussed in lecture, this should return a positive value for configurations that are 'good'
	for the mouse, and a negative value for locations that are 'bad' for the mouse.

	How to define 'good' and 'bad' is up to you. Note that you can write a utility function
	that favours your mouse or favours the cats, but that would be a bad idea... (why?)

	Input arguments:

		cat_loc - Cat locations
		cheese_loc - Cheese locations
		mouse_loc - Mouse location
		cats - # of cats
		cheeses - # of cheeses
		depth - current search depth
		gr - The graph's adjacency list for the maze

		These arguments are as described in A1. Do have a look at your solution!
 */
int utilityValue = 0;
 // just returns manhattan distance between location and closest cheese
 int smallest = 100000;
 int distance;
 for (int i = 0; i < cheeses; i++) {
	 // manhattan distance between location and cheese
	 distance = abs(mouse_loc[0][0] - cheese_loc[i][0]) + abs(mouse_loc[0][1] - cheese_loc[i][1]);
	 if (distance < smallest) {
		smallest = distance;
	 }
 }
 utilityValue -= smallest;
 // less moves to cheese = better moves to cheese
 utilityValue -= 0.01*depth;
 // it's bad if you're surrounded by walls
 int index = mouse_loc[0][0] + mouse_loc[0][1]*32;
 int numWalls = 0;
 for (int i = 0; i < 4; i++) {
	numWalls += 1 - gr[index][i];
 }
 utilityValue -= numWalls * 0.5;
 if (numWalls == 3) utilityValue -= 50;

 // being far from cat is better
 smallest = 100000;
 for (int i = 0; i < cats; i++) {
	 // manhattan distance between location and cheese
	 distance = abs(mouse_loc[0][0] - cat_loc[i][0]) + abs(mouse_loc[0][1] - cat_loc[i][1]);
	 if (distance < smallest) {
		smallest = distance;
	 }
 }
 utilityValue += smallest;

 return utilityValue;
}

int checkForTerminal(int mouse_loc[1][2],int cat_loc[10][2],int cheese_loc[10][2],int cats,int cheeses)
{
 /* 
		printf("chosen path: %d\n", chose);
     - The mouse eats a cheese
   
   If the node is a terminal, the function returns 1, else it returns 0
 */

 // Check for cats having lunch
 for (int i=0; i<cats; i++)
  if (mouse_loc[0][0]==cat_loc[i][0]&&mouse_loc[0][1]==cat_loc[i][1]) return(1);

 // Check for mouse having lunch
 for (int i=0; i<cheeses; i++)
  if (mouse_loc[0][0]==cheese_loc[i][0]&&mouse_loc[0][1]==cheese_loc[i][1]) return(1);

 return(0);

}

