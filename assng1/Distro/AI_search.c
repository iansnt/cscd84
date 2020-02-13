/*
	CSC D84 - Unit 1 - Search

	This file contains stubs for implementing the different search
	algorithms covered in the course. Please read the assignment
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

	Starter code: F.J.E., Jul. 15
	Updated: F.J.E., Jan. 18
*/

/**********************************************************************
% COMPLETE THIS TEXT BOX:
%
% 1) Student Name: Ajiteshwar Rai		
% 2) Student Name:		
%
% 1) Student number: 1003587647
% 2) Student number:
% 
% 1) UtorID raiajite
% 2) UtorID
% 
% We hereby certify that the work contained here is our own
%
% ___Ajiteshwar Rai___             _____________________
% (sign with your name)            (sign with your name)
***********************************************************************/

#include "AI_search.h"

// container to create a stack
struct Container{
	int top;
	int rear;
	int* store;
};

// node for creating linked list for priority queue
struct Node{
	int index;
	int priority;
	struct Node* next;
};

struct Node* createNode(int index, int priority){
	struct Node* node = (struct Node*)malloc(sizeof(struct Node));
	node->index = index;
	node->priority = priority;
	node->next = NULL;
	return node;
}

int headIndex(struct Node** head){
	return (*head)->index;
}

void popPQueue(struct Node** head){
	struct Node* temp = *head;
	(*head) = (*head)->next;
	free(temp);
}

void pushPQueue(struct Node** head, int index, int priority){
	struct Node* start = *head;
	struct Node* temp = createNode(index, priority);
	// if lower priority than head, make it new head
	if(start->priority > priority){
		temp->next = start;
		*head = temp;
	} else {
		// keep going until at right location
		while(start->next != NULL && start->next->priority <= priority){
			start = start->next;
		}
		temp->next = start->next;
		start->next = temp;
	}
}

struct Container* createStack(){
    struct Container* container = (struct Container*)malloc(sizeof(struct Container));
    container->top = -1;
    container->store = (int*)malloc(1024 * sizeof(int));
    return container;
}

int isEmpty(struct Container* container){
	return container->top == -1;
}

int isEmptyPQueue(struct Node** node){
	return (*node) == NULL;
}

void pushStack(struct Container* stack, int index){
	stack->store[++stack->top] = index;
}

int popStack(struct Container* stack){
	if (isEmpty(stack)) 
        return -1;
    return stack->store[stack->top--];
}

void freeList(struct Node* head){
	// jsut freeing remaining space
	struct Node* temp;
	while (head != NULL){
		temp = head;
		head = head->next;
		free(temp);
	}
}


void search(double gr[graph_size][4], int path[graph_size][2], int visit_order[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int mode, int (*heuristic)(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4]))
{
 /*
   This function is the interface between your solution for the assignment and the driver code. The driver code
   in AI_search_core_GL will call this function once per frame, and provide the following data
   
   Board and game layout:
	
	The game takes place on a grid of size 32x32, the file board_layout.h specifies the size and defines two
	constants 'size_X' and 'size_Y' for the horizontal and vertical size of the board, respectively. For our
	purposes, the grid of locations is represented by a graph with one node per grid location, so given
	the 32x32 cells, the graph has 1024 nodes. 

	To create a maze, we connect cell locations in the grid in such a way that a) there is a path from any
	grid location to any other grid location (i.e. there are no disconnected subsets of nodes in the graph),
	and b) there are loops.

	Since each node represents a grid location, each node can be connected to up to 4 neighbours in the
	top, right, bottom, and left directions respectively:

			node at (i,j-1)
				^
				|
	(node at i-1, j) <- node at (i,j) -> node at (i+1, j)
				|
				v
			node at (i,j+1)

	The graph is theredore stored as an adjacency list with size 1024 x 4, with one row per node in the
	graph, and 4 columns corresponding to the weight of an edge linking the node with each of its 4
	possible neighbours in the order towp, right, bottom, left (clockwise from top).

	Since all we care is whether nodes are connected. Weights will be either 0 or 1, if the weight is
	1, then the neighbouring nodes are connected, if the weight is 0, they are not. For example, if

	graph[i][0] = 0 
	graph[i][1] = 1
	graph[i][2] = 0
	graph[i][3] = 1

	then node i is connected to the right and left neighbours, but not to top or bottom.

	The index in the graph for the node corresponding to grid location (x,y) is

	index = x + (y*size_X) 		or in this case		index = x + (y*32)

	Conversely, if you have the index and want to figure out the grid location,

	x = index % size_X		or in this case		x = index % 32
	y = index / size_Y		or in this case		y = index / 32

	(all of the above are *integer* operations!)

	A path is a sequence of (x,y) grid locations. We store it using an array of size	
	1024 x 2 (since there are 1024 locations, this is the maximum length of any
	path that visits locations only once).

	Agent locations are coordinate pairs (x,y)	

   Arguments:
		gr[graph_size][4]   - This is an adjacency list for the maze
		path[graph_size][2] - An initially empty path for your code to fill.
				      In this case, empty means all entries are initially -1
		visit_order[size_X][size_Y]	- An array in which your code will store the
						  *order* in which grid locations were 
						  visited during search. For example, while
						  doing BFS, the initial location is the
						  start location, it's visit order is 1.
						  Then the search would expand the immediate
						  neighbours of the initial node in some order,
						  these would get a visit order of 2, 3, 4, and
						  5 respectively, and so on.
					
						  This array will be used to display the search
						  pattern generated by each search method.

		cat_loc[10][2], cats   - Location of cats and number of cats (we can have at most 10,
					 but there can be fewer). Only valid cat locations are 0 to (cats-1)
		cheese_loc[10][2], cheeses - Location and number of cheese chunks (again at most 10,
					     but possibly fewer). Valid locations are 0 to (cheeses-1)
		mouse_loc[1][2] - Mouse location - there can be only one!
		mode - Search mode selection:
					mode = 0 	- BFS
					mode = 1	- DFS
					mode = 2	- A*

		(*heuristic)(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[10][2], int cats, int cheeses)
				- This is a pointer to one of the heuristic functions you will implement, either H_cost()
				  or H_cost_nokitty(). The driver in AI_search_core_GL will pass the appropriate pointer
				  depending on what search the user wants to run.
			
				  If the mode is 0 or 1, this pointer is NULL

				  * How to call the heuristic function from within this function : *
					- Like any other function:
						h = heuristic( x, y, cat_loc, cheese_loc, mouse_loc, cats, cheeses);

   Return values:
		Your search code will directly update data passed-in as arguments:

		- path[graph_size][2]	: Your search code will update this array to contain the path from
					  the mouse to one of the cheese chunks. The order matters, so
					  path[0][:] must be the mouse's current location, path[1][:]
					  is the next move for the mouse. Each successive row will contain
					  the next move toward the cheese, and the path ends at a location
					  whose coordinates correspond to one of the cheese chunks.
					  Any entries beyond that must remain set to -1
		- visit_order[size_X][size_Y] 	:  Your search code will update this array to contain the
						   order in which each location in the grid was expanded
						   during search. This means, when that particular location
						   was checked for being a goal, and if not a goal, had its
						   neighbours added as candidates for future expansion (in
						   whatever order is dictated by the search mode).

						   Note that since there are 1024 locations, the values in 
						   this array must always be in [0, 1023]. The driver code
						   will then display search order as a yellow-colored
						   brightness map where nodes expanded earlier will look
						   brighter.

		* Your code MUST NOT modify the locations or numbers of cats and/or cheeses, the graph,
	     	  or the location of the mouse - if you try, the driver code will know it *
			
		That's that, now, implement your solution!
 */

 /********************************************************************************************************
 * 
 * TO DO:	Implement code to carry out the different types of search depending on the
 *		mode.
 *
 *		You can do this by writing code within this single function (and being clever about it!)
 *		Or, you can use this function as a wrapper that calls a different search function
 *		 (BFS, DFS, A*) depending on the mode. Note that in the latter case, you will have
 *	    	  to inform your A* function somehow of what heuristic it's supposed to use.
 *
 *		Visiting Order: When adding the neighbours of a node to your list of candidates for
 *				expansion, do so in the order top, right, bottom, left.
 *
 *		NOTE: Your search functions should be smart enough to not choose a path that goes
 *		      through a cat! this is easily done without any heuristics.
 * 
 *		How you design your solution is up to you. But:
 *
 *		- Document your implementation by adding concise and clear comments in this file
 *		- Document your design (how you implemented the solution, and why) in the report
 *
 ********************************************************************************************************/

 // changing the mode based on what they enter
 if(mode == 0){
	 bfs_search(gr, path, visit_order, mouse_loc, cheese_loc, cheeses, cats, cat_loc);
 } else if(mode == 1){
	 dfs_search(gr, path, visit_order, mouse_loc, cheese_loc, cheeses, cats, cat_loc);
 } else if(mode == 2){
	 heuristic_search(gr, path, visit_order, mouse_loc, cheese_loc, cat_loc, cheeses, cats, heuristic);
 } else {
	 // don't think I need this but I'll leave it in, who really cares
	 printf("invalid search mode\n");
 }

 return;
}

int checkCat(int index, int cats, int cat_loc[10][2]){
	// quick funciton that checks whether this current block is a cat
	for(int i = 0; i < cats; i++){
		// just go through all cats and compare the location with current location
		int curr_cat_index = cat_loc[i][0] + (cat_loc[i][1]*32);
		if(index == curr_cat_index){
			return 0;
		}
	}
	// yeah it returns 1 if there is no cat, but you know what I mean
	return 1;
}

void bfs_search(double gr[graph_size][4], int path[graph_size][2], int visit_order[size_X][size_Y], int mouse_loc[1][2], int cheese_loc[10][2], int cheeses, int cats, int cat_loc[10][2]){
	// get mouse's current location
	int m_loc = mouse_loc[0][0] + (mouse_loc[0][1]*size_X);
	// create the queue and add the current mouse location
	struct Node* head = createNode(m_loc, 0);
	int visited[1024] = {0};
	int deep[1024] = {-1};
	visited[m_loc] = 0;
	deep[m_loc] = 0;
	int found_cheese = 0;
	int curr_elem;
	int visit_num = 0;
	// just loop till you go everywhere or you find a cheese
	while(isEmptyPQueue(&head) != 1 && found_cheese == 0){
		curr_elem = headIndex(&head);
		// add when you visit a node
		visit_order[curr_elem % size_X][curr_elem / size_Y] = visit_num;
		visit_num++;
		for(int  i = 0; i < cheeses; i++){
			// if you found cheese, yippee
			if(cheese_loc[i] && curr_elem == (cheese_loc[i][0] + (cheese_loc[i][1]*size_X))){
				found_cheese = 1;
			}
		}

		// adding the possible options around it to the queue, checking if there is a cat or its already added
		if((gr[curr_elem][0] == 1) && checkCat(curr_elem-32, cats, cat_loc) && (visited[curr_elem-32] == 0)){
			pushPQueue(&head, curr_elem-32, head->priority + 1);
			visited[curr_elem-32] = curr_elem;
			deep[curr_elem-32] = deep[curr_elem]+1;
		}
		if((gr[curr_elem][1] == 1) && checkCat(curr_elem+1, cats, cat_loc) && (visited[curr_elem+1] == 0)){
			pushPQueue(&head, curr_elem+1, head->priority + 1);
			visited[curr_elem+1] = curr_elem;
			deep[curr_elem+1] = deep[curr_elem]+1;
		}
		if((gr[curr_elem][2] == 1) && checkCat(curr_elem+32, cats, cat_loc) && (visited[curr_elem+32] == 0)){
			pushPQueue(&head, curr_elem+32, head->priority + 1);
			visited[curr_elem+32] = curr_elem;
			deep[curr_elem+32] = deep[curr_elem]+1;
		}
		if((gr[curr_elem][3] == 1) && checkCat(curr_elem-1, cats, cat_loc) && (visited[curr_elem-1] == 0)){
			pushPQueue(&head, curr_elem-1, head->priority + 1);
			visited[curr_elem-1] = curr_elem;
			deep[curr_elem-1] = deep[curr_elem]+1;
		}
		popPQueue(&head);
	}

	int depth = deep[curr_elem];
	// you want the depth so you can create a path by going backwards
	while(depth != -1){
		// since we saved which node is visited from which, we can jsut follow the trail
		path[depth][0] = curr_elem % size_X;
		path[depth][1] = curr_elem / size_Y;
		depth--;
		curr_elem = visited[curr_elem];
	}
	if(head != NULL){
		// free whatever is left in the linked list after we found the cheese
		freeList(head);
	}
}

void dfs_search(double gr[graph_size][4], int path[graph_size][2], int visit_order[size_X][size_Y], int mouse_loc[1][2], int cheese_loc[10][2], int cheeses, int cats, int cat_loc[10][2]){
	// get mouse's current location
	int m_loc = mouse_loc[0][0] + (mouse_loc[0][1]*size_X);
	// create the queue and add the current mouse location
	struct Container* stack = createStack();
	pushStack(stack, m_loc);
	int visited[1024] = {0};
	int deep[1024] = {-1};
	visited[m_loc] = 0;
	deep[m_loc] = 0;
	int prev_visit = m_loc;
	int found_cheese = 0;
	int curr_elem;
	int visit_num = 0;
	while(found_cheese == 0){
		// this is a little different cuz the stack works easier with arrays than linked list imo
		curr_elem = popStack(stack);
		visit_order[curr_elem % size_X][curr_elem / size_Y] = visit_num;
		visit_num++;
		for(int  i = 0; i < cheeses; i++){
			// mmm finding cheese so we good
			if(cheese_loc[i] && curr_elem == (cheese_loc[i][0] + (cheese_loc[i][1]*size_X))){
				found_cheese = 1;
			}
		}
		// adding the possible options around it to the stack again checking for cats
		if((gr[curr_elem][0] == 1) && checkCat(curr_elem-32, cats, cat_loc) && (visited[curr_elem-32] == 0)){
			pushStack(stack, curr_elem-32);
			visited[curr_elem-32] = curr_elem;
			deep[curr_elem-32] = deep[curr_elem]+1;
		}
		if((gr[curr_elem][1] == 1) && checkCat(curr_elem+1, cats, cat_loc) && (visited[curr_elem+1] == 0)){
			pushStack(stack, curr_elem+1);
			visited[curr_elem+1] = curr_elem;
			deep[curr_elem+1] = deep[curr_elem]+1;
		}
		if((gr[curr_elem][2] == 1) && checkCat(curr_elem+32, cats, cat_loc) && (visited[curr_elem+32] == 0)){
			pushStack(stack, curr_elem+32);
			visited[curr_elem+32] = curr_elem;
			deep[curr_elem+32] = deep[curr_elem]+1;
		}
		if((gr[curr_elem][3] == 1) && checkCat(curr_elem-1, cats, cat_loc) && (visited[curr_elem-1] == 0)){
			pushStack(stack, curr_elem-1);
			visited[curr_elem-1] = curr_elem;
			deep[curr_elem-1] = deep[curr_elem]+1;
		}
	}

	int depth = deep[curr_elem] - 1;
	// this is the same as bfs, we get depth and follow backwards since we store predecesor
	while(depth != -1){
		path[depth][0] = curr_elem % size_X;
		path[depth][1] = curr_elem / size_Y;
		depth--;
		curr_elem = visited[curr_elem];
	}
	// freeing whatever is in the stack
	free(stack->store);
	free(stack);
}

void heuristic_search(double gr[graph_size][4], int path[graph_size][2], int visit_order[size_X][size_Y], int mouse_loc[1][2], int cheese_loc[10][2], int cat_loc[10][2], int cheeses, int cats, int (*heuristic)(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4])){
	// get mouse's current location
	int m_loc = mouse_loc[0][0] + (mouse_loc[0][1]*size_X);
	// create the queue and add the current mouse location
	struct Node* head = createNode(m_loc, 0);
	int visited[1024] = {0};
	int deep[1024] = {-1};
	visited[m_loc] = 0;
	deep[m_loc] = 0;
	int found_cheese = 0;
	int curr_elem;
	int visit_num = 0;
	// largely the same as bfs but how we use the heuristics that we calculate
	while(isEmptyPQueue(&head) != 1 && found_cheese == 0){
		curr_elem = headIndex(&head);
		visit_order[curr_elem % size_X][curr_elem / size_Y] = visit_num;
		visit_num++;
		for(int  i = 0; i < cheeses; i++){
			// found cheese yum leave the loop
			if(cheese_loc[i] && curr_elem == (cheese_loc[i][0] + (cheese_loc[i][1]*size_X))){
				found_cheese = 1;
			}
		}

		// adding the possible options around it to the queue still checking for ze cats
		// but as you can see now we call the heauristic function to switch up the priorities
		if((gr[curr_elem][0] == 1) && checkCat(curr_elem-32, cats, cat_loc) && (visited[curr_elem-32] == 0)){
			pushPQueue(&head, curr_elem-32, head->priority + 1 + heuristic((curr_elem - 32) % size_X, (curr_elem-32)/size_Y, cat_loc, cheese_loc, mouse_loc, 10, cheeses, gr));
			visited[curr_elem-32] = curr_elem;
			deep[curr_elem-32] = deep[curr_elem]+1;
		}
		if((gr[curr_elem][1] == 1) && checkCat(curr_elem+1, cats, cat_loc) && (visited[curr_elem+1] == 0)){
			pushPQueue(&head, curr_elem+1, head->priority + 1 + heuristic((curr_elem+1) % size_X, (curr_elem+1)/size_Y, cat_loc, cheese_loc, mouse_loc, 10, cheeses, gr));
			visited[curr_elem+1] = curr_elem;
			deep[curr_elem+1] = deep[curr_elem]+1;
		}
		if((gr[curr_elem][2] == 1) && checkCat(curr_elem+32, cats, cat_loc) && (visited[curr_elem+32] == 0)){
			pushPQueue(&head, curr_elem+32, head->priority + 1 + heuristic((curr_elem + 32) % size_X, (curr_elem+32)/size_Y, cat_loc, cheese_loc, mouse_loc, 10, cheeses, gr));
			visited[curr_elem+32] = curr_elem;
			deep[curr_elem+32] = deep[curr_elem]+1;
		}
		if((gr[curr_elem][3] == 1) && checkCat(curr_elem-1, cats, cat_loc) && (visited[curr_elem-1] == 0)){
			pushPQueue(&head, curr_elem-1, head->priority + 1 + heuristic((curr_elem - 1) % size_X, (curr_elem-1)/size_Y, cat_loc, cheese_loc, mouse_loc, 10, cheeses, gr));
			visited[curr_elem-1] = curr_elem;
			deep[curr_elem-1] = deep[curr_elem]+1;
		}
		popPQueue(&head);
	}
	// create path
	int depth = deep[curr_elem];
	while(depth != -1){
		path[depth][0] = curr_elem % size_X;
		path[depth][1] = curr_elem / size_Y;
		depth--;
		curr_elem = visited[curr_elem];
	}
	if(head != NULL){
		freeList(head);
	}
}

int H_cost(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4])
{
 /*
	This function computes and returns the heuristic cost for location x,y.
	As discussed in lecture, this means estimating the cost of getting from x,y to the goal. 
	The goal is cheese. Which cheese is up to you.
	Whatever you code here, your heuristic must be admissible.

	Input arguments:

		x,y - Location for which this function will compute a heuristic search cost
		cat_loc - Cat locations
		cheese_loc - Cheese locations
		mouse_loc - Mouse location
		cats - # of cats
		cheeses - # of cheeses
		gr - The graph's adjacency list for the maze

		These arguments are as described in the search() function above
 */

 // here we find the closest cheese using manhattan distance
 int closestX = cheese_loc[0][0];
 int closestY = cheese_loc[0][1];
 for (int i = 1; i < cheeses; i++){
	 if(abs(cheese_loc[i][0]+cheese_loc[i][1] - x - y) < abs(closestX+closestY - x - y)){
		 closestX = cheese_loc[i][0];
		 closestY = cheese_loc[i][1];
	 }
 }
 // return how far the node is from the goal ala manhattan distance
 int retVal = abs(closestX+closestY - x - y);
 return retVal;
}

int bfsCat(double gr[graph_size][4], int x, int y, int cat_loc[10][2], int cats){
	// this is a cool function that returns how many cats can reach a point in x number of steps
	int allowed_steps = 5;
	int num_cats = 0;
	for(int i = 0; i < cats; i++){
		// similar to the bfs search but we do not need to keep track of predecesors or visit_order
		struct Node* head = createNode(cat_loc[i][0]+(32*cat_loc[i][1]), 0);
		int curr_elem;
		int iteration = 0;
		int visited[1024] = {0};
		int not_reached = 1;
		while(not_reached && isEmptyPQueue(&head) != 1 && iteration < allowed_steps){
			curr_elem = headIndex(&head);
			if(curr_elem == (x + (y*size_X))){
				// if you find the spot, then increment the number of problem cats
				num_cats++;
			}

			// adding the possible options around it to the queue
			if((gr[curr_elem][0] == 1) && (visited[curr_elem-32] == 0)){
				pushPQueue(&head, curr_elem-32, head->priority + 1);
				visited[curr_elem-32] = curr_elem;
			}
			if((gr[curr_elem][1] == 1) && (visited[curr_elem+1] == 0)){
				pushPQueue(&head, curr_elem+1, head->priority + 1);
				visited[curr_elem+1] = curr_elem;
			}
			if((gr[curr_elem][2] == 1) && (visited[curr_elem+32] == 0)){
				pushPQueue(&head, curr_elem+32, head->priority + 1);
				visited[curr_elem+32] = curr_elem;
			}
			if((gr[curr_elem][3] == 1) && (visited[curr_elem-1] == 0)){
				pushPQueue(&head, curr_elem-1, head->priority + 1);
				visited[curr_elem-1] = curr_elem;
			}
			popPQueue(&head);
			iteration++;
		}
		// free the rest
		if(head != NULL){
			freeList(head);
		}
	}
	// return how many cats reach the spot
	return num_cats;
}

int H_cost_nokitty(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4])
{
 /*
	This function computes and returns the heuristic cost for location x,y.
	As discussed in lecture, this means estimating the cost of getting from x,y to the goal. 
	The goal is cheese. 

	However - this time you want your heuristic function to help the mouse avoid being eaten.
	Therefore: You have to somehow incorporate knowledge of the cats' locations into your
	heuristic cost estimate. How well you do this will determine how well your mouse behaves
	and how good it is at escaping kitties.

	This heuristic *does not have to* be admissible.

	Input arguments have the same meaning as in the H_cost() function above.
 */
 // you still wanna go to the closest cheese
int closestX = cheese_loc[0][0];
 int closestY = cheese_loc[0][1];
 for (int i = 1; i < cheeses; i++){
	 if(abs(cheese_loc[i][0]+cheese_loc[i][1] - x - y) < abs(closestX+closestY - x - y)){
		 closestX = cheese_loc[i][0];
		 closestY = cheese_loc[i][1];
	 }
 }
 // but now you also want to avoid places that have a lot of cats able to reach it
 int retVal = abs(closestX+closestY - x - y);
 int num_cats = bfsCat(gr, x, y, cat_loc, cats) * 1000;
 if(num_cats != 0){
	 retVal += num_cats;
 }
 return retVal;
}

