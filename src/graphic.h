#ifndef __GRAPHIC_H__
#define __GRAPHIC_H__

#include "queue.h"
#include "node.h"
#include "graph.h"

#include "global.h"


typedef struct {
    
    int open_node_list_number;
    int closed_node_list_number;
    int unvisited_node_number;
       
} legend;


/****** GLobal variable initializatio ******/

extern int timer_active;

extern graph g;
extern queue q;
extern node* start;
extern node* end;
extern bool algorithm_finished;
extern bool algoritham_simulation_started;

extern float x_camera, y_camera, z_camera;
extern float dist, delta_fi, delta_teta;

extern float t, dt;
extern q_node* first_node, *second_node;

extern bool start_node_defined;
extern bool end_node_defined;


extern bool plain_graph;
extern legend l;


/****** GLUT initialization ******/
void init_glut(int argc, char **argv);

/****** GLUT callback functions  ******/
extern void on_keyboard(unsigned char key, int x, int y);
extern void on_timer(int value);
extern void on_reshape(int width, int height);
extern void on_display(void);


/****** Drawing function ******/

/* Draw graph */
void draw_graph(graph* g);

/* Draw graph path */
void draw_graph_path(graph* g, node* start, node* end, queue* q);

/* print label as node_id for every node */
void display_node_text(float x, float y, float z, char *message, int node_distance);

/* Draw path of walking  - simulation */
float draw_line_segment(node* A, node* B);

/* Legend  - description od node types */
/* void draw_legend_text(float x, float y, legend* l); */

/* get legend data for drawing */
void get_legend_data(graph* g, legend* l);

/* Draw node */
void draw_node(float x, float y, float z, float radius);

/* draw edge */
void draw_edge(float x1, float y1, float z1, float x2, float y2, float z2);
    


#endif
