#include "pqueue.h"

static pqueue_node_t *
new_pqueue_node(void *data, uint64_t priority);

bool
queue_init(pqueue_t *pqueue, print_f print, compare_f compare)
{
	if((NULL == pqueue) || (NULL == compare) || (NULL == print))
	{
		errno = EINVAL;
		return false;
	}
	pqueue->head = NULL;
	pqueue->tail = NULL;
	pqueue->compare = compare;
	pqueue->print = print;
	return true;
}

bool
queue_enqueue(pqueue_t *pqueue, void *data, uint64_t priority)
{
	if((NULL == pqueue) || (NULL == data))
	{
		errno = EINVAL;
		return false;
	}
	pqueue_node_t *new_node = new_pqueue_node(data, priority);
	if(NULL == pqueue->head)
	{
		pqueue->head = new_node;
		pqueue->tail = new_node;
		return true;
	}
	pqueue_node_t *tmp = pqueue->head;
	if(priority < pqueue->head->priority)
	{
		tmp->prev = new_node;
		new_node->next = tmp;
		pqueue->head = new_node;
		return true;
	}
	while(NULL != tmp)
	{
		if((priority > tmp->priority) && (NULL == tmp->next))
		{
			tmp->next = new_node;
			new_node->prev = tmp;
			return true;
		}
		if(priority > tmp->priority && priority < tmp->next->priority) 
		{
			new_node->next = tmp->next;
			new_node->prev = tmp;
			tmp->next = new_node;
			new_node->next->prev = new_node;
			return true;
		}
		tmp = tmp->next;
	}
	pqueue->tail->next = new_node;
	new_node->prev = pqueue->tail;
	pqueue->tail = new_node;
	return true;
}

void *
queue_peek(pqueue_t *pqueue, void *data)
{
	if((NULL == pqueue) || (NULL == pqueue->head) || (NULL == data))
	{
		errno = EINVAL;
		return NULL;
	}
	pqueue_node_t *cur = pqueue->head;
	while(NULL != cur)
	{
		if(cur == data)
		{
			return cur;
		}
		cur = cur->next;
	}
	return NULL;
}

void *
queue_dequeue(pqueue_t *pqueue)
{
	if((NULL == pqueue) || (NULL == pqueue->head))
	{
		errno = EINVAL;
		return NULL;
	}
	else
	{
		void *data = pqueue->head->data;
		pqueue_node_t *tmp = pqueue->head;
		pqueue->head = pqueue->head->next;
		free(tmp);
		tmp = NULL;
		return data;
	}
}

bool
queue_update(pqueue_t *pqueue, void *data, uint64_t new_priority)
{
	if((NULL == pqueue) || (NULL == pqueue->head) || (NULL == data))
	{
		return false;
	}
	pqueue_node_t *cur = pqueue->head;
	while(NULL != cur)
	{
		if(cur->data == data)
		{
			if(cur->priority > new_priority)
			{
				cur->priority = new_priority;
				if(queue_sort(pqueue))
				{
					return true;
				}
			}
			/* Old value is better */
			return false;
		}
		cur = cur->next;
	}
	return false;

}

bool
queue_sort(pqueue_t *pqueue)
{
	if((NULL == pqueue) || (NULL == pqueue->head))
	{
		errno = EINVAL;
		return false;
	}
	bool changed = false;
	do
	{
		changed = false;
		pqueue_node_t *cur = pqueue->head;
		while(NULL != cur)
		{
			if((NULL != cur->next) && (cur->priority > cur->next->priority))
			{
				pqueue_node_t *tmp = cur->next;
				if(cur == pqueue->head)
				{
					pqueue->head = tmp;
				}
				if(NULL == tmp->next)
				{
					pqueue->tail = cur;
				}
				else
				{
					tmp->next->prev = cur;
				}
				if(NULL != cur->prev)
				{
					cur->prev->next = tmp;
				}
				cur->next = tmp->next;
				tmp->prev = cur->prev;
				tmp->next = cur;
				cur->prev = tmp;
				changed = true;
			}
			cur = cur->next;
		}
	}while(false != changed);
	return true;
}

void
queue_print(pqueue_t *pqueue)
{
	if(NULL == pqueue)
	{
		errno = EINVAL;
		return;
	}
	if(NULL == pqueue->head)
	{
		printf("Empty pqueue.\n");
	}
	pqueue_node_t *index = pqueue->head;
	while(NULL != index)
	{
		pqueue->print(index->data);
		printf(": %ld\n", index->priority);
		index = index->next;
	}
}

bool
queue_is_present(pqueue_t *pqueue, void *data)
{
	if((NULL == pqueue) || (NULL == pqueue->head) || (NULL == data))
	{
		errno = EINVAL;
		return false;
	}
	pqueue_node_t *cur = pqueue->head;
	while(NULL != cur)
	{
		if(cur->data == data)
		{
			return true;
		}
		cur = cur->next;
	}
	return false;
}

static pqueue_node_t *
new_pqueue_node(void *data, uint64_t priority)
{
	if(NULL == data)
	{
		return NULL;
	}
	pqueue_node_t *new_node = malloc(sizeof(*new_node));
	new_node->data = data;
	new_node->next = NULL;
	new_node->prev = NULL;
	new_node->priority = priority;
	return new_node;
}
