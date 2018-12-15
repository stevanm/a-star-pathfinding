#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <GL/glut.h>

#include "pathfinding.h"
#include "node.h"
#include "graph.h"
#include "queue.h"
#include "check_error.h"

#include "graphic.h"


int timer_active = 0;

graph g;
queue q;
node* start = NULL;
node* end = NULL;
bool algorithm_finished = false;
bool algoritham_simulation_started = false;

float x_camera, y_camera, z_camera;
float dist, delta_fi = 0, delta_teta = 0;

float t = 0, dt = 0.007;
q_node* first_node = NULL, *second_node = NULL;

bool start_node_defined = false;
bool end_node_defined =  false;

bool plain_graph = false;
legend l;

int main(int argc, char** argv){
	
    check_error(argc >= 3, "\n./a-star-simulation\tnodes_number\tgraph_sparsity\t[-p (plane graph)]\n\nnodes_number - Number of graph nodes\ngraph_sparsity - Graph density parameter [0, 1]\np - flag for plane graph generation\n");
    
    
	init_glut(argc, argv);
    
	int num_nodes = atoi(argv[1]);
	check_error(num_nodes >= 0 , "Number of nodes must be greater then 0.\n");
    
    float graph_sparsity = atof(argv[2]);
    check_error(graph_sparsity >= 0 && graph_sparsity <= 1, "Graph sparsity must be between 0 and 1.\n");
    
    
    if(argc >= 4){
        if( 0 == strcmp("-p", argv[3]) || 0 == strcmp("-P", argv[3]) ){
            plain_graph = true;
        }   
    }
    
	create_graph(&g, num_nodes, 500, 500, 500, graph_sparsity, plain_graph);
    
    /* Init cam position */
    x_camera = 0;
    y_camera =  0; 
    z_camera = 400;
    delta_fi = 0;
    delta_teta = 0;
    dist = z_camera;
    
    
    glutMainLoop();

	return 0;
}




