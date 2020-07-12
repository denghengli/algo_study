/*
 * Copyright (c) 20019-2020, wanweiyingchuang
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-11-30     denghengli   the first version
 */

#include "algo_array_queue.h"

/**
 * dynamically creat and init an array queue.
 * 
 * @param size: array size
 * @param tpsz: data type size
 * @return result: array queue or NULL
 */
struct array_queue* array_queue_creat(int size, int tpsz)
{
    struct array_queue *queue = NULL;

    if (size <= 0 || tpsz <= 0)
        return NULL;

	/* malloc an array queue struct */
    queue = ARRAY_QUEUE_MALLOC(sizeof(*queue));
    if (queue == NULL)
        return NULL;
	
	/* calloc an array queue */
	void *p = ARRAY_QUEUE_CALLOC(1, (size * tpsz));
	if (p == NULL)
		return NULL;

    queue->size = size;
    queue->num = 0;
    queue->head = 0;
    queue->tail = 0;
    queue->tpsz = tpsz;
    queue->p = p;
    
    return queue;
}

/**
 * init a static array queue.
 * 
 * @param queue: 
 * @param size: array size
 * @param tpsz: array data type length
 * @return -1:queue is null or malloc fail
 *          0:success
 */
int array_queue_init(struct array_queue *queue, int size, int tpsz)
{
	if (queue == NULL)
		return -1;
	
	/* calloc an array queue */
	void *p = ARRAY_QUEUE_CALLOC(1, (size * tpsz));
	if (p == NULL)
		return NULL;

    queue->size = size;
    queue->num = 0;
    queue->head = 0;
    queue->tail = 0;
    queue->tpsz = tpsz;
    queue->p = p;
	
	return 0;
}

/**
 * delete array queue space.
 * 
 * @param array_queue: array queue
 * @return -1: fail
 *          0:success
 */
int array_queue_empty(struct array_queue *queue)
{
	if (queue == NULL)
		return -1;

    /* free array queue space */
    if (queue->p != NULL)
    {
        ARRAY_QUEUE_FREE(queue->p);
        queue->size = 0;
        queue->num = 0;
        queue->head = 0;
        queue->tail = 0;
        queue->tpsz = 0;
        queue->p = NULL;
    }
	
	return 0;
}

/**
 * delete and destroy an array queue.
 * 
 * @param array_queue: array queue
 * @return -1: fail
 *          0:success
 */
int array_queue_destory(struct array_queue *queue)
{
    if (array_queue_empty(queue) != 0)
        return -1;

    ARRAY_QUEUE_FREE(queue);
	queue = NULL;
		
    return 0;
}

/**
 * enqueue data to the array queue.
 * 
 * @param queue: array queue
 * @return -1: fail
 *         -2: queue is full
 *          0: success
 */
int array_queue_enqueue(struct array_queue *queue, void *in_data)
{
    char *ptail = NULL;

    if (queue == NULL)
        return -1;

	/* queue is full */
    if(ARRAY_QUEUE_IS_FULL(queue))
        return -2;

	ptail = queue->p;
    ptail = ptail + queue->tpsz * queue->tail;
    memcpy(ptail, in_data, queue->tpsz);
    queue->num++;
    queue->tail = (queue->tail + 1) % queue->size;

    return 0;
}

/**
 * dequeue data from the array queue.
 * 
 * @param array_queue: array queue
 * @return -1: fail
 *         -2: queue is empty
 *          0: success
 */
int array_queue_dequeue(struct array_queue *queue, void *out_data)
{
	char *phead = NULL;

    if (queue == NULL)
        return -1;

	/* queue is empty */
	if(ARRAY_QUEUE_IS_EMPTY(queue))
        return -2;

	phead = queue->p;
    phead = phead + queue->tpsz * queue->head;
    memcpy(out_data, phead, queue->tpsz);
    queue->num--;
    queue->head = (queue->head + 1) % queue->size;

    return 0;
}

struct array_queue *array_queue_head;
int array_queue_w[6] = {11,22,33,44,55,66};
int array_queue_r[6] = {0};
void array_queue_test(void)
{
	array_queue_head = array_queue_creat(5, sizeof(int));
	
	array_queue_enqueue(array_queue_head, (void*)&array_queue_w[0]);
	array_queue_enqueue(array_queue_head, (void*)&array_queue_w[1]);
	array_queue_enqueue(array_queue_head, (void*)&array_queue_w[2]);
	array_queue_enqueue(array_queue_head, (void*)&array_queue_w[3]);
	array_queue_enqueue(array_queue_head, (void*)&array_queue_w[4]);
	array_queue_enqueue(array_queue_head, (void*)&array_queue_w[5]);
	
	array_queue_dequeue(array_queue_head, (void*)&array_queue_r[0]);
	array_queue_dequeue(array_queue_head, (void*)&array_queue_r[1]);
	array_queue_dequeue(array_queue_head, (void*)&array_queue_r[2]);
	array_queue_dequeue(array_queue_head, (void*)&array_queue_r[3]);
	array_queue_dequeue(array_queue_head, (void*)&array_queue_r[4]);
	array_queue_dequeue(array_queue_head, (void*)&array_queue_r[5]);
}

