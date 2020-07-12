/*
 * Copyright (c) 20019-2020, wanweiyingchuang
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-11-29     denghengli   the first version
 */

#ifndef __ALGO_LIST_STACK_H__
#define __ALGO_LIST_STACK_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rtthread.h>

#define LIST_STACK_MALLOC(size)   rt_malloc(size);
#define LIST_STACK_CALLOC(n,size) rt_calloc(n,size);
#define LIST_STACK_FREE(p)        rt_free(p);

#define LIST_STACK_NUM(pstack)      (pstack->num)
#define LIST_STACK_IS_EMPTY(pstack) (pstack->top == NULL)

struct list_stack_node
{
    struct list_stack_node *next; /* next node*/
	int    value_size; /* value size */
    void   *value; /* node value */
};

struct list_stack
{
    struct list_stack_node *top; /* stack top */
    int num; /* stack node num */
};

extern struct list_stack* list_stack_creat(void);
extern int list_stack_init   (struct list_stack *stack);
extern int list_stack_empty  (struct list_stack *stack);
extern int list_stack_destory(struct list_stack *stack);
extern int list_stack_pop (struct list_stack *stack, void *out_data);
extern int list_stack_push(struct list_stack *stack, void *in_data, int dsize);

extern void list_stack_test(void);

#endif
