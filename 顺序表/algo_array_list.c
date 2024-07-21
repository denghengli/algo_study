/*
 * Copyright (c) 20019-2020, wanweiyingchuang
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-07-29     denghengli   the first version
 */

#include "algo_array_list.h"

/**
 * dynamically create a dynamic array, including the array header and data space.
 * 
 * @param cap: array size
 * @return NULL:malloc fail
 *        !NULL:success
 */
struct array_list* array_list_creat(unsigned int cap)
{
    struct array_list *list = NULL;
    void **array = NULL;
    
    list = ARRAY_LIST_MALLOC(sizeof(struct array_list));
    if(!list)
        return NULL;

    memset(list, 0, sizeof(struct array_list));
    array = ARRAY_LIST_MALLOC(sizeof(void *) * cap);
    if(array == NULL)
        return NULL;

    memset(array, 0, sizeof(void *) * cap);
    
    list->array = array;    
    list->cap = cap;
    list->used = 0;

    return list;
}


/**
 * 销毁一个线性表array_list
 * 
 * @param list: array_list
 * @return -1: fail
 *          0: success
 */
int array_list_destroy(struct array_list *list)
{
    int i = 0;
    
    if(list == NULL)
        return -1;

    /* 删除数组元素指向的指针数据 */
    for(i=0; i<list->used; i++)
    {
        if(list->array[i])
            ARRAY_LIST_FREE(list->array[i]);
        list->array[i] = NULL;
    }

    /* 删除数组元素 */
    if(list->array)
        ARRAY_LIST_FREE(list->array);
    list->array = NULL;
    
    /* 删除链表 */
    ARRAY_LIST_FREE(list);

    return 0;
}


/**
 * 将一个线性表array_list中的所有元素清空
 * 
 * @param list: array_list
 * @return -1: fail
 *          0: success
 */
int array_list_clear(struct array_list *list)
{
    int i = 0;
    
    if(list == NULL)
        return -1;

    /* 删除数组元素指向的指针数据 */
    for(i=0; i<list->used; i++)
    {
        if(list->array[i])
            ARRAY_LIST_FREE(list->array[i]);
        list->array[i] = NULL;
    }

    return 0;
}

/**
 * 向一个线性表array_list的pos位置处插入新元素
 * 
 * @param list: array_list
 * @return -1: fail
 *          0: success
 */
int array_list_insert(struct array_list *list, void *data)
{
    int ret = 0;
    
    if(list == NULL)
        return -1;

    /* 需要扩容 */
    if(list->used == list->cap)
    {
        ret = array_list_grow(list, list->cap * 2);
        if(ret == -1)
            return -1;
    }
    
    list->array[list->used] = data;
    list->used += 1;
    return 0;
}


/**
 * 删除一个线性表array_list的pos位置处的元素
 * 
 * @param list: array_list
 * @return -1: fail
 *          0: success
 */
int array_list_delete(struct array_list *list, unsigned int pos)
{
    void *del_data = NULL;
    int i = 0;
    
    if(list == NULL || pos >= list->used)
        return -1;

    del_data = list->array[pos];

    /* 向前移动 */
    for(i=pos+1; i<list->used; i++) {
        list->array[i-1] = list->array[i];
    }

    if(del_data)
        ARRAY_LIST_FREE(del_data);
    
    list->used -= 1;
    return 0;
}


/**
 * 增加array_list空间大小，返回新空间大小
 * 
 * @param list: array_list
 * @return -1: fail
 *          0: success
 */
int array_list_grow(struct array_list *list, int new_cap)
{
    void **new_array = NULL;
    
    if(list == NULL || new_cap < list->cap)
        return -1;

    /* 申请新的空间 */
    new_array = ARRAY_LIST_MALLOC(sizeof(void *) * new_cap);
    if(new_array == NULL)
        return -1;
    memset(new_array, 0, sizeof(void *) * new_cap);
    /* 拷贝 */
    memcpy(new_array, list->array, sizeof(void *) * list->used);

    /* 回收旧的空间 */
    ARRAY_LIST_FREE(list->array);

    /* 指向新的空间 */
    list->array = new_array;
    list->cap = new_cap;

    return 0;
}



