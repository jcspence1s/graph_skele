#include "graph.h"
#include "pqueue.h"
#include <stdio.h>
#include <string.h>

void
myPrint(void *data);

int 
main(void)
{
	graph_t myGraph;
	pqueue_t myPqueue;

	printf("Graph has %s been initialized\n", graph_init(&myGraph, myPrint, strcmp) ? "\b" : "not");
	char *a = "A";
	char *b = "B";
	char *c = "C";
	char *d = "D";
	char *e = "E";
	char *f = "F";

	graph_add_node(&myGraph, a);
	graph_add_node(&myGraph, b);
	graph_add_node(&myGraph, c);
	graph_add_node(&myGraph, d);
	graph_add_node(&myGraph, e);
	graph_add_node(&myGraph, f);

	graph_add_edge(&myGraph, a, b, 2);
	graph_add_edge(&myGraph, a, d, 5);
	graph_add_edge(&myGraph, a, c, 1);
	graph_add_edge(&myGraph, b, f, 1);
	graph_add_edge(&myGraph, b, d, 2);
	graph_add_edge(&myGraph, c, d, 1);
	graph_add_edge(&myGraph, c, e, 4);
	//graph_add_edge(&myGraph, a, c, 3);

	graph_print(&myGraph);

	printf("A -> B is %s found.\n", graph_find_edge(&myGraph, a, b) ? "\b" : "not");
	printf("B -> A is %s found.\n", graph_find_edge(&myGraph, b, a) ? "\b" : "not");
	printf("B -> D is %s found.\n", graph_find_edge(&myGraph, b, d) ? "\b" : "not");
	
	/*
	printf("%s is removed.\n", (char *)graph_remove_node(&myGraph, a));
	graph_print(&myGraph);
	graph_destroy(&myGraph);
	graph_print(&myGraph);
	printf("Pqueue has %s been initialized\n", queue_init(&myPqueue, myPrint, strcmp) ? "\b" : "not");

	queue_enqueue(&myPqueue, a, 2);
	queue_enqueue(&myPqueue, b, 4);
	queue_enqueue(&myPqueue, c, 3);
	queue_enqueue(&myPqueue, d, 1);

	queue_update(&myPqueue, c, 0);
	
	queue_print(&myPqueue);

	printf("Dequeued %s \n", (char *)queue_dequeue(&myPqueue));
	printf("Dequeued %s \n", (char *)queue_dequeue(&myPqueue));
	printf("Dequeued %s \n", (char *)queue_dequeue(&myPqueue));
	printf("Dequeued %s \n", (char *)queue_dequeue(&myPqueue));
	printf("Dequeued %s \n", (char *)queue_dequeue(&myPqueue));
	*/

	pqueue_t path;
	queue_init(&path, myPrint, myGraph.compare);
	if(find_path(&myGraph, f, e, &path))
	{
		graph_node_t *popped = queue_dequeue(&path);
		while(NULL != popped)
		{
			myPrint(popped->data);
			printf("->");
			popped = (graph_node_t *)queue_dequeue(&path);
		}
	}
	else
	{
		printf("No path\n");
	}

	return 0;
}

void
myPrint(void *data)
{
	printf("%s", (char *)data);
}
