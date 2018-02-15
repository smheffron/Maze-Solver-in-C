#include "../include/graph.h"
#include"../include/input_error.h"

//credit to Prof. Chada

// Initializes a graph with number_of_vertices as the number of vertices. 

void init_graph(struct graph* grph, unsigned int number_of_vertices)
{
    unsigned int i;
    grph -> num_vertices = number_of_vertices;
    grph -> vertices= malloc(sizeof(struct vertex)*(number_of_vertices +1));
   
    for (i=0; i< number_of_vertices; i++)
    {   
        init_vector(&(grph->vertices[i].adj));
    }
}


// Returns the adjacency list of a vertex
struct vector *adj_list (struct graph *grph, unsigned int vertex)
{
    if (vertex >0)
    {
        return (&(grph->vertices[vertex].adj));
    }
    exit(VERTEX_OUT_OF_BOUNDS);
}

// Returns the number of vertices in the graph
unsigned int num_vertices (struct graph* grph)
{
   
    return (grph->num_vertices);
}

// Adds an edge to the graph
void insert_edge (struct graph* grph, unsigned int src, unsigned int dest)
{

        insert_element_vector(&(grph->vertices[src].adj), dest);
        insert_element_vector(&(grph->vertices[dest].adj), src);

        grph->vertices[src].number = src;
        grph->vertices[dest].number = dest;
    

}

// Frees the memory allocated to the adjacency lists in a graph
void free_graph(struct graph *grph)
{
    unsigned int i;
    for (i=0; i < grph->num_vertices; i++)
    free_vector (&(grph->vertices[i].adj));
    free(grph->vertices);
    grph -> num_vertices =0;
    
}

//Prints the graph.
void print_graph(struct graph *grph)
{
   unsigned int i;
    
    for (i=0; i < grph->num_vertices; i++)
    {   printf("The adjacency list for vertex %u is:\n",i); 
        print_vector(&(grph->vertices[i].adj));
    }
}

