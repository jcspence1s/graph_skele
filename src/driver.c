#include "graph.h"
#include <stdio.h>
#include <string.h>

void
myPrint(void *data);

int 
main(void)
{
	graph_t myGraph;

	printf("Graph has %s been initialized\n", graph_init(&myGraph, myPrint, strcmp) ? "\b" : "not");
	char *a = "A";
	char *b = "B";
	char *c = "C";
	char *d = "D";

	graph_add_node(&myGraph, a);
	graph_add_node(&myGraph, b);
	graph_add_node(&myGraph, c);
	graph_add_node(&myGraph, d);

	graph_add_edge(&myGraph, a, b, 1);
	graph_add_edge(&myGraph, a, c, 3);

	graph_print(&myGraph);

	return 0;
}

void
myPrint(void *data)
{
	printf("%s", (char *)data);
}
