#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <GL/glut.h>

#include "graph.h"
#include "node.h"
#include "check_error.h"


/*
 * Allocate graph nodes memory 
 */	
bool allocate_graph_nodes(graph* g, unsigned num_nodes){
	            
    if(NULL == g){
        return false;
    }

	g->graph_nodes = NULL;
	
	g->graph_nodes = malloc(num_nodes * sizeof(node));
	
    if(NULL == g->graph_nodes){
		return false;
	}
	
	return true;
}	


/*
 * Allocate graph matrix of connectivity memory 
 */
bool allocate_graph_connectivity_matrix(graph* g, unsigned num_nodes){
        
    if(NULL == g){
        return false;
    }

    g->connectivity_matrix = NULL;
    
    g->connectivity_matrix = malloc(num_nodes * num_nodes * sizeof(unsigned));
    
    if(NULL == g->connectivity_matrix){
        return false;
    }
    
    return true;
}


/*
 * Dealocate graph memory for all dynamic alloc members
 */
void deallocate_graph(graph* g){
	    
    if(NULL == g){
        return;
    }

    free(g->graph_nodes);
    free(g->connectivity_matrix);
    
    return;
}


/*
 * init (create) graph
 */
 bool create_graph(graph* g, unsigned num_nodes, int dX, int dY, int dZ, float graph_sparsity, bool plain_graph){
                  
        if(NULL == g){
            printf("NULL pointer: NULL == g\n");
            return false;
        }

        g->num_nodes = num_nodes;
        
        g->open_list_size = 0;
        g-> closed_list_size = 0;

        check_error(allocate_graph_nodes(g, num_nodes), "allocate_graph_nodes (create_graph) failed\n");
        check_error(allocate_graph_connectivity_matrix(g, num_nodes), "allocate_graph_connectivity_matrix (create_graph) failed\n");
        
        int x, y, z;
            
        srand(time(NULL));

        unsigned i;
        for(i = 0; i < g->num_nodes; i++){
            
            x = (dX == 0) ? 0 : rand() % dX; 
            y = (dY == 0) ? 0 : rand() % dY; 
            
            if(plain_graph){
                z = 0; 
            }
            else{
                z = (dZ == 0) ? 0 : rand() % dZ;     
            }
            
            
            /* randomly update node coordinates and set node ID, also sets:
                parent = NULL, g_func = FLT_MAX, in_open_list = True, in_close_list = True */
            update_node(&(g->graph_nodes[i]), i, x, y, z); 
        }
        
        check_error(create_random_connectivity_matrix(g, graph_sparsity), "create_random_connectivity_matrix (create graph) failed\n");        
        
        calculate_centroid(g);
        
        
        return true;
	}


/*
* Graph information preview
*/
void print_graph_info(graph *g){
        
    if(NULL == g){
        return;
    }
	 
	printf("********** GRAPH INFO **********: \n");
	printf("Number of nodes: %u\n", g->num_nodes);
    printf("Graph centroid (x, y, z): (%f, %f, %f)\n", g->x_centroid, g->y_centroid, g->z_centroid);
    
	
	unsigned i;
	for(i = 0; i < g->num_nodes; i++){
        
        printf("\n**********************************************************\n");
        print_node(&(g->graph_nodes[i]));
        printf("\n************* NEIGHBOURS OF NODE ID %u *************\n", i);
        /* print node neighbours */
        print_node_neighbours(g, i);
        

    }
        
    return;
}

/* 
 * Print all graph nodes 
 */
void print_graph_nodes(graph *g){
	     
    if(NULL == g){
        return;
    }
    
    if(NULL == g->graph_nodes){
        return;
    }
    
	unsigned i;
	for(i = 0; i < g->num_nodes; i++){
        print_node(&(g->graph_nodes[i]));
        printf("\n");
    }
        
    return;
}

/*
 * Decides whether the node is randomly selected or not
 */
bool is_node_chosen(float graph_sparsity){

    int r = rand();
	
    if(r <= (int)(RAND_MAX * graph_sparsity)) {
        return true;
        } 
	else {
        return false;
        }
        
    return true;
}

/* 
 * Create matrix of connectivity with random value 
 */
bool create_random_connectivity_matrix(graph *g, float graph_sparsity){
	    
    if(NULL == g){
        return false;
    }
    
    unsigned i, j;
	unsigned *con_array = g->connectivity_matrix;
    unsigned n = g->num_nodes;
	
    for(i = 0; i < n; i++){
		for(j = 0; j < n; j++){
			con_array[(i * n) + j] = is_node_chosen(graph_sparsity);
		}
	}
	
	g->connectivity_matrix = con_array;
    return true;
}


/*
 * Print info about neighbours fror node at index nod_index in graph_nodes
 */
void print_node_neighbours(graph* g, unsigned node_index){
        
    if(NULL == g){
        return;
    }
    
    /*
    if(node_index >= g->num_nodes || node_index < 0){
        printf("Node index must be non-negative number between 0 and number of nodes\n");
        return;
    }
    */
    
    unsigned j;
    //unsigned* i_node = g->connectivity_matrix + (node_index * g->num_nodes);
    
    //printf("Neighbours of Node %u:\n", node_index);
    
    for(j = 0; j < g->num_nodes; j++){
        
        if(is_neighbour_node(g, node_index, j)){
            print_node(g->graph_nodes + j);
            }
        }   
    
    return;
    }


/*
 * Is node at position j_index neighbour to node at i_index
 */
bool is_neighbour_node(graph* g, unsigned i_index, unsigned j_index){
        
    if(NULL == g){
        return false;
    }
    
    /* loop is not allowed */
    if(i_index == j_index){
        return false;
    }
    
    return g->connectivity_matrix[i_index * g->num_nodes + j_index] == 1;
}


/*
 * Calculate coordinates of graph centroid
 */
bool calculate_centroid(graph* g){
    
    if(NULL == g){
        return false;
    }
    
    float x_sum = 0;
    float y_sum = 0;
    float z_sum = 0;
    
    unsigned i;
    for(i = 0; i < g->num_nodes; i++){
    
        x_sum += g->graph_nodes[i].x;
        y_sum += g->graph_nodes[i].y;
        z_sum += g->graph_nodes[i].z;
        
    }
    
    g->x_centroid = x_sum / g->num_nodes;
    g->y_centroid = y_sum / g->num_nodes;
    g->z_centroid = z_sum / g->num_nodes;
    
    return true;
}

/* Edge cost is calculated as edge distance in Euclid space. */
float edge_cost(node* n1, node* n2){
    
    if(NULL == n1 || NULL == n2){
        return FLT_MIN;
    }
    
    return sqrt( (n2->x - n1->x) * (n2->x - n1->x) + (n2->y - n1->y) * (n2->y - n1->y) + (n2->z - n1->z) * (n2->z - n1->z) );
}

/*
 Return heuristc value for node with index equal node_index
 */
float heuristic(node* n, node* end){
    
    if(NULL == n || NULL == end){
        return FLT_MIN;
        
    }
    return edge_cost(n, end);
}

/* Heuristic provide a-star algoritam simulation as Dijkstra's algorithm*/
float dijkstra_heuristic(node* n, node* end){
    
    if(NULL == n || NULL == end){
        return FLT_MIN;
        
    }
    
    return 0;
}




/* print graph path */
void print_graph_path(graph* g, node* start, node* end){
        
    if(NULL == g){
        return;
    }
    
    if(NULL == end){
        return;
    }
    
    if(NULL == start){
        return;
    }
    
    print_graph_path(g, start, end->parent);
    
    print_node(end);
}

/* empty open list? */
bool is_open_list_empty(graph* g){
    
    return 0 == g->open_list_size;
    
}

/* empty closed list? */
bool is_closed_list_empty(graph* g){

    return 0 == g->closed_list_size;
    
}

/* is node in open list */
bool is_node_in_open_list(graph* g, node* n){
    
    return (g->graph_nodes[n->node_id]).in_open_list;
}

/* is node in closed list */
bool is_node_in_closed_list(graph* g, node* n){
    
    return (g->graph_nodes[n->node_id]).in_closed_list;
}



/************* Test implemented functions *************/
#ifdef TEST_GRAPH

/* Test code */
int main(){
    
    graph g;

    check_error(create_graph(&g, 10, 50, 60, 30, 0.5, 0), "create_graph (main) failed\n");
    //check_error(create_graph(NULL, 10, 50, 60, 30, 0.5, 0), "create_graph (main) failed\n");
    //check_error(create_graph(&g, 10, 50, 60, 30, 0.5, 3), "create_graph (main) failed\n");
    
    //print_graph_nodes(&g);
    
    //create_random_connectivity_matrix(&g, 0.5);
    
    //print_node_neighbours(&g, 0);
    //print_node_neighbours(NULL, 0);
    //print_node_neighbours(&g, 11);
    //print_node_neighbours(&g, -11);
    
    //printf("%s\n", is_neighbour_node(&g, 0, 0) ? "YES, it is neighbour node" : "NO, it is not neighbour node");

    //node* start = &(g.graph_nodes[0]);
    // printf("Edge cost between node 0 and 0 is: %f\n", edge_cost(start, start));
    // printf("Heuristic between node 0 and 0 is: %f\n", heuristic(start, start));
    
    print_graph_info(&g);
    

    
    return 0;
}



#endif
