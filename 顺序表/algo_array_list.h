/*
 * Copyright (c) 20019-2020, wanweiyingchuang
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-07-29     denghengli   the first version
 */

#ifndef __ALGO_ARRAY_LIST_H__
#define __ALGO_ARRAY_LIST_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rtthread.h>

#define ARRAY_LIST_MALLOC(size)   rt_malloc(size)
#define ARRAY_LIST_CALLOC(n,size) rt_calloc(n,size)
#define ARRAY_LIST_FREE(p)        rt_free(p)

#define ARRAY_LIST_GROW_STEP 50

struct array_list
{
    unsigned int cap;   /* array total size */
    unsigned int used;  /* array used size, from 1 to size */
    void **array; /* array, dynamically applied by array_init()*/
};

/* 创建并且返回一个空的array_list */
extern struct array_list* array_list_creat(unsigned int cap);
/* 销毁一个线性表array_list */
extern int array_list_destroy(struct array_list *list);
/* 将一个线性表array_list中的所有元素清空 */
extern int array_list_clear(struct array_list *list);
/* 向一个线性表array_list的最后位置处插入新元素,元素指向data */
extern int array_list_insert(struct array_list *list, void *data);
/* 删除一个线性表array_list的pos位置处的元素 */
extern int array_list_delete(struct array_list *list, unsigned int pos);

/* 增加array_list空间大小，返回新空间大小 */
extern int array_list_grow(struct array_list *list, int new_cap);

#endif



