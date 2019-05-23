#include "graph.h"

static graph_node_t *
new_graph_node(void *data);

bool
graph_init(graph_t *graph, print_f print, compare_f compare)
{
	if((NULL == graph) || (NULL == print) || (NULL == compare))
	{
		return false;
	}
	graph->head = NULL;
	graph->print = print;
	graph->compare = compare;
	return true;
}

bool
graph_add_node(graph_t *graph, void *data)
{
	if((NULL == graph) || (NULL == data))
	{
		errno = EINVAL;
		return false;
	}
	if(NULL == graph->head)
	{
		graph->head = new_graph_node(data);
		if(NULL == graph->head)
		{
			/* Malloc failed */
			return false;
		}
		return true;
	}
	graph_node_t *index = graph->head;
	while(NULL != index->next_node)
	{
		index = index->next_node;
	}
	graph_node_t *new_node = new_graph_node(data);
	if(NULL == new_node)
	{
		return false;
	}
	index->next_node = new_node;
	index->next_node->prev_node = index;
	return true;
}

void
graph_print(graph_t *graph)
{
	if(NULL == graph)
	{
		errno = EINVAL;
		return;
	}
	if(NULL == graph->head)
	{
		printf("Empty Graph\n");
		return;
	}
	graph_node_t *cur_node = graph->head;
	while(NULL != cur_node)
	{
		graph->print(cur_node->data);
		printf(": ");
		graph_edge_t *cur_edge = cur_node->edges;
		if(NULL == cur_edge)
		{
			printf("\tNo Edges\n");
			cur_node = cur_node->next_node;
			continue;
		}
		while(NULL != cur_edge)
		{
			graph->print(cur_edge->edge->data);
			cur_edge = cur_edge->next_edge;
		}
		cur_node = cur_node->next_node;
	}
}

static graph_node_t *
new_graph_node(void *data)
{
	graph_node_t *new_node = malloc(sizeof(*new_node));
	if(NULL == new_node)
	{
		return NULL;
	}	
	new_node->data = data;
	new_node->edges = NULL;
	new_node->next_node = NULL;
	new_node->prev_node = NULL;
	return new_node;
}
