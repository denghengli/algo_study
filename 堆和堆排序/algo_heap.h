/*
 * Copyright (c) 20019-2020, wanweiyingchuang
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-01-14     denghengli   the first version
 */

#ifndef __ALGO_HEAP_H__
#define __ALGO_HEAP_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FreeRTOS.h"

#define HEAP_MALLOC(size)    pvPortMalloc(size);
#define HEAP_REALLOC(p,size) realloc(p,size);
#define HEAP_CALLOC(n,size)  calloc(n,size);
#define HEAP_FREE(p)         vPortFree(p);

#define HEAP_MAX_SIZE        100

struct heap_node;
struct heap;

/* 
 * 堆key比较, key_cmp:传入的要比较的key, key_becmp:被比较的key
 * 任意一个节点，其左子树中任意一个节点的值，都要小于这个节点的值
 * 任意一个几点，其右子数中任意一个节点的值，都要大于这个节点的值
 * 返回值 > 0 : key_cmp > key_becmp
 * 返回值 = 0 : key_cmp = key_becmp
 * 返回值 < 0 : key_cmp < key_becmp
*/
typedef int (*heap_keycmp)(struct heap *heap, const void *key_cmp, const void *key_becmp);
/* 堆中的节点数据删除函数,如果插入节点为动态分配,则需要在该函数中释放节点空间 */
typedef int (*heap_value_free)(struct heap_node *node);

struct heap_node
{
    void *key;
    void *value;
};

struct heap
{
    int num; /*堆中已经存储的元素个数*/
    struct heap_node  array[HEAP_MAX_SIZE];  /*堆*/
    heap_keycmp       keycmp;                /*堆key比较*/
    heap_value_free   valuefree;             /*堆节点数据删除*/
};

/*根据当前结构体元素的地址，获取到结构体首地址*/
//#define OFFSETOF(TYPE,MEMBER) ((unsigned int)&((TYPE *)0)->MEMBER)
//#define container(ptr,type,member) ({\
//  const typeof( ((type *)0)->member) *__mptr = (ptr);\
//  (type *) ((char *)__mptr - OFFSETOF(type,member));})

#define OFFSETOF(TYPE,MEMBER) ((unsigned int)&((TYPE *)0)->MEMBER)
#define container(ptr,type,member) ((type *) ((char *)ptr - OFFSETOF(type,member)))

extern struct heap *heap_creat(heap_keycmp keycmp, heap_value_free valuefree);
extern struct heap *heap_creat_default(heap_value_free valuefree);
extern int  heap_insert    (struct heap *heap, void *key, void *value);
extern int  heap_delete_max(struct heap *heap);
extern int  heap_build     (struct heap *heap);
extern int  heap_sort      (struct heap *heap);
extern void heap_empty     (struct heap *heap);
extern void heap_destroy   (struct heap **heap);

extern void heap_sample(void);

#endif



