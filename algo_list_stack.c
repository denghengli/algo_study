/*
 * Copyright (c) 20019-2020, wanweiyingchuang
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-11-29     denghengli   the first version
 */

#include "algo_list_stack.h"

/**
 * dynamically create a list stack.
 * 
 * @return NULL:malloc fail
 *        !NULL:success
 */
struct list_stack* list_stack_creat(void)
{
    struct list_stack *stack = NULL;

    stack = LIST_STACK_CALLOC(1,sizeof(*stack));
    if (stack == NULL)
        return NULL;
    
    stack->top = NULL;
    stack->num = 0;

    return stack;
}

/**
 * init a list stack.
 * 
 * @param stack:list stack
 * @return -1:stack is null
 *          0:success
 */
int list_stack_init(struct list_stack *stack)
{
    if (stack == NULL)
        return -1;

    stack->top = NULL;
    stack->num = 0;

    return 0;
}

/**
 * delete list stack space.
 * 
 * @param stack: list stack
 * @return -1: fail
 *          0:success
 */
int list_stack_empty(struct list_stack *stack)
{
    struct list_stack_node *node = NULL;

    if (stack == NULL)
        return -1;

    while (!LIST_STACK_IS_EMPTY(stack))
    {
        node = stack->top;
        stack->top = node->next;
        stack->num--;
		
		LIST_STACK_FREE(node->value);
		node->value = NULL;
        LIST_STACK_FREE(node);
        node = NULL;
    }
    
    return 0;
}

/**
 * delete and destroy a list stack.
 * 
 * @param stack: list stack
 * @return -1: fail
 *          0:success
 */
int list_stack_destory(struct list_stack *stack)
{
    if (list_stack_empty(stack) != 0)
        return -1;

    LIST_STACK_FREE(stack);
    stack = NULL;

    return 0;
}

/**
 * pop data from the array stack.
 * 
 * @param list_stack: array stack
 * @return -1: fail
 *         -2: stack is empty
 *          0: success
 */
int list_stack_pop (struct list_stack *stack, void *out_data)
{
	struct list_stack_node *node = NULL;
	
    if (stack == NULL)
        return -1;

    if (LIST_STACK_IS_EMPTY(stack))
        return -2;

	node = stack->top;
	stack->top = node->next;
	stack->num--;
	
    memcpy(out_data, node->value, node->value_size);
	LIST_STACK_FREE(node->value);
	LIST_STACK_FREE(node);
	node->value = NULL;
	node = NULL;
	
    return 0;
}

/**
 * push data to the list stack forward stack top.
 * 
 * @param stack: list stack
 * @param in_data: push data
 * @return -1: fail
 *          0: success
 */
int list_stack_push(struct list_stack *stack, void *in_data, int dsize)
{
    struct list_stack_node *node = NULL;

    if (stack == NULL)
        return -1;

	/* malloc node space */
    node = LIST_STACK_MALLOC(sizeof(*node));
    if (node == NULL)
        return -1;

	/* malloc value space */
	node->value = LIST_STACK_MALLOC(dsize);
	if (node->value == NULL)
		return -1;
	
	memcpy(node->value, in_data, dsize);
	node->value_size = dsize;
    node->next = stack->top;

    stack->top = node;
    stack->num++;

    return 0;
}

struct list_stack *list_stack_head;
int list_stack_w[5] = {11,22,33,44,55};
int list_stack_r[5] = {0};
void list_stack_test(void)
{
	list_stack_head = list_stack_creat();
	
	list_stack_push(list_stack_head, (void*)&list_stack_w[0], 4);
	list_stack_push(list_stack_head, (void*)&list_stack_w[1], 4);
	list_stack_push(list_stack_head, (void*)&list_stack_w[2], 4);
	list_stack_push(list_stack_head, (void*)&list_stack_w[3], 4);
	list_stack_push(list_stack_head, (void*)&list_stack_w[4], 4);
	
	list_stack_pop(list_stack_head, (void*)&list_stack_r[0]);
	list_stack_pop(list_stack_head, (void*)&list_stack_r[1]);
	list_stack_pop(list_stack_head, (void*)&list_stack_r[2]);
	list_stack_pop(list_stack_head, (void*)&list_stack_r[3]);
	list_stack_pop(list_stack_head, (void*)&list_stack_r[4]);
	
	list_stack_push(list_stack_head, (void*)&list_stack_w[0], 4);
	list_stack_pop(list_stack_head, (void*)&list_stack_r[0]);
	list_stack_push(list_stack_head, (void*)&list_stack_w[1], 4);
	list_stack_pop(list_stack_head, (void*)&list_stack_r[1]);
	list_stack_push(list_stack_head, (void*)&list_stack_w[2], 4);
	list_stack_pop(list_stack_head, (void*)&list_stack_r[2]);
	list_stack_push(list_stack_head, (void*)&list_stack_w[3], 4);
	list_stack_pop(list_stack_head, (void*)&list_stack_r[3]);
	list_stack_push(list_stack_head, (void*)&list_stack_w[4], 4);
	list_stack_pop(list_stack_head, (void*)&list_stack_r[4]);
}

