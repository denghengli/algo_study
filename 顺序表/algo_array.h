/*
 * Copyright (c) 20019-2020, wanweiyingchuang
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-11-18     denghengli   the first version
 */

#ifndef __ALGO_ARRAY_H__
#define __ALGO_ARRAY_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAR_MALLOC(size)   malloc(size);
#define ARRAR_CALLOC(n,size) calloc(n,size);
#define ARRAY_FREE(p)        free(p);

struct dstrct_array
{
    unsigned int size; /* array total size */
    unsigned int used; /* array used size, from 1 to size */
    unsigned int tpsz; /* data type size */
    void *p; /* array, dynamically applied by array_init()*/
};

extern struct dstrct_array* array_creat(unsigned int size, unsigned int tpsz);
extern int array_init(struct dstrct_array *array, unsigned int size, unsigned int tpsz);
extern int array_insert (struct dstrct_array *array, unsigned int pos, void *const value);
extern int array_del_value (struct dstrct_array *array, void *value);
extern int array_del_pos   (struct dstrct_array *array, unsigned int pos);
extern int array_modify    (struct dstrct_array *array, unsigned int pos, void *const value);
extern int array_search_pos(struct dstrct_array *array, void* const value);
extern int array_empty  (struct dstrct_array *array);
extern int array_destroy(struct dstrct_array *array);

extern unsigned int array_used(struct dstrct_array *array);
extern unsigned int array_size(struct dstrct_array *array);

extern void algo_array_test(void);

#endif


