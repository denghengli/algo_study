/*
 * Copyright (c) 20019-2020, wanweiyingchuang
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-11-27     denghengli   the first version
 */

#include "algo_array_stack.h"

/**
 * dynamically creat and init an array stack.
 * 
 * @param size: array size
 * @param tpsz: data type size
 * @return result: array stack or NULL
 */
struct array_stack* array_stack_creat(int size, int tpsz)
{
    struct array_stack *stack = NULL;

    if (size <= 0 || tpsz <= 0)
        return NULL;

	/* malloc an array stack struct */
    stack = ARRAY_STACK_MALLOC(sizeof(*stack));
    if (stack == NULL)
        return NULL;
	
	/* calloc an array stack */
	void *p = ARRAY_STACK_CALLOC(1, (size * tpsz));
	if (p == NULL)
		return NULL;

    stack->size = size;
    stack->tpsz = tpsz;
    stack->pos = -1;
    stack->p = p;
    
    return stack;
}

/**
 * init a static array stack.
 * 
 * @param stack: 
 * @param size: array size
 * @param tpsz: array data type length
 * @return -1:stack is null or malloc fail
 *          0:success
 */
int array_stack_init(struct array_stack *stack, int size, int tpsz)
{
	if (stack == NULL)
		return -1;
	
	/* calloc an array stack */
	void *p = ARRAY_STACK_CALLOC(1, (size * tpsz));
	if (p == NULL)
		return NULL;

    stack->size = size;
    stack->tpsz = tpsz;
    stack->pos = -1;
    stack->p = p;
	
	return 0;
}

/**
 * delete array stack space.
 * 
 * @param array_stack: array stack
 * @return -1: fail
 *          0:success
 */
int array_stack_empty(struct array_stack *stack)
{
	if (stack == NULL)
		return -1;

    /* free array stack space */
    if (stack->p != NULL)
    {
        ARRAY_STACK_FREE(stack->p);
        stack->size = 0;
        stack->tpsz = 0;
        stack->pos = -1;
        stack->p = NULL;
    }
	
	return 0;
}

/**
 * delete and destroy an array stack.
 * 
 * @param array_stack: array stack
 * @return -1: fail
 *          0:success
 */
int array_stack_destory(struct array_stack *stack)
{
    if (array_stack_empty(stack) != 0)
        return -1;

    ARRAY_STACK_FREE(stack);
	stack = NULL;
		
    return 0;
}

/**
 * pop data from the array stack.
 * 
 * @param array_stack: array stack
 * @return -1: fail
 *         -2: stack is empty
 *          0: success
 */
int array_stack_pop(struct array_stack *stack, void *out_data)
{
    char *pbegin = NULL;
	void *data = NULL;

    if (stack == NULL)
        return -1;

	/* stack is empty */
    if(ARRAY_STACK_IS_EMPTY(stack))
        return -2;

	pbegin = stack->p;
    data = pbegin + stack->tpsz * stack->pos;
    memcpy(out_data, data, stack->tpsz);
    stack->pos--;

    return 0;
}

/**
 * push data to the array stack.
 * 
 * @param array_stack: array stack
 * @return -1: fail
 *         -2: stack is full
 *          0: success
 */
int array_stack_push(struct array_stack *stack, void *in_data)
{
	char *pbegin = NULL;
    void *data = NULL;

    if (stack == NULL)
        return -1;

	/* stack is full */
	if(ARRAY_STACK_IS_FULL(stack))
        return -2;

	pbegin = stack->p;
    stack->pos++;
    data = pbegin + stack->tpsz * stack->pos;
    memcpy(data, in_data, stack->tpsz);

    return 0;
}


/**
 * Support dynamic expansion of sequential stack, 
 * when the stack is full to reapply for 2 times the size of the stack space.
 * 
 * @param array_stack: array stack
 * @return -1: fail
 *         -2: stack is full
 *          0: success
 */
int array_stack_push_dyn(struct array_stack *stack, void *in_data)
{
    void *new_p = NULL;

    if (stack == NULL)
        return -1;

    /* stack is full */
	if(ARRAY_STACK_IS_FULL(stack))
    {
        new_p = ARRAY_STACK_CALLOC(1, (2 * stack->size * stack->tpsz));
        if (new_p == NULL)
            return -1;

        memcpy(new_p, stack->p, stack->size * stack->tpsz);
        ARRAY_STACK_FREE(stack->p);
        stack->p = new_p;
        stack->size *= 2;
        return array_stack_push(stack, in_data);
    }
    else
    {
        return array_stack_push(stack, in_data);
    }
}



struct array_stack *array_stack_head;
int array_stack_w[5] = {11,22,33,44,55};
int array_stack_r[5] = {0};
void array_stack_test(void)
{
	array_stack_head = array_stack_creat(5, sizeof(int));
	
	array_stack_push(array_stack_head, (void*)&array_stack_w[0]);
	array_stack_push(array_stack_head, (void*)&array_stack_w[1]);
	array_stack_push(array_stack_head, (void*)&array_stack_w[2]);
	array_stack_push(array_stack_head, (void*)&array_stack_w[3]);
	array_stack_push(array_stack_head, (void*)&array_stack_w[4]);
	
	array_stack_pop(array_stack_head, (void*)&array_stack_r[0]);
	array_stack_pop(array_stack_head, (void*)&array_stack_r[1]);
	array_stack_pop(array_stack_head, (void*)&array_stack_r[2]);
	array_stack_pop(array_stack_head, (void*)&array_stack_r[3]);
	array_stack_pop(array_stack_head, (void*)&array_stack_r[4]);
	
	array_stack_push(array_stack_head, (void*)&array_stack_w[0]);
	array_stack_pop(array_stack_head, (void*)&array_stack_r[0]);
	array_stack_push(array_stack_head, (void*)&array_stack_w[1]);
	array_stack_pop(array_stack_head, (void*)&array_stack_r[1]);
	array_stack_push(array_stack_head, (void*)&array_stack_w[2]);
	array_stack_pop(array_stack_head, (void*)&array_stack_r[2]);
	array_stack_push(array_stack_head, (void*)&array_stack_w[3]);
	array_stack_pop(array_stack_head, (void*)&array_stack_r[3]);
	array_stack_push(array_stack_head, (void*)&array_stack_w[4]);
	array_stack_pop(array_stack_head, (void*)&array_stack_r[4]);
}












