/*
 * Copyright (c) 20019-2020, wanweiyingchuang
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-11-21    denghengli   the first version
 */

#ifndef __ALGO_SINGLE_LIST_H__
#define __ALGO_SINGLE_LIST_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rtthread.h>

#define SINGAL_LIST_TEST 0

#define SINGAL_LIST_MALLOC(size)   rt_malloc(size);
#define SINGAL_LIST_CALLOC(n,size) rt_calloc(n,size);
#define SINGAL_LIST_FREE(p)        rt_free(p);
#define SIGNAL_LIST_NAME_LEN 16

struct singal_list_node
{
    struct singal_list_node *next; /* next node*/
    char name[SIGNAL_LIST_NAME_LEN + 1];  /* node name */
    void *value; /* node value */
};

struct single_list
{
    struct singal_list_node *head; /* list head */
    unsigned int len; /* node num */
};

extern struct single_list* single_list_creat(void);
extern int single_list_init(struct single_list *list);
extern int single_list_add_node_head(struct single_list *list, char *name, void *value);
extern int single_list_add_node_tail(struct single_list *list, char *name, void *value);
extern int single_list_insert_node(struct single_list *list, struct singal_list_node *pos_node, char *name, void *value, char dir);
extern int single_list_node_del(struct single_list *list, struct singal_list_node *del_node, char isdyn);
extern int single_list_node_empty(struct single_list *list, char isdyn);
extern int single_list_destroy(struct single_list **list, char isdyn);
extern struct singal_list_node* single_list_node_find(struct single_list *list, char * name);

extern void single_list_test(void);

#endif
