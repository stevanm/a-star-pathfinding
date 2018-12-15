#ifndef __PATHFINDING_H__
#define __PATHFINDING_H__


#include <stdbool.h>

#include "graph.h"
#include "queue.h"


/*
A-star algorithm with input: 

    g - graph
    start - start node
    end - end node
    heuristic_function - heuristic function for filing a search
    path - result path
    
 */
bool a_star(graph* g, node* start, node* end, float (*heuristic_function)(node* s, node* e));


#endif
