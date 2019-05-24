#include "graph.h"

static graph_node_t *
new_graph_node(void *data);

static graph_edge_t *
new_node_edge(graph_node_t *node, uint64_t weight);

graph_node_t *
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

graph_node_t *
graph_find_node(graph_t *graph, void *data)
{
	if((NULL == graph) || (NULL == data))
	{
		errno = EINVAL;
		return NULL;
	}
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

graph_edge_t *
graph_find_edge(graph_t *graph, void *from, void *to)
{
	if((NULL == graph) || (NULL == graph->head) || (NULL == from) || (NULL == to))
	{
		errno = EINVAL;
		return NULL;
	}
	graph_node_t *cur_node = graph_find_node(graph, from);
	if(NULL == cur_node)
	{
		return NULL;
	}
	graph_edge_t *cur_edge = cur_node->edges;
	while(NULL != cur_edge)
	{
		if(0 == graph->compare(to, cur_edge->edge->data))
		{
			return cur_edge;
		}
		cur_edge = cur_edge->next_edge;
	}
	return NULL;
}

void *
graph_remove_node(graph_t *graph, void *data)
{
	if((NULL == graph) || (NULL == graph->head) || (NULL == data))
	{
		errno = EINVAL;
		return NULL;
	}
	graph_node_t *remove_node = graph_find_node(graph, data);
	if(NULL == remove_node)
	{
		return NULL;
	}
	graph_edge_t *cur_edge = remove_node->edges;
	if(NULL == cur_edge)
	{
		void *ret_data = remove_node->data;
		free(remove_node);
		remove_node = NULL;
		return ret_data;
	}
	graph_edge_t *remove_edge = NULL;
	while(NULL != cur_edge)
	{
		remove_edge = graph_find_edge(graph, cur_edge->edge->data, data);
		if(NULL == remove_edge)
		{
			return NULL;
		}
		if(!graph_remove_edge(graph, cur_edge->edge->data, data))
		{
			return NULL;
		}
		cur_edge = remove_node->edges;;
	}
	if(NULL == remove_node->prev_node)
	{
		graph->head = remove_node->next_node;
	}
	else
	{
		remove_node->next_node->prev_node = remove_node->prev_node;
		remove_node->prev_node->next_node = remove_node->next_node;
	}
	void *ret_data = remove_node->data;
	free(remove_node);
	remove_node = NULL;
	return ret_data;
}


bool
graph_remove_edge(graph_t *graph, void *from, void *to)
{
	if((NULL == graph) || (NULL == graph->head) || (NULL == from) || (NULL == to))
	{
		errno = EINVAL;
		return false;
	}
	bool removed = false;
	graph_node_t *from_node = graph_find_node(graph, from);	
	graph_node_t *to_node = graph_find_node(graph, to);	
	graph_edge_t *from_edge = from_node->edges;
	graph_edge_t *to_edge = to_node->edges;
	while(NULL != from_edge)
	{
		if(from_edge->edge == to_node)
		{
			if(NULL != from_edge->prev_edge)
			{
				from_edge->prev_edge->next_edge = from_edge->next_edge;
			}
			if(NULL == from_edge->prev_edge)
			{
				from_node->edges = from_edge->next_edge;
				if(NULL != from_node->edges)
				{
					from_node->edges->prev_edge = NULL;
				}
			}
			if(NULL != from_edge->next_edge)
			{
				from_edge->next_edge->prev_edge = from_edge->prev_edge;
			}
			free(from_edge);
			from_edge = NULL;
			removed = true;
			break;
		}
		from_edge = from_edge->next_edge;
	}
	if(false == removed)
	{
		/* No edge found */
		return false;
	}
	removed = false;
	while(NULL != to_edge)
	{
		if(to_edge->edge == from_node)
		{
			if(NULL != to_edge->prev_edge)
			{
				to_edge->prev_edge->next_edge = to_edge->next_edge;
			}
			if(NULL == to_edge->prev_edge)
			{
				to_node->edges = to_edge->next_edge;
				if(NULL != to_node->edges)
				{
					to_node->edges->prev_edge = NULL;
				}
			}
			if(NULL != to_edge->next_edge)
			{
				to_edge->next_edge->prev_edge = to_edge->prev_edge;
			}
			free(to_edge);
			to_edge = NULL;
			removed = true;
			break;
		}
		to_edge = to_edge->next_edge;
	}
	if(false == removed)
	{
		/* No Edge found */
		return false;
	}
	return true;
}

void
graph_destroy(graph_t *graph)
{
	if(NULL == graph)
	{
		errno = EINVAL;
		return;
	}
	graph_node_t *head = graph->head;
	while(NULL != head)
	{
		graph_node_t *tmp = graph->head->next_node;
		printf("Removing %s\n", (char *)head->data);
		if(!graph_remove_node(graph, head->data))
		{
			printf("%s not found\n", (char *)head->data);
		}
		graph->head = tmp;
		head = tmp;
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

