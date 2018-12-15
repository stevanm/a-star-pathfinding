#ifndef __GRAPH_H__
#define __GRAPH_H__


#include <stdbool.h>

#include "node.h"
#include "queue.h"


typedef struct _graph{
	
	unsigned num_nodes; 
	node* graph_nodes;
    unsigned *connectivity_matrix;
    
    float x_centroid, y_centroid, z_centroid;
    
    /* Need for A-star algorithm */
    int open_list_size, closed_list_size;
	
} graph;


/*
 * Allocate graph nodes memory 
 */
bool allocate_graph_nodes(graph* g, unsigned num_nodes);

/*
 * Allocate graph matrix of connectivity memory 
 */
bool allocate_graph_connectivity_matrix(graph* g, unsigned num_nodes);

/*
 * Dealocate graph memory for all dynamic alloc members
 */
void deallocate_graph_memory(graph* g);

/*
 * init (create) graph
 */
 bool create_graph(graph* g, unsigned num_nodes, int dX, int dY, int dZ, float graph_sparsity, bool plain_graph);
 
 /*
 * graph information preview
 * */
void print_graph_info(graph* g);

/* 
 * Print all graph nodes 
 */
void print_graph_nodes(graph *g);

/*
 * Decides whether the node is randomly selected or not
 * */
bool is_node_chosen(float graph_sparsity);

/* 
 * Create matrix of connectivity with random value 
 */
bool create_random_connectivity_matrix(graph *g, float graph_sparsity);

/*
 * Print info about neighbours fror node at index nod_index in graph_nodes
 */
void print_node_neighbours(graph* g, unsigned node_index);

/*
 * Is node at position j_index neighbour to node at i_index
 */
bool is_neighbour_node(graph* g, unsigned i_index, unsigned j_index);

/*
 * Calculate coordinates of graph centroid
 */
bool calculate_centroid(graph* g);

/* Edge cost is calculated as edge distance in Euclid space. */
float edge_cost(node* n1, node* n2);

/*
 Return heuristc value for node with index equal node_index
 */
float heuristic(node* n, node* end);

/* Heuristic provide a-star algoritam simulation as Dijkstra's algorithm*/
float dijkstra_heuristic(node* n, node* end);

/* print graph path */
void print_graph_path(graph* g, node* start, node* end);

/* empty open list? */
bool is_open_list_empty(graph* g);

/* empty closed list? */
bool is_closed_list_empty(graph* g);

/* is node in open list */
bool is_node_in_open_list(graph* g, node* n);

/* is node in closed list */
bool is_node_in_closed_list(graph* g, node* n);


#endif

