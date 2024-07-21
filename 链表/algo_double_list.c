/*
 * Copyright (c) 20019-2020, wanweiyingchuang
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-11-18     denghengli   the first version
 */

#include "algo_double_list.h"

/**
 * dynamically create a double list.
 * 
 * @return NULL:malloc fail
 *        !NULL:success
 */
struct double_list* double_list_creat(void)
{
    struct double_list *list = NULL;

    list = DOUBLE_LIST_MALLOC(sizeof(*list));
    if (list == NULL)
        return NULL;

    list->head = NULL;
    list->tail = NULL;
    list->len = 0;

    return list;
}

/**
 * init a double list.
 * 
 * @param list: double list
 * @return -1:list is null
 *          0:success
 */
int double_list_init(struct double_list *list)
{
    if (list == NULL)
        return -1;
	
    list->head = NULL;
    list->tail = NULL;
    list->len = 0;
	
	return 0;
}


/**
 * insert a value to the double list forward.
 *
 * @param list: double list
 * @param value: data
 * @return -1:list is null or value is null
 *         -2:node malloc fail
 *          0:success
 */
int double_list_add_node_head(struct double_list *list, void *value)
{
    struct  double_list_node *node = NULL;

    if (list == NULL || value == NULL)
        return -1;
    
    node = DOUBLE_LIST_MALLOC(sizeof(*node));
    if (node == NULL)
        return -2;

    /* add node */
    node->value = value;
    node->prev = NULL;
    node->next = list->head;

    /* move list tail to end node */
    if (list->tail == NULL)
    {
        list->tail = node;
    }

    /* move list head to first node */
    list->head = node;

    list->len++;

    return 0;
}

/**
 * insert a value to the double list backwards.
 *
 * @param list: double list
 * @param value: data
 * @return -1:list is null or value is null
 *         -2:node malloc fail
 *          0:success
 */
int double_list_add_node_tail(struct double_list *list, void *value)
{
    struct  double_list_node *node = NULL;

    if (list == NULL || value == NULL)
        return -1;
    
    node = DOUBLE_LIST_MALLOC(sizeof(*node));
    if (node == NULL)
        return -2;

    node->value = value;
    node->next = NULL;
    node->prev = NULL;

    /* first node */
    if (list->head == NULL && list->tail == NULL && list->len == 0)
    {
        list->head = node;
        list->tail = node;
    }
    /* not the first node */
    else
    {
        node->prev = list->tail;
        list->tail->next = node;
        list->tail = node;
    }
    
    list->len++;

    return 0;
}

/**
 * Insert a new node before or after a node.
 *
 * @param list: double list
 * @param pos_node: insert node postion
 * @param value: data
 * @param dir: 1 before, 2 after
 * @return -1:list is null or value is null
 *         -2:node malloc fail
 *         -3:pos node is not in the list
 *          0:success
 */
int double_list_insert_node(struct double_list *list, struct double_list_node *pos_node, void *value, char dir)
{
    struct  double_list_node *node = NULL;
    struct  double_list_node *temp = NULL;
    unsigned int i = 0;

    if (list == NULL || pos_node == NULL || value == NULL)
        return -1;
    
    node = DOUBLE_LIST_MALLOC(sizeof(*node));
    if (node == NULL)
        return -2;

    for (i=0,temp=list->head; i < list->len; i++,temp=temp->next)
    {
        if (temp == pos_node)
            break;
    }
    /* delete node is not in the list */
    if (i == list->len)
        return -3;

    node->value = value;
    node->next = NULL;
    node->prev = NULL;

    /* Insert before the node*/
    if (dir == 1)
    {
        /* before the head */
        if (pos_node == list->head)
        {
            pos_node->prev = node;
            node->next = pos_node;
            list->head = node;
        }
        else
        {
            pos_node->prev->next = node;
            node->prev = pos_node->prev;
            node->next = pos_node;
            pos_node->prev = node;
        }
    }
    /* insert after the node */
    else if (dir == 2)
    {
        if (pos_node == list->tail)
        {
            pos_node->next = node;
            node->prev = pos_node;
            list->tail = node;
        }
        else
        {
            node->next = pos_node->next;
            pos_node->next->prev = node;
            pos_node->next = node;
            node->prev = pos_node;
        }
    }

    list->len++;

    return 0;
}

/**
 * delete a node.
 * @note The value space may be static, so the val_isdyn identification is needed 
 *       to tell if the value space needs to be released manually
 * @param list: double list
 * @param val_isdyn: 1 = value is dynamic apply
 * @return -1:list is null
 *         -3:delete node is not in the list
 *          0:success
 */
int double_list_node_del(struct double_list *list, struct double_list_node *del_node, char val_isdyn)
{
    struct  double_list_node *temp = NULL;
    unsigned int i = 0;

    if (list == NULL || del_node == NULL)
        return -1;

    for (i=0,temp=list->head; i < list->len; i++,temp=temp->next)
    {
        if (temp == del_node)
            break;
    }
    /* delete node is not in the list */
    if (i == list->len)
        return -3;

    /* delete node is the head node */
    if (del_node == list->head)
    {
        del_node->next->prev = NULL;
        list->head = del_node->next;
    }
    /* delete node is the tail node */
    else if (del_node == list->tail)
    {
        del_node->prev->next = NULL;
        list->tail = del_node->prev;
    }
    else
    {
       del_node->prev->next = del_node->next;
       del_node->next->prev = del_node->prev;
    }
    
    del_node->next = NULL;
    del_node->prev = NULL;
    if (val_isdyn)
    {
       DOUBLE_LIST_FREE(del_node->value);
	   del_node->value = NULL;
    }
    DOUBLE_LIST_FREE(del_node);
    del_node = NULL;

    list->len--;

    return 0;
}


/**
 * empty the double list all node.
 * @note The value space may be static, so the val_isdyn identification is needed 
 *       to tell if the value space needs to be released manually
 * @param list: double list
 * @param val_isdyn: 1 = value is dynamic apply
 * @return -1:list is null
 *          0:success
 */
int double_list_node_empty(struct double_list *list, char val_isdyn)
{
    struct  double_list_node *node = NULL;

    if (list == NULL)
        return -1;
    
    while (list->head != NULL && list->len > 0)
    {
        /* move list head node to head next node */
        node = list->head;
        list->head = list->head->next;
        list->len--;
        if (val_isdyn)
        {
            DOUBLE_LIST_FREE(node->value);
			node->value = NULL;
        }

        /* free space */
        node->next = NULL;
        node->prev = NULL;
        DOUBLE_LIST_FREE(node);
        node = NULL;
    }
    
	list->tail = NULL;
	
    return 0;
}

/**
 * destroy the double list.
 * @note The list node needs to be empty before destroy list
 * @param list: double list
 * @return -1:list is null
 *          0:success
 */
int double_list_destroy(struct double_list *list, char val_isdyn)
{
    if (double_list_node_empty(list, val_isdyn) != 0)
		return -1;
	
	list->head = NULL;
	list->tail = NULL;
    DOUBLE_LIST_FREE(list);
    list = NULL;

    return 0;
}



struct double_list *double_list;
struct double_list_node *double_node;
int double_test_w[] = {10, 20, 30, 40, 50};
int double_test_r[15] = {0};
void double_list_test(void)
{
	int i = 0;
	
    double_list = double_list_creat();
    
	/* head insert */
    double_list_add_node_head(double_list, (void*)&double_test_w[0]);
    double_list_add_node_head(double_list, (void*)&double_test_w[1]);
    double_list_add_node_head(double_list, (void*)&double_test_w[2]);
    double_list_add_node_head(double_list, (void*)&double_test_w[3]);
    double_list_add_node_head(double_list, (void*)&double_test_w[4]);
    
	double_node = double_list->head;
	for (i=0; i<double_list->len; i++)
	{
		double_test_r[i] = *(int*)double_node->value;//50 40 30 20 10
		double_node = double_node->next;
	}
	
	/* tail insert */
	double_list_node_empty(double_list, 0);
    double_list_add_node_tail(double_list, (void*)&double_test_w[0]);
    double_list_add_node_tail(double_list, (void*)&double_test_w[1]);
    double_list_add_node_tail(double_list, (void*)&double_test_w[2]);
    double_list_add_node_tail(double_list, (void*)&double_test_w[3]);
    double_list_add_node_tail(double_list, (void*)&double_test_w[4]);
	
	double_node = double_list->head;
	for (i=0; i<double_list->len; i++)
	{
		double_test_r[i] = *(int*)double_node->value;//10 20 30 40 50 
		double_node = double_node->next;
	}
	
	/* pos insert */
    double_list_insert_node(double_list, double_list->head, (void*)&double_test_w[0], 1);
    double_list_insert_node(double_list, double_list->head, (void*)&double_test_w[1], 1);
    double_list_insert_node(double_list, double_list->head, (void*)&double_test_w[2], 1);
    double_list_insert_node(double_list, double_list->head, (void*)&double_test_w[3], 1);
    double_list_insert_node(double_list, double_list->head, (void*)&double_test_w[4], 1);
	double_list_insert_node(double_list, double_list->head, (void*)&double_test_w[0], 2);
	double_list_insert_node(double_list, double_list->head, (void*)&double_test_w[1], 2);
	double_list_insert_node(double_list, double_list->head, (void*)&double_test_w[2], 2);
	double_list_insert_node(double_list, double_list->head, (void*)&double_test_w[3], 2);
	double_list_insert_node(double_list, double_list->head, (void*)&double_test_w[4], 2);
	
	double_node = double_list->head;
	for (i=0; i<double_list->len; i++)
	{
		double_test_r[i] = *(int*)double_node->value;
		double_node = double_node->next;
	}
	
	/* destroy list */
	double_list_destroy(double_list, 0);
}



