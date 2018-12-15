#ifndef __NODE_H__
#define __NODE_H__

#include <stdbool.h>


typedef struct _node{
	
	unsigned node_id;
	float x, y, z;
	
    /***** Data requred for A-star algorithm *****/
    float g_func; /* current value */
    //float h_func; /* heuristic */    
    //float f_func; /* f_func = g_func + h_func */
    
    struct _node* parent; /* paren node */
    bool in_open_list;  /* if True node is in open list */
    bool in_closed_list; /* if True node is in close list */
     
} node;


/*
 * Create new node 
 */
node* create_node(unsigned node_number, float x, float y, float z);

/*
 * Update node
 */
void update_node(node* n, unsigned node_id, float x, float y, float z);


/*
 * Set new node_number 
 */
void set_node_id(node* n, unsigned id);

/*
 * Set new coordinates (x, y, z)
 */
void set_coords(node* n, float x, float y, float z);

/*
 * Set real path value
 */
void set_g_func(node* n, float g_value);

/*
 * Set parent node
 */
void set_parent_node(node* n, node* parent_node);

/*
 * Set is in open list
 */
void set_in_open_list(node* n, bool is_in_list);

/*
 * Set is in close list
 */
void set_in_closed_list(node* n, bool is_in_list);


/*
 * Print node info
 */
void print_node(node* n);

/*
 * Translate point (x, y, z) to (x1, y1, z1)
 */
void translate_node(node* n, float dx, float dy, float dz);

/*
 Is node n equals to node m?
 */
bool is_equal(node* n, node* m);

#endif
