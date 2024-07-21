/*
 * Copyright (c) 20019-2020, wanweiyingchuang
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-12-1    denghengli   the first version
 */

#ifndef __ALGO_SORT_H__
#define __ALGO_SORT_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rtthread.h>

#define ARRAY_SORT_MALLOC(size)   rt_malloc(size);
#define ARRAY_SORT_CALLOC(n,size) rt_calloc(n,size);
#define ARRAY_SORT_FREE(p)        rt_free(p);

struct sort_array
{
    int size; /* array size */
    int num;  /* array num */
    int *p;  /* array */
};

extern struct sort_array* sort_array_creat(unsigned int size);
extern int sort_array_init(struct sort_array *array, unsigned int size);
extern int bubble_sort(struct sort_array *array);
extern int insertion_sort(struct sort_array *array);
extern int selection_sort(struct sort_array *array);

extern int quick_sort(int *array, int l, int r);

extern void sort_test(void);

#endif
