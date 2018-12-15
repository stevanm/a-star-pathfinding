#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "node.h"
#include "queue.h"
#include "check_error.h"


/* queue initialization but without data */
void init_queue(queue* q){
    
    if(NULL == q){
        return;
    } 
    
    q->first = NULL;
    q->last = NULL;
    
    q->data = NULL;

}


/* create queue node */
q_node* create_queue_node(node* n){
    
    if(NULL == n){
        return NULL;
    }
    
    q_node* m = malloc(sizeof(q_node));
    if(NULL == m){
        return NULL;
    }
    
    (m->n_node).node_id = n->node_id;
    (m->n_node).x = n->x;
    (m->n_node).y = n->y;
    (m->n_node).z = n->z;
    
    m->next = NULL;
    
    
    return m;
}


/* Adds an item to the queue. If the queue is full, then it is said to be an Overflow condition. */
bool enqueue(queue* q, node* n){
    
    if(NULL == q){
        return false;
    }
    
    if(NULL == n){
        return false;
    }
    
    q_node* m = create_queue_node(n);
        
    if(NULL == m){
        return false;
    }
    
    /* queue is empty */
    if(NULL == q->data){
                       
        q->first = m;
        q->last = m;
        
        q->data = m;
        
        return true;
    }
 
 
    (q->last)->next = m;
    q->last = m;

    return true;
}

/* Removes an item from the queue. The items are popped in the same order in which they are pushed. If the queue is empty, then it is said to be an Underflow condition. */
node dequeue(queue* q){
    
    if(NULL == q){
        fprintf(stderr, "%s\n", "NULL pointer exception");
        exit(EXIT_FAILURE);
    }
    
    if(NULL == q->first && NULL == q->last){
        fprintf(stderr, "%s\n", "Queue is empty");
        exit(EXIT_FAILURE);
    }
    
    node n;
    
    /* only one node in queue */
    if(q->first == q->last){
        
        n = (q->first)->n_node;
        q_node* qn = q->first;
        free(qn);
        
        (q->first) = NULL;
        (q->last) = NULL;
    }
    else {
        
        n = (q->first)->n_node;
        q_node* qn = q->first;
        q->first = (q->first)->next;
        free(qn);
    }
    
    return n;
    
}

/* Get the front item from queue. */
node* front(queue* q){
    
   return &((q->first)->n_node);
    
}

/* Get the last item from queue. */
node* rear(queue* q){

    return &((q->last)->n_node);

}


void print_queue(queue* q){
    
    q_node* qn = q->first;
    
    while(NULL != qn){
        
        print_node(&(qn->n_node));
        
        qn = qn->next;
    }
    
    
}

/************* Test implemented functions *************/
#ifdef TEST_QUEUE

/* Test code */
int main(){
    
    
    queue q;
    
    init_queue(&q);

    
    /*
    node* m = create_node(10, 100, 101, 99);
    q_node* qn = create_queue_node(m);
    print_node(&(qn->n_node));
    */
    
    /*
    node* n = create_node(10, 100, 101, 99);
    check_error(enqueue(&q, n), "enqueue failed\n");
    free(n);
    
    n = create_node(11, 150, 198, 899);
    check_error(enqueue(&q, n), "enqueue failed\n");
    free(n);    
    
    n = create_node(101, 50, 18, 8);
    check_error(enqueue(&q, n), "enqueue failed\n");
    free(n);
    print_queue(&q);
    */
    
    /*
    node p = dequeue(&q);
    print_node(&p);
    
    p = dequeue(&q);
    print_node(&p);
    
    p = dequeue(&q);
    print_node(&p);
    
    p = dequeue(&q);
    print_node(&p);
    
    print_queue(&q);
    */
    
    /*
    node * f = front(&q);
    print_node(f);
    
    node* r = rear(&q);
    print_node(r);
    
    */
    
    return 0;
}

#endif
