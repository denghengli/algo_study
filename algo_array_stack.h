/*
 * Copyright (c) 20019-2020, wanweiyingchuang
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-11-27     denghengli   the first version
 */

#ifndef __ALGO_ARRAY_STACK_H__
#define __ALGO_ARRAY_STACK_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_STACK_MALLOC(size)   malloc(size);
#define ARRAY_STACK_CALLOC(n,size) calloc(n,size);
#define ARRAY_STACK_FREE(p)        free(p);

#define ARRAY_STACK_SIZE(pstack)     (pstack->size)
#define ARRAY_STACK_IS_EMPTY(pstack) (pstack->pos == -1)
#define ARRAY_STACK_IS_FULL(pstack)  (pstack->pos == (pstack->size - 1))

struct array_stack
{
    int size; /* stack total size */
    int pos;  /* stack cur position rang:-1 -- (size-1)*/
    int tpsz; /* data type size */
    void *p;  /* stack space */
};

extern struct array_stack* array_stack_creat(int size, int tpsz);
extern int array_stack_init   (struct array_stack *stack, int size, int tpsz);
extern int array_stack_empty  (struct array_stack *stack);
extern int array_stack_destory(struct array_stack *stack);
extern int array_stack_pop (struct array_stack *stack, void *out_data);
extern int array_stack_push(struct array_stack *stack, void *in_data);
extern int array_stack_push_dyn(struct array_stack *stack, void *in_data);

extern void array_stack_test(void);

#endif
