/*
 * Copyright (c) 20019-2020, wanweiyingchuang
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-12-27     denghengli   the first version
 */

#ifndef __ALGO_HASH_TABLE_H__
#define __ALGO_HASH_TABLE_H__

//#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rtthread.h>

#define HASH_TABLE_MALLOC(size)    rt_malloc(size);
#define HASH_TABLE_REALLOC(p,size) rt_realloc(p,size);
#define HASH_TABLE_CALLOC(n,size)  rt_calloc(n,size);
#define HASH_TABLE_FREE(p)         rt_free(p);

struct hash_table_node;
struct hash_table;

/* 哈希函数,根据key计算哈希值 */
typedef int (*hash_fun)(struct hash_table *table, const void *key);
/* 
 *哈希key比较, key_cmp:传入的要比较的key, key_becmp:哈希表中被比较的key
 * hash桶中的元素是从小到大排列的，
 * 返回值 > 0 : key_cmp > key_becmp
 * 返回值 = 0 : key_cmp = key_becmp
 * 返回值 < 0 : key_cmp < key_becmp
*/
typedef int (*hash_keycmp)(struct hash_table *table, const void *key_cmp, const void *key_becmp);
/* hash桶中的节点数据删除函数,如果插入节点为动态分配,则需要在该函数中释放节点空间 */
typedef int (*node_value_free)(struct hash_table_node *node);

struct hash_table_node
{
    void *key;
    void *value;
    struct hash_table_node *next; /*哈希桶节点下个节点*/
};

struct hash_table
{
    int size; /*哈希桶的大小,即数组的大小*/
    int num;  /*各个哈希桶中节点个数的总和*/
    hash_fun hashfun; /*哈希函数*/
    hash_keycmp keycmp; /*哈希key比较*/
    node_value_free valuefree; /*哈希桶节点数据删除*/
    struct hash_table_node **tables; /*哈希桶,其实就是一个数组*/
};

/*根据当前结构体元素的地址，获取到结构体首地址*/
//#define OFFSETOF(TYPE,MEMBER) ((unsigned int)&((TYPE *)0)->MEMBER)
//#define container(ptr,type,member) ({\
//  const typeof( ((type *)0)->member) *__mptr = (ptr);\
//  (type *) ((char *)__mptr - OFFSETOF(type,member));})

#define OFFSETOF(TYPE,MEMBER) ((unsigned int)&((TYPE *)0)->MEMBER)
#define container(ptr,type,member) ((type *) ((char *)ptr - OFFSETOF(type,member)))

extern struct hash_table *hash_table_creat(int size, hash_fun hashfun, hash_keycmp keycmp, node_value_free valuefree);
extern struct hash_table *hash_table_creat_default(int size, node_value_free valuefree);
extern int    hash_table_insert(struct hash_table *hashtable, void *key, void *value);
extern int    hash_table_delete(struct hash_table *hashtable, void *key);
extern int    hash_table_modify(struct hash_table *hashtable, void *key, void *value);
extern void * hash_table_search(struct hash_table *hashtable, void *key);

extern void hash_table_sample(void);

#endif



