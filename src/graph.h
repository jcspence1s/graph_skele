#ifndef graph_h
#define graph_h

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <stdio.h>

typedef int(*compare_f)(const char *, const char *);
typedef void(*print_f)(void *);

typedef struct graph_edge_t
{
	uint64_t weight;
	struct graph_edge_t *next_edge;
	struct graph_edge_t *prev_edge;
	struct graph_node_t *edge;
}graph_edge_t;

typedef struct graph_node_t
{
	void *data;
	graph_edge_t *edges;
	struct graph_node_t *next_node;
	struct graph_node_t *prev_node;
}graph_node_t;

typedef struct graph_t
{
	graph_node_t *head;		
	print_f print;
	compare_f compare;
}graph_t;

bool
graph_init(graph_t *graph, print_f print, compare_f compare);

bool
graph_add_node(graph_t *graph, void *data);

bool
graph_add_edge(graph_t *graph, void *source, void *dest, uint64_t weight);

void
graph_print(graph_t *graph);

graph_node_t *
graph_find_node(graph_t *graph, void *data);

graph_edge_t *
graph_find_edge(graph_t *graph, void *from, void *to);

void *
graph_remove_node(graph_t *graph, void *data);

bool
graph_remove_edge(graph_t *graph, void *from, void *to);

#endif
