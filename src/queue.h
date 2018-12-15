#ifndef __QUEUE_H__
#define __QUEUE_H__


#include <stdbool.h>

#include "node.h"


typedef struct _q_node{
    
    node n_node;
    struct _q_node *next;
    
} q_node;


typedef struct _queue{
    
    q_node* first, *last;
    q_node* data;

    
} queue;


/* queue initialization */
void init_queue(queue* q);

q_node* create_queue_node(node* n);


/* Adds an item to the queue. If the queue is full, then it is said to be an Overflow condition. */
bool enqueue(queue* q, node* n);

/* Removes an item from the queue. The items are popped in the same order in which they are pushed. If the queue is empty, then it is said to be an Underflow condition. */
node dequeue(queue* q);

/* Get the front item from queue. */
node* front(queue* q);

/* Get the last item from queue. */
node* rear(queue* q);

void print_queue(queue* q);


#endif
