#include "../include/vector.h"
#include "../include/graph.h"
#include"../include/input_error.h"

//credit to prof. Chada

void init_vector(struct vector* v)
{
	v->data = malloc(sizeof(unsigned int) * INIT_VECTOR_SIZE);
	v->size = 0;
	v->capacity = INIT_VECTOR_SIZE;
}

int access_element_vector(struct vector* v, unsigned int index)
{
	if(index >= v->size)
		return -2;
	return v->data[index];
}

void insert_element_vector(struct vector* v, unsigned int element_to_insert)
{
	if(v->capacity == v->size)
	{
		v->data = realloc(v->data, sizeof(int) * v->capacity * 2);
		v->capacity *= 2;
	}
	v->data[v->size] = element_to_insert;
	v->size += 1;
}

unsigned int* vector_get_ptr(struct vector* v)
{
	return v->data;
}

void free_vector(struct vector* v)
{
	free(v->data);
	v->size = 0;
}

unsigned int size_vector(struct vector *v)
{
       return v->size;
}


void print_vector(struct vector* v)
{   unsigned int i;
    for (i=0; i<v->size; i++)
        printf("%u\n",v->data[i]);
}

