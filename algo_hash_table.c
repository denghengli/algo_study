/*
 * Copyright (c) 20019-2020, wanweiyingchuang
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-12-27     denghengli   the first version
 */

#include "algo_hash_table.h"

/**
 * 默认使用的哈希函数.
 * 
 * @return 哈希值
 */
static int hash_fun_default(struct hash_table *table, const void *key)
{
    unsigned int hash = 0;
    unsigned int seed = 131;
	char *temp = NULL;
	
	temp = (char*)key;
    while(*temp)
    {
        hash = hash * seed + *temp++;
    }
    
    hash &= 0x7FFFFFFF;
    hash %= table->size;

    return hash;
}

/**
 * 哈希key比较, key_cmp:传入的要比较的key, key_becmp:哈希表中被比较的key
 * 
 * @return > 0 : key_cmp > key_becmp
 * @return = 0 : key_cmp = key_becmp
 * @return < 0 : key_cmp < key_becmp
 * 
 */
static int hash_keycmp_default(struct hash_table *table, const void *key_cmp, const void *key_becmp)
{
    return strcmp(key_cmp, key_becmp);
}

/**
 * 动态创建一个哈希表.
 * 
 * @return NULL:创建失败
 *        !NULL:创建成功
 */
struct hash_table *hash_table_creat(int size, hash_fun hashfun, hash_keycmp keycmp, node_value_free valuefree)
{
    struct hash_table *hashtable = NULL;
    struct hash_table_node **tables = NULL;
    int i = 0;

    if (size < 0 || hashfun == NULL || keycmp == NULL)
        return NULL;

    /*申请哈希表结构空间*/
    hashtable = HASH_TABLE_MALLOC(sizeof(*hashtable));
    if (hashtable == NULL)
        return NULL;
    
    /*申请哈希桶数据空间,实际就是申请size大小的数组空间*/
    tables = (struct hash_table_node**)HASH_TABLE_MALLOC(size * sizeof(tables));
    if (tables == NULL)
        return NULL;

    hashtable->num       = 0;
    hashtable->size      = size;
    hashtable->tables    = tables;
    hashtable->hashfun   = hashfun;
    hashtable->keycmp    = keycmp;
    hashtable->valuefree = valuefree;

    /*清零哈希桶(数组)空间*/
    for (i = 0; i < size; i++)
    {
        hashtable->tables[i] = NULL;
    }
    
    return hashtable;
}

/**
 * 使用默认的函数函数、key比较函数、节点删除函数 动态创建一个哈希表.
 * 
 * @return NULL:创建失败
 *        !NULL:创建成功
 */
struct hash_table *hash_table_creat_default(int size, node_value_free valuefree)
{
    return hash_table_creat(size, hash_fun_default, hash_keycmp_default, valuefree);
}


/**
 * 向一个哈希桶插入一个节点,有3种情况:
 * 1、prev==NULL,插入位置是头结点 2、key小于cur->key 3、cur==NULL,链表尾插入
 * 
 * @param hashtable: 散列表
 * @param key: 关键值
 * @param value: 节点数据
 * @param value: 节点数据大小
 * 
 * @return 0:插入成功
 *        -1:哈希表不存在 或 key为空 或 value为空
 *        -2:节点已经存在
 */
int hash_table_insert(struct hash_table *hashtable, void *key, void *value)
{
    struct hash_table_node *prev = NULL;
    struct hash_table_node *cur = NULL;
    struct hash_table_node *new_node = NULL;
    int hvalue = 0;

    if (hashtable == NULL || key == NULL || value == NULL)
        return -1;

    /*根据key计算出哈希值*/
    hvalue = hashtable->hashfun(hashtable, key);
    cur = hashtable->tables[hvalue];

    /*hash桶中的元素是从小到大排列的,查找要插入的位置*/
    while((cur != NULL) && (hashtable->keycmp(hashtable, key, cur->key) > 0))
    {
        prev = cur;
        cur = cur->next;
    }

    /*如果key相同,表示节点以及存在,直接返回*/
    if ((cur != NULL) && (hashtable->keycmp(hashtable, key, cur->key) == 0))
        return -2;

    /*插入新增节点*/
    new_node = (struct hash_table_node*)HASH_TABLE_MALLOC(sizeof(*new_node));
    if (new_node == NULL)
        return NULL;

    new_node->key = key;
    new_node->value = value;
    if (prev == NULL) /*第1种情况*/
    {
        new_node->next = NULL;
		hashtable->tables[hvalue] = new_node;/*这里不能为cur = new_node,因为此时的cur和table[hvalue]都是无效的*/
    }
    else /*第2、3种情况*/
    {
        new_node->next = cur;
        prev->next = new_node;
    }
    
    hashtable->num ++;

    return 0;
}


/**
 * 删除一个节点.
 * 
 * @param hashtable: 散列表
 * @param key: 删除节点关键值
 * 
 * @return 0:删除成功
 *        -1:哈希表不存在 或 key为空
 *        -2:节点不存在
 */
int hash_table_delete(struct hash_table *hashtable, void *key)
{
    struct hash_table_node *prev = NULL;
    struct hash_table_node *cur = NULL;
    int hvalue = 0;

    if (hashtable == NULL || key == NULL)
        return -1;

    /*根据key计算出哈希值*/
    hvalue = hashtable->hashfun(hashtable, key);
    cur = hashtable->tables[hvalue];

    /*hash桶中的元素是从小到大排列的,查找要删除的位置*/
    while((cur != NULL) && (hashtable->keycmp(hashtable, key, cur->key) >= 0))
    {
        if (hashtable->keycmp(hashtable, key, cur->key) == 0)
        {
            if (prev == NULL)/*如果删除的是头结点*/
            {
                hashtable->tables[hvalue] = cur->next;
            }
            else
            {
                prev->next = cur->next;
            }
            /*若节点所指向的数据(包括key和value)为动态分配,则需要在这里释放*/
            hashtable->valuefree(cur);
            HASH_TABLE_FREE(cur);

            hashtable->num --;
            return 0;
        }
        else
        {
            prev = cur;
            cur = cur->next;
        }
    }

    return -2;
}

/**
 * 修改一个节点.注意事项:
 * 1、会先释放节点指向的就数据空间(这里如果是realloc更大的数据空间,容易造成指针泄露,且是不知道整个数据结构的大小的) 
 * 2、修改的节点必须为新动态分配的空间
 *
 * @param hashtable: 散列表
 * @param key: 修改节点关键值
 * @param value: 修改节点数据
 * 
 * @return 0:修改成功
 *        -1:哈希表不存在 或 key为空 或value为空
 *        -2:节点不存在
 */
int hash_table_modify(struct hash_table *hashtable, void *key, void *value)
{
    struct hash_table_node *cur = NULL;
    int hvalue = 0;

    if (hashtable == NULL || key == NULL || value == NULL)
        return -1;

    /*根据key计算出哈希值*/
    hvalue = hashtable->hashfun(hashtable, key);
    cur = hashtable->tables[hvalue];

    /*hash桶中的元素是从小到大排列的,查找要修改的位置*/
    while((cur != NULL) && (hashtable->keycmp(hashtable, key, cur->key) >= 0))
    {
        if (hashtable->keycmp(hashtable, key, cur->key) == 0)
        {
			hashtable->valuefree(cur);
			cur->key = key;
			cur->value = value;
            return 0;
        }
        else
        {
            cur = cur->next;
        }
    }

    return -2;
}

/**
 * 根据key查找节点数据.
 * 
 * @param hashtable: 散列表
 * @param key: 查找节点关键值
 * @param value: 查找节点数据
 * 
 * @return NULL:查找失败
 *        !NULL:查找成功
 */
void * hash_table_search(struct hash_table *hashtable, void *key)
{
    struct hash_table_node *cur = NULL;
    int hvalue = 0;

    if (hashtable == NULL || key == NULL)
        return NULL;

    /*根据key计算出哈希值*/
    hvalue = hashtable->hashfun(hashtable, key);
    cur = hashtable->tables[hvalue];

    /*hash桶中的元素是从小到大排列的,查找要查找的位置*/
    while((cur != NULL) && (hashtable->keycmp(hashtable, key, cur->key) >= 0))
    {
        if (hashtable->keycmp(hashtable, key, cur->key) == 0)
        {
            return cur->value;
        }
        else
        {
            cur = cur->next;
        }
    }

    return NULL;
}


/*******************************************************************************************
 *                                          使用示例
 *******************************************************************************************/
struct test_node
{
    char key[10];
    char value[10];
};

int node_value_free_sample(struct hash_table_node *node)
{
    struct test_node *node_temp = NULL;

    /*根据key在test_node结构体中的偏移地址,找到散列表节点实际指向的结构体首地址*/
	node_temp = container(node->key, struct test_node, key);
    /*如果节点所指向数据空间为动态申请的则需要释放*/
    HASH_TABLE_FREE(node_temp);
	
	return 0;
}

struct hash_table *hash_table_test;
char table_node_read[5][10];

void hash_table_sample(void)
{
    int i = 0;
    struct test_node *node_temp = NULL;
	char rd_key[10] = {10}, del_key[10] = {0};
	char *temp = NULL;
	
    hash_table_test = hash_table_creat_default(5, node_value_free_sample);
	
	/*插入 -- 查询*/
    for (i=0; i<5; i++)
    {
        node_temp = HASH_TABLE_MALLOC(sizeof(*node_temp));
		memset(node_temp, 0, sizeof(*node_temp));
        sprintf(node_temp->key, "AAA%d", i);
		sprintf(node_temp->value, "%d", i+10);
        hash_table_insert(hash_table_test, node_temp->key, node_temp->value);
    }
	for (i=0; i<5; i++)
    {
        sprintf(rd_key, "AAA%d", i);
        temp = hash_table_search(hash_table_test, rd_key);
		memcpy(table_node_read[i], temp, 10);
    }
	
	/*修改 -- 查询*/
    for (i=0; i<5; i++)
    {
        node_temp = HASH_TABLE_MALLOC(sizeof(*node_temp));
		memset(node_temp, 0, sizeof(*node_temp));
        sprintf(node_temp->key, "AAA%d", i);
		sprintf(node_temp->value, "%d", i+20);
        hash_table_modify(hash_table_test, node_temp->key, node_temp->value);
    }
	for (i=0; i<5; i++)
    {
        sprintf(rd_key, "AAA%d", i);
        temp = hash_table_search(hash_table_test, rd_key);
		memcpy(table_node_read[i], temp, 10);
    }
	
	/*删除 -- 查询*/
    for (i=0; i<3; i++)
    {
        sprintf(del_key, "AAA%d", i);
        hash_table_delete(hash_table_test, del_key);
    }
	for (i=0; i<5; i++)
    {
		temp = NULL;
		memset(table_node_read[i], 0, 10);
        sprintf(rd_key, "AAA%d", i);
        temp = hash_table_search(hash_table_test, rd_key);
		if (temp != NULL)
		{
			memcpy(table_node_read[i], temp, 10);
		}
    }
}












