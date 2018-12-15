#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <float.h>

#include "check_error.h"
#include "node.h"

/*
 * Create new node 
 */
node* create_node(unsigned node_id, float x, float y, float z){
    
    node* n = malloc(sizeof(node));
    if(NULL == n){
        return NULL;
    }
    
    set_node_id(n, node_id);
    set_coords(n, x, y, z);
    
    set_g_func(n, FLT_MAX);
    
    set_parent_node(n, NULL);
    set_in_open_list(n, false);
    set_in_closed_list(n, false);

    
    return n;
}

/*
 * Update node
 */
void update_node(node* n, unsigned node_id, float x, float y, float z){
    
    
    set_node_id(n, node_id);
    set_coords(n, x, y, z);
    
    set_g_func(n, FLT_MAX);
    
    set_parent_node(n, NULL);
    set_in_open_list(n, false);
    set_in_closed_list(n, false);

    return;
}



/*
 * Set new node_number 
 */
void set_node_id(node* n, unsigned id){
	if(NULL == n){
        return;
    }
    n->node_id = id;
}

/*
 * Set new coordinates (x, y, z)
 */
void set_coords(node* n, float x, float y, float z){
    if(NULL == n){
        return;
    }
    n->x = x;
    n->y = y;
    n->z = z;
}

/*
 * Set real path value
 */
void set_g_func(node* n, float g_value){
    
    n->g_func = g_value;
    
    return;
}

/*
 * Set parent node
 */
void set_parent_node(node* n, node* parent_node){
    if(NULL == n){
        return;
    }
    n->parent = parent_node;
}


/*
 * Set is in open list
 */
void set_in_open_list(node* n, bool is_in_list){
    
    n->in_open_list = is_in_list;
    
    return;
}

/*
 * Set is in close list
 */
void set_in_closed_list(node* n, bool is_in_list){
    
    n->in_closed_list = is_in_list;
    
    return;
}


/*
 * Print node info
 */
void print_node(node* n){
    if(NULL == n){
        return;
    }
    
    printf("%u  (%.2f, %.2f, %.2f)\n", n->node_id, n->x, n->y, n->z);
    
   /* printf("Node ID: %u, Coordinates: (%.2f, %.2f, %.2f), True path value g = %.2f, Heuristic value h = %.2f, f = g + h = %.2f, Node adress: %p, Parent node address: %p\n", n->node_id, n->x, n->y, n->z, n->g_func, n->h_func, n->f_func, n, n->parent); */
}

/*
 * Translate point (x, y, z) to (x1, y1, z1)
 */	
void translate_node(node* n, float dx, float dy, float dz){
    if(NULL == n){
        return;
    }
    
    n->x += dx;
    n->y += dy;
    n->z += dz;
}

/*
 Is node n equals to node m?
 */
bool is_equal(node* n, node* m){   
    if(NULL == n || NULL == m){
        return false;
    }
    //return n == m;
    return n->node_id == m->node_id;
}


/************* Test implemented functions *************/
#ifdef TEST_NODE

/* Test code */
int main(){
    
    node* n = create_node(10, 1000, 1001, 1002);
    print_node(n);
    print_node(NULL);
    
    set_node_id(n, 199);
    set_node_id(NULL, 199);
    print_node(n);
    
    set_coords(n, 199, 200, 201);
    set_coords(NULL, 199, 200, 201);
    print_node(n);
    
    set_parent_node(n, n);
    set_parent_node(NULL, n);
    print_node(n);
    
    translate_node(n, 50 , 50, 50);
    translate_node(NULL, 50 , 50, 50);
    print_node(n);
    
    update_node(n, n->node_id + 1, 100, 100, 100);
    print_node(n);
    
    printf("%s", is_equal(n, n) ? "True\n" : "False\n");
    
    free(n);
    return 0;
}

#endif
