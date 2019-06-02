#ifndef pqueue_h
#define pqueue_h

#include <stdbool.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef int(*compare_f)(const char *, const char*);
typedef void(*print_f)(void *);

typedef struct pqueue_node_t
{
	void *data;
	struct pqueue_node_t *next;
	struct pqueue_node_t *prev;
	uint64_t priority;
}pqueue_node_t;

typedef struct pqueue_t
{
	pqueue_node_t *head;	
	pqueue_node_t *tail;
	compare_f compare;
	print_f print;
}pqueue_t;

bool
queue_init(pqueue_t *pqueue, print_f print, compare_f compare);

bool
queue_enqueue(pqueue_t *pqueue, void *data, uint64_t priority);

void *
queue_dequeue(pqueue_t *pqueue);

void
queue_print(pqueue_t *pqueue);

bool
queue_update(pqueue_t *pqueue, uint64_t data, uint64_t new_priority);

bool
queue_sort(pqueue_t *pqueue);

void *
queue_peek(pqueue_t *pqueue, void *data);

bool
queue_is_present(pqueue_t *pqueue, uint64_t data);

#endif
