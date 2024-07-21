/*
 * Copyright (c) 20019-2020, wanweiyingchuang
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-11-30     denghengli   the first version
 */

#include "algo_list_queue.h"

/**
 * dynamically create a list queue.
 * 
 * @return NULL:malloc fail
 *        !NULL:success
 */
struct list_queue* list_queue_creat(void)
{
    struct list_queue *queue = NULL;

    queue = LIST_QUEUE_CALLOC(1,sizeof(*queue));
    if (queue == NULL)
        return NULL;
    
    queue->head = NULL;
    queue->tail = NULL;
    queue->num = 0;

    return queue;
}

/**
 * init a list queue.
 * 
 * @param queue:list queue
 * @return -1:queue is null
 *          0:success
 */
int list_queue_init(struct list_queue *queue)
{
    if (queue == NULL)
        return -1;

    queue->head = NULL;
    queue->tail = NULL;
    queue->num = 0;

    return 0;
}

/**
 * delete list queue space.
 * 
 * @param queue: list queue
 * @return -1: fail
 *          0:success
 */
int list_queue_empty(struct list_queue *queue)
{
    struct list_queue_node *del_node = NULL;

    if (queue == NULL)
        return -1;

    while (!LIST_QUEUE_IS_EMPTY(queue))
    {
        del_node = queue->head;
        queue->head = del_node->next;
        queue->num--;
		
		LIST_QUEUE_FREE(del_node->value);
		del_node->value = NULL;
        LIST_QUEUE_FREE(del_node);
        del_node = NULL;
    }
    
    return 0;
}

/**
 * delete and destroy a list queue.
 * 
 * @param queue: list queue
 * @return -1: fail
 *          0:success
 */
int list_queue_destory(struct list_queue *queue)
{
    if (list_queue_empty(queue) != 0)
        return -1;

    LIST_QUEUE_FREE(queue);
    queue = NULL;

    return 0;
}

/**
 * dequeue data from the list queue.
 * 
 * @param queue: list queue
 * @param out_data: dequeue data
 * @return -1: fail
 *         -2: queue is empty
 *          0: success
 */
int list_queue_dequeue(struct list_queue *queue, void *out_data)
{
	struct list_queue_node *del_node = NULL;
	
    if (queue == NULL)
        return -1;

    if (LIST_QUEUE_IS_EMPTY(queue))
        return -2;

	del_node = queue->head;
	queue->head = del_node->next;
	queue->num--;
	
    memcpy(out_data, del_node->value, del_node->value_size);
	LIST_QUEUE_FREE(del_node->value);
	LIST_QUEUE_FREE(del_node);
	del_node->value = NULL;
	del_node = NULL;
	
    return 0;
}

/**
 * enqueue data to the list queue backwards.
 * 
 * @param queue: list queue
 * @param in_data: enqueue data
 * @return -1: fail
 *          0: success
 */
int list_queue_enqueue(struct list_queue *queue, void *in_data, int dsize)
{
    struct list_queue_node *add_node = NULL;

    if (queue == NULL)
        return -1;

	/* malloc add_node space */
    add_node = LIST_QUEUE_MALLOC(sizeof(*add_node));
    if (add_node == NULL)
        return -1;

	/* malloc value space */
	add_node->value = LIST_QUEUE_MALLOC(dsize);
	if (add_node->value == NULL)
		return -1;
	
	memcpy(add_node->value, in_data, dsize);
	add_node->value_size = dsize;
    add_node->next = NULL;

    if (queue->head == NULL )
	{
		queue->head = add_node;
		queue->tail = add_node;
	}
	else
	{
		queue->tail->next = add_node;
		queue->tail = add_node;
	}
    queue->num++;

    return 0;
}

struct list_queue *list_queue_head;
int list_queue_w[5] = {11,22,33,44,55};
int list_queue_r[5] = {0};
void list_queue_test(void)
{
	list_queue_head = list_queue_creat();
	
	list_queue_enqueue(list_queue_head, (void*)&list_queue_w[0], 4);
	list_queue_enqueue(list_queue_head, (void*)&list_queue_w[1], 4);
	list_queue_enqueue(list_queue_head, (void*)&list_queue_w[2], 4);
	list_queue_enqueue(list_queue_head, (void*)&list_queue_w[3], 4);
	list_queue_enqueue(list_queue_head, (void*)&list_queue_w[4], 4);
	
	list_queue_dequeue(list_queue_head, (void*)&list_queue_r[0]);
	list_queue_dequeue(list_queue_head, (void*)&list_queue_r[1]);
	list_queue_dequeue(list_queue_head, (void*)&list_queue_r[2]);
	list_queue_dequeue(list_queue_head, (void*)&list_queue_r[3]);
	list_queue_dequeue(list_queue_head, (void*)&list_queue_r[4]);
	
	list_queue_enqueue(list_queue_head, (void*)&list_queue_w[2], 4);
	list_queue_enqueue(list_queue_head, (void*)&list_queue_w[2], 4);

	list_queue_dequeue(list_queue_head, (void*)&list_queue_r[0]);
	list_queue_dequeue(list_queue_head, (void*)&list_queue_r[1]);
	list_queue_dequeue(list_queue_head, (void*)&list_queue_r[2]);
	list_queue_dequeue(list_queue_head, (void*)&list_queue_r[3]);
	list_queue_dequeue(list_queue_head, (void*)&list_queue_r[4]);
}

