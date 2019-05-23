#include "graph.h"

static graph_node_t *
new_graph_node(void *data);

static graph_edge_t *
new_node_edge(graph_node_t *node, uint64_t weight);

static graph_node_t *
graph_find_node(graph_t *graph, void *data);

static bool
add_edge_to_node(graph_node_t *from_node, graph_node_t *to_node, uint64_t weight);

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

bool
graph_add_edge(graph_t *graph, void *source, void *dest, uint64_t weight)
{
	if((NULL == graph) || (NULL == graph->head) || (NULL == source) || (NULL == dest))
	{
		errno = EINVAL;
		return false;
	}
	graph_node_t *from_node = graph_find_node(graph, source);
	graph_node_t *to_node = graph_find_node(graph, dest);
	if((NULL == from_node) || (NULL == to_node))
	{
		errno = EINVAL;
		return false;
	}
	if(add_edge_to_node(from_node, to_node, weight));
	{
		if(add_edge_to_node(to_node, from_node, weight))
		{
			return true;
		}
		return false;
	}
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
		printf(": \t");
		graph_edge_t *cur_edge = cur_node->edges;
		if(NULL == cur_edge)
		{
			printf("No Edges\n");
			cur_node = cur_node->next_node;
			continue;
		}
		while(NULL != cur_edge)
		{
			graph->print(cur_edge->edge->data);
			printf(" %ld -> ", cur_edge->weight);
			cur_edge = cur_edge->next_edge;
		}
		cur_node = cur_node->next_node;
		puts("");
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

static graph_edge_t *
new_node_edge(graph_node_t *node, uint64_t weight)
{
	graph_edge_t *new_edge = malloc(sizeof(*new_edge));
	if(NULL == new_edge)
	{
		return NULL;
	}
	new_edge->weight = weight;
	new_edge->next_edge = NULL;
	new_edge->prev_edge = NULL;
	new_edge->edge = node;
	return new_edge;
}

static bool
add_edge_to_node(graph_node_t *from_node, graph_node_t *to_node, uint64_t weight)
{
	graph_edge_t *new_edge = new_node_edge(to_node, weight);
	if(NULL == new_edge)
	{
		return false;
	}
	if(NULL == from_node->edges)
	{
		from_node->edges = new_edge;
		return true;
	}
	graph_edge_t *index = from_node->edges;
	while(NULL != index->next_edge)
	{
		if(to_node == index->edge)
		{
			return true;
		}
		index = index->next_edge;
	}
	index->next_edge = new_edge;
	new_edge->prev_edge = index;
	return true;
}

static graph_node_t *
graph_find_node(graph_t *graph, void *data)
{
	graph_node_t *index = graph->head;
	while(NULL != index)
	{
		if(0 == graph->compare(index->data, data))
		{
			return index;
		}
		index = index->next_node;
	}
	return NULL;
}
