/*
 * Copyright (c) 20019-2020, wanweiyingchuang
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-01-07     denghengli   the first version
 */

#ifndef __ALGO_BS_TREE_H__
#define __ALGO_BS_TREE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FreeRTOS.h"
#include "algo_double_list.h"

#define BS_TREE_MALLOC(size)    pvPortMalloc(size);
#define BS_TREE_REALLOC(p,size) realloc(p,size);
#define BS_TREE_CALLOC(n,size)  calloc(n,size);
#define BS_TREE_FREE(p)         vPortFree(p);

struct bs_tree_node;
struct bs_tree;

/* 
 * 二叉查找树key比较, key_cmp:传入的要比较的key, key_becmp:被比较的key
 * 任意一个节点，其左子树中任意一个节点的值，都要小于这个节点的值
 * 任意一个几点，其右子数中任意一个节点的值，都要大于这个节点的值
 * 返回值 > 0 : key_cmp > key_becmp
 * 返回值 = 0 : key_cmp = key_becmp
 * 返回值 < 0 : key_cmp < key_becmp
*/
typedef int (*bstree_keycmp)(struct bs_tree *bstree, const void *key_cmp, const void *key_becmp);
/* 二叉树中的节点数据删除函数,如果插入节点为动态分配,则需要在该函数中释放节点空间 */
typedef int (*bstree_value_free)(struct bs_tree_node *node);

struct bs_tree_node
{
    void *key;
    void *value;
    struct bs_tree_node *left_child;  /*左子树*/
    struct bs_tree_node *right_child; /*右子树*/
};

struct bs_tree
{
    int num;                        /*二叉树中节点个数的总和*/
    struct bs_tree_node *root;      /*二叉树的根节点*/
    bstree_keycmp        keycmp;    /*二叉树key比较*/
    bstree_value_free    valuefree; /*二叉树节点数据删除*/
};

#define BSTREE_IS_EMPTY(tree) (tree->num == 0)

/*根据当前结构体元素的地址，获取到结构体首地址*/
//#define OFFSETOF(TYPE,MEMBER) ((unsigned int)&((TYPE *)0)->MEMBER)
//#define container(ptr,type,member) ({\
//  const typeof( ((type *)0)->member) *__mptr = (ptr);\
//  (type *) ((char *)__mptr - OFFSETOF(type,member));})

#define OFFSETOF(TYPE,MEMBER) ((unsigned int)&((TYPE *)0)->MEMBER)
#define container(ptr,type,member) ((type *) ((char *)ptr - OFFSETOF(type,member)))

extern struct bs_tree *bs_tree_creat(bstree_keycmp keycmp, bstree_value_free valuefree);
extern struct bs_tree *bs_tree_creat_default(bstree_value_free valuefree);
extern int    bs_tree_insert(struct bs_tree *bstree, void *key, void *value);
extern int    bs_tree_delete(struct bs_tree *bstree, void *key);
extern int    bs_tree_modify(struct bs_tree *bstree, void *key, void *value);
extern int    bs_tree_search(struct bs_tree *bstree, void *key, struct double_list *dlist);
extern void * bs_tree_search_min(struct bs_tree *bstree);
extern void * bs_tree_search_max(struct bs_tree *bstree);
extern void   bs_tree_node_empty(struct bs_tree **bstree, struct bs_tree_node **node);
extern void   bs_tree_destroy   (struct bs_tree **bstree);

/*中序遍历二叉树，并将节点数据放入双向链表中，链表在使用完释放空间的时候，不能将节点数据空间删除*/
extern int bs_tree_inorder(struct bs_tree *bstree, struct bs_tree_node *node, struct double_list *dlist);

extern void bs_tree_sample(void);

#endif



