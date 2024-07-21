/*
 * Copyright (c) 20019-2020, wanweiyingchuang
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-11-18     denghengli   the first version
 */

#ifndef __ALGO_DOUBLE_LIST_H__
#define __ALGO_DOUBLE_LIST_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DOUBLE_LIST_MALLOC(size)   malloc(size);
#define DOUBLE_LIST_CALLOC(n,size) calloc(n,size);
#define DOUBLE_LIST_FREE(p)        free(p);

struct double_list_node
{
	struct double_list_node *prev; /* prev node*/
    struct double_list_node *next; /* next node*/
    void *value; /* node value */
};

struct double_list
{
    struct double_list_node *head; /* list head */
    struct double_list_node *tail; /* list head */
    unsigned int len; /* node num */
};

extern struct double_list* double_list_creat(void);
extern int double_list_init(struct double_list *list);
extern int double_list_add_node_head(struct double_list *list, void *value);
extern int double_list_add_node_tail(struct double_list *list, void *value);
extern int double_list_insert_node  (struct double_list *list, struct double_list_node *pos_node, void *value, char dir);
extern int double_list_node_del     (struct double_list *list, struct double_list_node *del_node, char val_isdyn);
extern int double_list_node_empty   (struct double_list *list, char val_isdyn);
extern int double_list_destroy      (struct double_list *list, char val_isdyn);

extern void double_list_test(void);

#endif