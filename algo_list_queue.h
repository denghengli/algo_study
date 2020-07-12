/*
 * Copyright (c) 20019-2020, wanweiyingchuang
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-11-30     denghengli   the first version
 */

#ifndef __ALGO_LIST_QUEUE_H__
#define __ALGO_LIST_QUEUE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rtthread.h>

#define LIST_QUEUE_MALLOC(size)   rt_malloc(size);
#define LIST_QUEUE_CALLOC(n,size) rt_calloc(n,size);
#define LIST_QUEUE_FREE(p)        rt_free(p);

#define LIST_QUEUE_NUM(pqueue)      (pqueue->num)
#define LIST_QUEUE_IS_EMPTY(pqueue) (pqueue->num == 0)

struct list_queue_node
{
    struct list_queue_node *next; /* next node*/
	int    value_size; /* value size */
    void   *value; /* node value */
};

struct list_queue
{
    struct list_queue_node *head; /* points to the the next dequeue data  */
    struct list_queue_node *tail; /* points to the the last enqueue data  */
    int num; /* queue node num */
};

extern struct list_queue* list_queue_creat(void);
extern int list_queue_init   (struct list_queue *queue);
extern int list_queue_empty  (struct list_queue *queue);
extern int list_queue_destory(struct list_queue *queue);
extern int list_queue_dequeue(struct list_queue *queue, void *out_data);
extern int list_queue_enqueue(struct list_queue *queue, void *in_data, int dsize);

extern void list_queue_test(void);

#endif
