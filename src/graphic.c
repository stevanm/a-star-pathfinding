#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <GL/glut.h>

#include "queue.h"
#include "node.h"
#include "graph.h"
#include "graphic.h"
#include "pathfinding.h"
#include "check_error.h"


#define UNUSED(x) (void)(x)
#define BUFSIZE 500


/* GLUT initialization */


void init_glut(int argc, char **argv){
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(900, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);
    
    GLfloat light_ambient[] = { 0, 0, 0, 1 };
    GLfloat light_diffuse[] = { 1, 1, 1, 1 };
    GLfloat light_specular[] = { 1, 1, 1, 1 };
    GLfloat model_ambient[] = { 0.4, 0.4, 0.4, 1 };
    
    glClearColor(0, .9, 1.0, 0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    glEnable(GL_COLOR_MATERIAL);
    
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);
    
}

/****** GLUT callback functions  ******/

/* keyboard glut callback */
void on_keyboard(unsigned char key, int x, int y){
    
    UNUSED(x);
    UNUSED(y);
    
    
    switch (key) {
    
    /* ESCAPE */
    case 27:
                        /* Program is finished. */
                        exit(0);
                        break;
    
    /* Go */
    case 'g':
    case 'G':
                        /* Simulation starts. */
                        if (!timer_active) {
                            glutTimerFunc(TIMER_INTERVAL, on_timer, 0);
                            timer_active = 1;
                            
                            /* Start algorithm simulation */
                            if(start_node_defined && end_node_defined){
                                algorithm_finished = a_star(&g, start, end, heuristic);
                                
                                get_legend_data(&g, &l); /* get legend data */
                            }
                            else {
                                algorithm_finished = false;
                            }
                        }
                        break;
    case 'q':
    case 'Q':
                        {
                            algoritham_simulation_started = true;
                            
                            first_node = q.first;
                            second_node = (q.first)->next;
                       
                        }
                        break;
                        
    /* STOP */
    case 'p':
    case 'P':
                        /* Simulation stops. */
                        timer_active = 0;
                        break;

    case 'a': 
                        delta_fi -= 0.1;
                        break;
    
    case 'd':
                        delta_fi += 0.1;
                        break;
    
    case 'w':             
                         delta_teta = (delta_teta + 0.1 < M_PI/2) ? (delta_teta += 0.1) : M_PI/2;
                        // delta_teta -= 0.1;
                        break;
    
    case 's':               
                        delta_teta = (delta_teta - 0.1 > -M_PI/2) ? (delta_teta -= 0.1) : -M_PI/2; 
                        // delta_teta += 0.1;
                        break;
        
    case 'b':           dist += 5;
                        break;
    
    case 'f':
                        dist -= 5;
                        break;
                        
    case 'n':           {
                            if(!start_node_defined){
                                int start_node;
                                printf("Input START node: ");
                                scanf("%d", &start_node);
                                start = &(g.graph_nodes[start_node]);
                                start_node_defined =  true;
                                /* printf("START: %d\n", start_node); */
                            }
                        }
                        break;
    
    case 'm':            {
                            if(!end_node_defined){
                                int end_node;
                                printf("Input END node: ");
                                scanf("%d", &end_node);
                                end = &(g.graph_nodes[end_node]);
                                end_node_defined =  true; 
                                /* printf("end: %d\n", end_node); */
                            }
                        }
                        break;
    }

    glutPostRedisplay();
    
}


/* timer glut callback */
void on_timer(int value)
{
    if (value != 0){
        return;
    }

    /* t += dt; */
    t += dt;
    
    
    glutPostRedisplay();

    if (timer_active)
        glutTimerFunc(TIMER_INTERVAL, on_timer, 0);
}

/* reshape glut callback */
void on_reshape(int width, int height){
     /* Set viewport. */
    glViewport(0, 0, width, height);

    /* Set projection parameters. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float) width / height, 1, 10000);  
}

/* display glut callback */
void on_display(void){
    GLfloat light_position[] = { 1, 1, 1, 0 };

    GLfloat no_material[] = { 0, 0, 0, 1 };
    GLfloat material_ambient[] = { 0.7, 0.7, 0.7, 1 };
    GLfloat material_diffuse[] = { 0.1, 0.5, 0.8, 1 };
    GLfloat material_specular[] = { 1, 1, 1, 1 };
    GLfloat high_shininess[] = { 100 };
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_material);
        
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    /* All points are translated for centroid vector 
     * and camera direction point is to (0, 0, 0)*/
    gluLookAt(g.x_centroid + dist * sin(delta_fi) * cos(delta_teta), \
              g.y_centroid + dist * sin(delta_teta), \
              g.z_centroid + dist * cos(delta_fi) * cos(delta_teta), \
              g.x_centroid, g.y_centroid, g.z_centroid, \
             0, 1, 0);
    
  
    /* Draw graph */
    draw_graph(&g); 
    
    
    /* Draw final graph if animation is finished */ 
    if(algorithm_finished){
       
        draw_graph(&g); 
        draw_graph_path(&g, &(g.graph_nodes[start->node_id]), &(g.graph_nodes[end->node_id]), &q); 
       
       if(algoritham_simulation_started){

           
            if((g.graph_nodes[end->node_id]).node_id == ((first_node->next)->n_node).node_id){                            
                algoritham_simulation_started = 0;
                }
        
            t = draw_line_segment(&(first_node->n_node), &(second_node->n_node)); 
            
            /*
            print_node(&(first_node->n_node));
            print_node(&(second_node->n_node));
            */
            
            /* printf("PARAMETRA T: %f\n", t); */
            if(t >= 1){ 
                
                t = 0; 
                
                first_node = second_node;
                if(NULL == second_node){ 
                    timer_active = 0; 
                    }
                else{
                    second_node = first_node->next; 
                    }
            }
        }
    }
	
    glutSwapBuffers();

}



/****** Drawing function *******/

/* Draw graph which nodes are in specific type of lists */

/*
 Red nodes      =   nodes in open list
 Green nodes    =   nodes in closed list
 Blue nodes     =   nodes never been in open or closed list
 */
void draw_graph(graph* g){
    
    /* unsigned to string conversion */
    char node_id_string[10];
          
    unsigned i, j;
    
    for(i = 0; i < g->num_nodes; i++){
        
        /* prepare data for test output */
        sprintf(node_id_string, "%u", (g->graph_nodes[i]).node_id);
        display_node_text((g->graph_nodes[i]).x, (g->graph_nodes[i]).y, (g->graph_nodes[i]).z, node_id_string, 10);

        /*default node colour */
        glColor3f(0.0f, 0.0f, 0.0f);  

        
        /* if node is only in open list */
        /* open list = TRUE, closed list = FALSE*/
        if((g->graph_nodes[i]).in_open_list && !(g->graph_nodes[i]).in_closed_list){
        
             /* Draw nodes */
            glColor3f(1.0f, 0.0f, 0.0f);  
            
            draw_node((g->graph_nodes[i]).x, (g->graph_nodes[i]).y, (g->graph_nodes[i]).z, 5);
            
                
            
            /* Draw all neighbour edges */
            for(j = 0; j < g->num_nodes; j++){
                if(is_neighbour_node(g, i, j)){
                    
                    /* Draw edges */
                    glColor3f(1.0f, 1.0f, 1.0f); 
                    
                    draw_edge((g->graph_nodes[i]).x, (g->graph_nodes[i]).y, (g->graph_nodes[i]).z, \
                        (g->graph_nodes[j]).x, (g->graph_nodes[j]).y, (g->graph_nodes[j]).z);
                    }
                }

            
            /*default node colour */
            glColor3f(0.0f, 0.0f, 0.0f); 
        }
            
         /* if node is only in closed list */
         /* open list = FALSE, closed list = TRUE */
        else if(!(g->graph_nodes[i]).in_open_list && (g->graph_nodes[i]).in_closed_list){
            
            /* Draw nodes */
            glColor3f(0.0f, 1.0f, 0.0f);  
            draw_node((g->graph_nodes[i]).x, (g->graph_nodes[i]).y, (g->graph_nodes[i]).z, 5);
            
            
            /* Draw all neighbour edges */   
            for(j = 0; j < g->num_nodes; j++){
                if(is_neighbour_node(g, i, j)){
                    
                    /* Draw edges */
                    glColor3f(1.0f, 1.0f, 1.0f); 
                    
                    draw_edge((g->graph_nodes[i]).x, (g->graph_nodes[i]).y, (g->graph_nodes[i]).z, \
                        (g->graph_nodes[j]).x, (g->graph_nodes[j]).y, (g->graph_nodes[j]).z);
                    }
                }

                        
            /*default node colour */
            glColor3f(0.0f, 0.0f, 0.0f); 
            
        }
        /* Node is neither in open neither closed list */
        /* open list = FALSE, closed list = FALSE */
        else {
        
            /* Draw nodes */
            glColor3f(0.0f, 0.0f, 0.0f);  
            draw_node((g->graph_nodes[i]).x, (g->graph_nodes[i]).y, (g->graph_nodes[i]).z, 5);

            
            /* Draw all neighbour edges */
            for(j = 0; j < g->num_nodes; j++){
                if(is_neighbour_node(g, i, j)){
            
                    /* Draw edge*/
                    glColor3f(1.0f, 1.0f, 1.0f);
                    
                    draw_edge((g->graph_nodes[i]).x, (g->graph_nodes[i]).y, (g->graph_nodes[i]).z, \
                        (g->graph_nodes[j]).x, (g->graph_nodes[j]).y, (g->graph_nodes[j]).z);  
                    }
                }

            
            /*default node colour */
            glColor3f(0.0f, 0.0f, 0.0f); 
            
        }
        
    }
    
}

/* Draw graph path */
void draw_graph_path(graph* g, node* start, node* end, queue* q){
        
    if(NULL == g){
        return;
    }
    
    if(NULL == end){
        return;
    }
    
    if(NULL == start){
        return;
    }

    
    glColor3f(0.0f, 0.0f, 1.0f);                 
    
    node* i;
    for(i = end; i != NULL;){
              
        /* Draw all path nodes */
        glPushMatrix();
            glTranslatef((g->graph_nodes[i->node_id]).x, (g->graph_nodes[i->node_id]).y, (g->graph_nodes[i->node_id]).z);
            glutSolidSphere(6, 100, 100);     
        glPopMatrix();
        
        
        
        /* Draw path edges */ 
        glLineWidth(4);
   
        
        if(NULL == i->parent){
            break;
        }
        
        glPushMatrix();  
        
        glPushAttrib(GL_ENABLE_BIT); 
        
        glLineStipple(8, 0xAAAA);
        glEnable(GL_LINE_STIPPLE); 
        
        glBegin(GL_LINES);
        
        glVertex3f((g->graph_nodes[i->node_id]).x, (g->graph_nodes[i->node_id]).y, (g->graph_nodes[i->node_id]).z);
        
        /* add node to queue for simulating */
        check_error(enqueue(q, i), "enqueue failed\n");
        
        i = i->parent;
        
        glVertex3f((g->graph_nodes[i->node_id]).x, (g->graph_nodes[i->node_id]).y, (g->graph_nodes[i->node_id]).z);
    
        /* add node to queue for simulating */
        check_error(enqueue(q, i), "enqueue failed\n");
        
        glEnd();
        glPopMatrix();
        
        glPopAttrib();
        
    
        }
        
        glLineWidth(1);

        glColor3f(0.0f, 0.0f, 0.0f);                 
   
}


/* Display info about node -- node_id text*/
void display_node_text(float x, float y, float z, char *message, int node_distance){
    
  /* glColor3f(0,0,0); */
  
  glPushMatrix();
  glDisable(GL_LIGHTING);
  
  glTranslatef(x - node_distance/2.0, y + node_distance/1.5 , z);
  glScalef(0.08,0.08,0.08); 
  
  while (*message) {
    glutStrokeCharacter(GLUT_STROKE_ROMAN, *message);
    message++;
  }
  glEnable(GL_LIGHTING);
  glPopMatrix();
}


/* Draw path of walking  - simulation */
float draw_line_segment(node* A, node* B){
        
    if(NULL == A || NULL == B){
        exit(EXIT_FAILURE);
    }

    glColor3f(0.3f, 0.8f, 0.3f);                 

    float x_p, y_p, z_p;

    if(t >= 0 && t < 1){
            
        x_p = A->x + t * (B->x - A->x);
        y_p = A->y + t * (B->y - A->y);
        z_p = A->z + t * (B->z - A->z);
        
        /* Draw current animation point position */
        glPushMatrix();
            glTranslatef(x_p, y_p, z_p);
            glutSolidSphere(5, 100, 100);     
        glPopMatrix();
        
    }

    return t;
       
}


/* Legend  - description od node types */
/*
void draw_legend_text(float x, float y, legend* l){
    
  va_list args;
  char buffer[200], *p;

  va_start(args, format);
  vsprintf(buffer, format, args);
  va_end(args);
  glPushMatrix();
  glTranslatef(x, y, 0);
  for (p = buffer; *p; p++)
    glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
  glPopMatrix();



}
*/

/* get legend data for drawing */
void get_legend_data(graph* g, legend* l){
   
    
    int open_node_list_number = 0;
    int closed_node_list_number = 0;
        
    unsigned i;
    for(i = 0; i < g->num_nodes; i++){
        
        if(is_node_in_open_list(g, &(g->graph_nodes[i]))){
            open_node_list_number++;
        }
        
        if(is_node_in_closed_list(g, &(g->graph_nodes[i]))){
            closed_node_list_number++;
        } 
    }
    
    l->open_node_list_number = open_node_list_number;
    l->closed_node_list_number = closed_node_list_number;
    l->unvisited_node_number = g->num_nodes - (l->open_node_list_number + l->closed_node_list_number);
    
    printf("Open list nodes: %d\nClosed list nodes: %d\nUnvisited node: %d\n", l->open_node_list_number, l->closed_node_list_number, l->unvisited_node_number);
    
}


/* Draw node */
void draw_node(float x, float y, float z, float radius){
    
    glPushMatrix();
        glTranslatef(x, y, z);
        glutSolidSphere(radius, 100, 100);     
    glPopMatrix();
       
}


/* draw edge */
void draw_edge(float x1, float y1, float z1, float x2, float y2, float z2){
    
    glPushMatrix();
    glBegin(GL_LINES);                    
        
        glVertex3f(x1, y1, z1); 
        glVertex3f(x2, y2, z2);            
        
    glEnd();
    glPopMatrix();
    
}
