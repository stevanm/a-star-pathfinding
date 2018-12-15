#define _DEFAULT_SOURCE 700

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <GL/glut.h>
#include <unistd.h>

#include "pathfinding.h"
#include "queue.h"
#include "graph.h"
#include "node.h"
#include "check_error.h"

/*
----------------------------------------------------------------------------------------------------------
Input: Graph g, start node "star" and target node "end"
Output: The shortest path from the start to the target node (if there is a path between these two nodes)
----------------------------------------------------------------------------------------------------------
*/

//enqueue

bool a_star(graph* g, node* start, node* end, float (*heuristic_function)(node* s, node* e)){

    
    /* add node to queue for simulating */
    // check_error(enqueue(q, start), "enqueue failed\n");
    
    
    if(NULL == g){
        return false;
    }
    
    if(NULL == start){
        return false;
    }

    if(NULL == end){
        return false;
    }
    
    /*
    1. put the closed list to be empty list, put in the open list only the starting node;
    */

    /* closed list and open list are empty at the start ---> see graph structure and create_graph function */
    g->graph_nodes[start->node_id].in_open_list = true;
    g->open_list_size++; /* one node is added to open list */
    

    start->g_func = 0;
    /* start->h_func = heuristic_function(start, end); */
    /* start->f_func = start->g_func + start->h_func; */
        
    
    node* n, *v;
    unsigned m_index, n_index;
    unsigned i;
    
    /* 1. as long as there are elements in the open list works */        
      while( g->open_list_size > 0 ){

        n = NULL;
        
        /* 2.1 select node n (current node) from the open list with the best grade f (n); */
        for(i = 0; i < g->num_nodes; i++){
            
            if((g->graph_nodes[i]).in_open_list){
                
                v = &(g->graph_nodes[i]);
                
                if( (FLT_MAX != (g->graph_nodes[i]).g_func) && \
                    (NULL == n || (g->graph_nodes[v->node_id]).g_func +  heuristic_function(&(g->graph_nodes[v->node_id]), end) < \
                    (g->graph_nodes[n->node_id]).g_func +  heuristic_function(&(g->graph_nodes[n->node_id]), end)) ){
                        n = v;
                    }
            }
        }
        
        
        if(NULL == n){ 
            break;  /* path not found */
        } 
        
        /*  
        2.2 If n is the target node then {
            2.2.1 report on success and return the solution by constructing the path from the starting point to the target node (going backwards from the target node);  
            }
        */
        if(is_equal(n, end)){
            
            set_in_open_list(&(g->graph_nodes[n->node_id]), false);
            g->open_list_size--; 
            
            set_in_closed_list(&(g->graph_nodes[n->node_id]), true);
            g->closed_list_size++;    
            
            
            printf("*************** Path found! ***************\n");
            printf("Path nodes:\n");
            print_graph_path(g, &(g->graph_nodes[start->node_id]), &(g->graph_nodes[end->node_id]));
            printf("*******************************************\n");
            
            return true;
        }
        
        
        /* 2.3 for each node that is directly accessible from n runs */        
        n_index = n->node_id;
        
        for(m_index = 0; m_index < g->num_nodes; m_index++){   
        
            if(is_neighbour_node(g, n_index, m_index)){
                
                /* 2.3.1. if m is not in the open or closed list then */
                if( !is_node_in_open_list(g, &(g->graph_nodes[m_index]) ) && \
                    !is_node_in_closed_list(g,  &(g->graph_nodes[m_index]) ) ){
                    /* 2.3.1.1 Add it to the open list and mark n as his parent.
                     * 2.3.1.2 Calculate and associate the value of f (m) to the m;
                     */
                    
                    set_in_open_list(&(g->graph_nodes[m_index]), true);
                    g->open_list_size++; /* increment open list size after element is added */ 
                
                    (g->graph_nodes[m_index]).parent = &(g->graph_nodes[n_index]);

                    
                    (g->graph_nodes[m_index]).g_func = (g->graph_nodes[n_index]).g_func + \
                                                                edge_cost(&(g->graph_nodes[n_index]), &(g->graph_nodes[m_index]));
                    
                    }
                    
                /* 2.3.2 otherwise */
                else {
                    /*
                    2.3.2.1. If the path from the starting node to the node m over the node is better (shorter or cheaper)
                        from the current path to m (the current value of g (m)) then {
                            2.3.2.1.1 change the information on the parent node to the node and update the values of f (m)
                                    and if m was on the closed list, transfer it to the open;
                                }
                            }
                        */
                        if((g->graph_nodes[n_index]).g_func + edge_cost(&(g->graph_nodes[n_index]), &(g->graph_nodes[m_index])) <  (g->graph_nodes[m_index]).g_func){
                                    
                            (g->graph_nodes[m_index]).g_func = (g->graph_nodes[n_index]).g_func + edge_cost(&(g->graph_nodes[n_index]), &(g->graph_nodes[m_index]));
                            
                            (g->graph_nodes[m_index]).parent = &(g->graph_nodes[n_index]);
                            
                            
                            if(is_node_in_closed_list(g, &(g->graph_nodes[m_index]))){
                                
                                set_in_closed_list(&(g->graph_nodes[m_index]), false);
                                g->closed_list_size--; /* deincrement closed list size after element is removed */ 
                                
                                set_in_open_list(&(g->graph_nodes[m_index]), true);
                                g->open_list_size++; /* increment open list size after element is added */ 
                                
                                }
                                
                            }  
                    }
                }
               
            }
             
            /* 2.4 remove n from the open list and add it to the closed list - it was already ejected at the beginning of the algorithm */
                                
            set_in_open_list(&(g->graph_nodes[n_index]), false);
            g->open_list_size--;  
            
            set_in_closed_list(&(g->graph_nodes[n_index]), true);
            g->closed_list_size++;   
    }
    
    /* 3. Report that the requested path does not exist (the open list is empty and success is not reported). */
    printf("*** Path NOT found! ***\n");

    return true;    

}

/************* Test implemented functions *************/
#ifdef TEST_PATHFINDING 

/* Test code */
int main(){
    
    graph g;
    check_error(create_graph(&g, 1000, 500, 600, 300, 0.2, 0), "create_graph (main) failed\n");

    node* start = &(g.graph_nodes[0]);
    node* end = &(g.graph_nodes[999]);
    
    check_error(a_star(&g, start, end, heuristic), "a-star failed\n");
    
    
    return 0;
}

#endif
