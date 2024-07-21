/*
 * Copyright (c) 20019-2020, wanweiyingchuang
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-11-21    denghengli   the first version
 */

#include "algo_single_list.h"

/**
 * dynamically create a single list.
 * 
 * @return NULL:malloc fail
 *        !NULL:success
 */
struct single_list* single_list_creat(void)
{
    struct single_list *list = NULL;

    list = SINGAL_LIST_MALLOC(sizeof(*list));
    if (list == NULL)
        return NULL;

    list->head = NULL;
    list->len = 0;

    return list;
}

/**
 * init a single list.
 * 
 * @param list: single list
 * @return -1:list is null
 *          0:success
 */
int single_list_init(struct single_list *list)
{
    if (list == NULL)
        return -1;
	
	list->head = NULL;
	list->len = 0;
	
	return 0;
}

/**
 * insert a value to the single list forward.
 *
 * @param list: single list
 * @param value: data
 * @return -1:list is null or value is null
 *         -2:node malloc fail
 *         -3:used is greater than size
 *          0:success
 */
int single_list_add_node_head(struct single_list *list, char *name, void *value)
{
    struct  singal_list_node *node = NULL;
    int name_len = 0;
    
    if (list == NULL || name == NULL || value == NULL)
        return -1;
    
    node = SINGAL_LIST_MALLOC(sizeof(*node));
    if (node == NULL)
        return -2;

    /* add node */
    memset(node->name, 0, sizeof(node->name));
    name_len = (strlen(name) > SIGNAL_LIST_NAME_LEN) ? SIGNAL_LIST_NAME_LEN : strlen(name);
    strncpy(node->name, name, name_len);
    node->value = value;
    node->next = list->head;

    /* move list head to first node */
    list->head = node;

    list->len++;

    return 0;
}


/**
 * insert a value to the single list backwards.
 *
 * @param list: single list
 * @param value: data
 * @return -1:list is null or value is null
 *         -2:node malloc fail
 *          0:success
 */
int single_list_add_node_tail(struct single_list *list, char *name, void *value)
{
    struct  singal_list_node *node = NULL;
    struct  singal_list_node *tail = NULL;
    int name_len = 0;
    
    if (list == NULL || value == NULL)
        return -1;
    
    node = SINGAL_LIST_MALLOC(sizeof(*node));
    if (node == NULL)
        return -2;
    
    memset(node->name, 0, sizeof(node->name));
    name_len = (strlen(name) > SIGNAL_LIST_NAME_LEN) ? SIGNAL_LIST_NAME_LEN : strlen(name);
    strncpy(node->name, name, name_len);
    node->value = value;
    node->next = NULL;

    /* first node */
    if (list->head == NULL && list->len == 0)
        list->head = node;
    /* not the first node */
    else
    {
        tail = list->head;
        while (tail->next != NULL)
        {
            tail = tail->next;
        }
        tail->next = node;
    }
    
    list->len++;

    return 0;
}

/**
 * Insert a new node before or after a node.
 *
 * @param list: single list
 * @param pos_node: insert node postion
 * @param value: data
 * @param dir: 1 before, 2 after
 * @return -1:list is null or value is null
 *         -2:node malloc fail
 *         -3:pos node is not in the list
 *          0:success
 */
int single_list_insert_node(struct single_list *list, struct singal_list_node *pos_node, char *name, void *value, char dir)
{
    struct  singal_list_node *node = NULL;
    struct  singal_list_node *prev = NULL;
    int name_len = 0;
    
    if (list == NULL || pos_node == NULL || value == NULL)
        return -1;
    
    node = SINGAL_LIST_MALLOC(sizeof(*node));
    if (node == NULL)
        return -2;

    memset(node->name, 0, sizeof(node->name));
    name_len = (strlen(name) > SIGNAL_LIST_NAME_LEN) ? SIGNAL_LIST_NAME_LEN : strlen(name);
    strncpy(node->name, name, name_len);
    node->value = value;
    node->next = NULL;

    /* Insert before the node*/
    if (dir == 1)
    {
        /* before the head */
        if (pos_node == list->head)
        {
            node->next = list->head;
            list->head = node;
        }
        else
        {
            prev = list->head;
            while (pos_node != prev->next)
            {
                prev = prev->next;
                if (prev == NULL) /* pos node is not in the list */
                    return -3;
            }
            node->next = pos_node;
            prev->next = node;
        }
    }
    /* insert after the node */
    else if (dir == 2)
    {
        node->next = pos_node->next;
        pos_node->next = node;
    }

    list->len++;

    return 0;
}

/**
 * delete a node.
 * @note The value space may be static, so the val_isdyn identification is needed 
 *       to tell if the value space needs to be released manually
 * @param list: single list
 * @param isdyn: 1 = value is dynamic apply
 * @return -1:list is null
 *         -3:delete node is not in the list
 *          0:success
 */
int single_list_node_del(struct single_list *list, struct singal_list_node *del_node, char isdyn)
{
    struct  singal_list_node *prev = NULL;
    struct  singal_list_node *cur = NULL;

    if (list == NULL || del_node == NULL)
        return -1;

    cur = list->head;
    while (cur != NULL && cur != del_node)
    {   
        prev = cur;
        cur = cur->next;
    }
    
    /* delete node is not in the list */
    if (cur == NULL)
        return -3;

    /* delete node is the head node */
    if (cur == list->head)
        list->head = cur->next;
    else
        prev->next = cur->next;
    
    cur->next = NULL;
    if (isdyn)
    {
        //SINGAL_LIST_FREE(cur->name);
        SINGAL_LIST_FREE(cur->value);
        //cur->name = NULL;
		cur->value = NULL;
    }
    SINGAL_LIST_FREE(cur);
    cur = NULL;

    list->len--;

    return 0;
}

/**
 * empty the single list all node.
 * @note The value space may be static, so the val_isdyn identification is needed 
 *       to tell if the value space needs to be released manually
 * @param list: single list
 * @param isdyn: 1 = value is dynamic apply
 * @return -1:list is null
 *          0:success
 */
struct singal_list_node* single_list_node_find(struct single_list *list, char * name)
{
    struct  singal_list_node *cur = NULL;
    
    if (list == NULL || name == NULL)
        return NULL;

    cur = list->head;
    while (cur != NULL)
    {   
        if (strncmp(cur->name, name, SIGNAL_LIST_NAME_LEN) == 0)
            return cur;

        cur = cur->next;
    }
    
    /* node is not in the list */
    if (cur == NULL)
        return NULL;
    
    return cur;
}

/**
 * empty the single list all node.
 * @note The value space may be static, so the val_isdyn identification is needed 
 *       to tell if the value space needs to be released manually
 * @param list: single list
 * @param isdyn: 1 = value is dynamic apply
 * @return -1:list is null
 *          0:success
 */
int single_list_node_empty(struct single_list *list, char isdyn)
{
    struct  singal_list_node *node = NULL;

    if (list == NULL)
        return -1;
    
    while (list->head != NULL && list->len > 0)
    {
        /* move list head node to head next node */
        node = list->head;
        list->head = list->head->next;
        list->len--;
        if (isdyn)
        {
            //SINGAL_LIST_FREE(node->name);
            SINGAL_LIST_FREE(node->value);
            //node->name = NULL;
			node->value = NULL;
        }

        /* free space */
		node->next = NULL;
        SINGAL_LIST_FREE(node);
        node = NULL;
    }
    
    return 0;
}

/**
 * destroy the single list.
 * @note The list node needs to be empty before destroy list
 * @param list: single list
 * @return -1:list is null
 *          0:success
 */
int single_list_destroy(struct single_list **list, char val_isdyn)
{
    if (single_list_node_empty(*list, val_isdyn) != 0)
		return -1;
	
	//*((*list)->head) = NULL;
    SINGAL_LIST_FREE(*list);
    *list = NULL;

    return 0;
}


#if SINGAL_LIST_TEST

struct single_list *single_list;
struct singal_list_node *single_node;
int single_test_w[] = {10, 20, 30, 40, 50};
int single_test_r[15] = {0};
char *test_name = "single_list";
void single_list_test(void)
{
	int i = 0;
	
    single_list = single_list_creat();
    
	/* head insert */
    single_list_add_node_head(single_list, (void*)test_name, (void*)&single_test_w[0]);
    single_list_add_node_head(single_list, (void*)test_name, (void*)&single_test_w[1]);
    single_list_add_node_head(single_list, (void*)test_name, (void*)&single_test_w[2]);
    single_list_add_node_head(single_list, (void*)test_name, (void*)&single_test_w[3]);
    single_list_add_node_head(single_list, (void*)test_name, (void*)&single_test_w[4]);
    
	single_node = single_list->head;
	for (i=0; i<single_list->len; i++)
	{
		single_test_r[i] = *(int*)single_node->value;//50 40 30 20 10 
		single_node = single_node->next;
	}
	
	/* tail insert */
	single_list_node_empty(single_list, 0);
    single_list_add_node_tail(single_list, (void*)test_name, (void*)&single_test_w[0]);
    single_list_add_node_tail(single_list, (void*)test_name, (void*)&single_test_w[1]);
    single_list_add_node_tail(single_list, (void*)test_name, (void*)&single_test_w[2]);
    single_list_add_node_tail(single_list, (void*)test_name, (void*)&single_test_w[3]);
    single_list_add_node_tail(single_list, (void*)test_name, (void*)&single_test_w[4]);
	
	single_node = single_list->head;
	for (i=0; i<single_list->len; i++)
	{
		single_test_r[i] = *(int*)single_node->value;//10 20 30 40 50 
		single_node = single_node->next;
	}
	
	/* pos insert */
    single_list_insert_node(single_list, single_list->head, (void*)test_name, (void*)&single_test_w[0], 1);
    single_list_insert_node(single_list, single_list->head, (void*)test_name, (void*)&single_test_w[1], 1);
    single_list_insert_node(single_list, single_list->head, (void*)test_name, (void*)&single_test_w[2], 1);
    single_list_insert_node(single_list, single_list->head, (void*)test_name, (void*)&single_test_w[3], 1);
    single_list_insert_node(single_list, single_list->head, (void*)test_name, (void*)&single_test_w[4], 1);
    single_list_insert_node(single_list, single_list->head, (void*)test_name, (void*)&single_test_w[0], 2);
    single_list_insert_node(single_list, single_list->head, (void*)test_name, (void*)&single_test_w[1], 2);
    single_list_insert_node(single_list, single_list->head, (void*)test_name, (void*)&single_test_w[2], 2);
    single_list_insert_node(single_list, single_list->head, (void*)test_name, (void*)&single_test_w[3], 2);
    single_list_insert_node(single_list, single_list->head, (void*)test_name, (void*)&single_test_w[4], 2);
	
	single_node = single_list->head;
	for (i=0; i<single_list->len; i++)
	{
		single_test_r[i] = *(int*)single_node->value;
		single_node = single_node->next;
	}
	
	/* destroy list */
	single_list_destroy(&single_list, 0);
}


#endif





