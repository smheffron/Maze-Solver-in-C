#ifndef H_GRAPH_H
#define H_GRAPH_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/vector.h"


enum COLOR {black,grey};

// This structure is for a vertex. adj is the adjacency list of the vertex
struct vertex
{
    int number;
    struct vector adj;
    int d;
    enum COLOR color;
    
};


// The graph structure stores the number of vertices in the graph and an array of vertices
struct graph
{
    unsigned int num_vertices;
    struct vertex *vertices;
    
};
enum graph_errors
{
    VERTEX_OUT_OF_BOUNDS = -1
};


void init_graph(struct graph* grph, unsigned int number_of_vertices);
struct vector *adj_list (struct graph *grph, unsigned int vertex);
unsigned int num_of_vertices (struct graph* grph);
void insert_edge (struct graph* grph, unsigned int src, unsigned int dest);
void print_graph(struct graph *grph);
void free_graph(struct graph *grph);
#endif
