/*
 * Copyright (c) 20019-2020, wanweiyingchuang
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-01-14     denghengli   the first version
 */

#include "algo_heap.h"


/**
 * 堆key比较, key_cmp:传入的要比较的key, key_becmp:被比较的key
 * 
 * @return > 0 : key_cmp > key_becmp
 * @return = 0 : key_cmp = key_becmp
 * @return < 0 : key_cmp < key_becmp
 * 
 */
static int heap_keycmp_default(struct heap *heap, const void *key_cmp, const void *key_becmp)
{
    return strcmp(key_cmp, key_becmp);
}

/**
 * 交换堆两个节点数据.
 * 
 * @param array: 堆
 * @param child: 节点下标
 * @param father: 节点父节点下标
 */
static void heap_swap(struct heap_node *array, int child, int father)
{
    struct heap_node temp;

    temp = array[child];
    array[child] = array[father];
    array[father] = temp;
}

/**
 * 大顶堆从上往下进行堆化
 * 
 * @param array: 堆
 */
static void heapify(struct heap *heap, int start_pos)
{
    int max_pos = start_pos;//根节点和2个子节点3个节点中最大的那个节点下标,从堆顶开始

    while (1)
    {
        /*子节点没有超过堆范围 & 子节点比根节点大，则需要交换根节点和子节点位置并继续向下堆化*/
        if ((start_pos * 2 + 1 < heap->num) && (heap->keycmp(heap, heap->array[start_pos].key,heap->array[start_pos * 2 + 1].key) < 0))  
        {
            max_pos = start_pos * 2 + 1;
        }
        if ((start_pos * 2 + 2 < heap->num) && (heap->keycmp(heap, heap->array[max_pos].key,heap->array[start_pos * 2 + 2].key) < 0))  
        {
            max_pos = start_pos * 2 + 2;
        }

        if (max_pos == start_pos) break;//最大节点下标没改变，说明跟节点就是最大的节点了，停止向下堆化
        heap_swap(heap->array, max_pos, start_pos);
        start_pos = max_pos;//继续向下堆化
    }
}

/**
 * 动态创建一个堆.
 * 
 * @return NULL:创建失败
 *        !NULL:创建成功
 */
struct heap *heap_creat(heap_keycmp keycmp, heap_value_free valuefree)
{
    struct heap *heap = NULL;
    int i = 0;
    
    if (keycmp == NULL)
        return NULL;

    /*申请堆结构空间，数组下标从0 - n-1*/
    heap = HEAP_MALLOC(sizeof(*heap));
    if (heap == NULL)
        return NULL;

    heap->keycmp = keycmp;
    heap->valuefree = valuefree;
    heap->num = 0;
    for (i=0; i<HEAP_MAX_SIZE; i++)
    {
        heap->array[i].key = NULL;
        heap->array[i].value = NULL;
    }
    
    return heap;
}

/**
 * 使用默认 key比较函数 动态创建一个堆.
 * 
 * @return NULL:创建失败
 *        !NULL:创建成功
 */
struct heap *heap_creat_default(heap_value_free valuefree)
{
    return heap_creat(heap_keycmp_default, valuefree);
}

/**
 * 向堆中插入一个节点.从下往上堆化
 * 
 * @param heap: 堆
 * @param key: 关键值
 * @param value: 节点数据
 * 
 * @return 0:插入成功
 *        -1:堆不存在 或 key为空 或 value为空
 *        -3:插入失败
 */
int heap_insert(struct heap *heap, void *key, void *value)
{
    int n = 0;

    if (heap == NULL || key == NULL || value == NULL)
        return -1;
    
    /*堆满了*/
    if (heap->num >= HEAP_MAX_SIZE)
        return -3;

    heap->array[heap->num].key = key;//堆下标从0开始
    heap->array[heap->num].value = value;
    heap->num += 1;

    /*1、从下往上进行堆化，(n/2 + 1) - n为叶子节点不需要堆化
     *2、n/2节点为n节点的父节点，n节点为n/2节点的2个子节点 
     *3、子节点大于0，子节点比根节点值大，则继续向上堆化
    */
   n = heap->num - 1;
   while ( (n > 0) && (heap->keycmp(heap, heap->array[n].key, heap->array[(n - 1) / 2].key) > 0) )
   {
       heap_swap(heap->array, n, (n-1)/2);//交换节点与根节点数据
       n = n / 2;//将下标指向根节点，继续向上堆化
   }
    return 0;
}

/**
 * 大顶堆删除最大节点,从上往下堆化
 * 
 * @param heap: 堆
 * 
 * @return 0:删除成功
 *        -1:堆不存在
 *        -3:删除失败
 */
int heap_delete_max(struct heap *heap)
{
    if (heap == NULL)
        return -1;
    
    /*堆中没有数据*/
    if (heap->num < 1)
        return -3;

    heap->valuefree(&heap->array[0]);
    heap->array[0] = heap->array[heap->num - 1];
    heap->array[heap->num - 1].key = NULL;
    heap->array[heap->num - 1].value = NULL;
    heap->num --;
    heapify(heap, 0);

    return 0;
}

/**
 * 对已有数据的堆进行堆化
 * 
 * @param heap: 堆
 * 
 * @return 0:建堆成功
 *        -1:堆不存在
 *        -3:堆中没有数据
 */
int heap_build(struct heap *heap)
{
    int i = 0;

    if (heap == NULL)
        return -1;
    
    /*堆中没有数据*/
    if (heap->num < 1)
        return -3;

    for (i=heap->num/2 - 1; i >= 0; i--)
    {
        heapify(heap, i);
    }

    return 0;
}

/**
 * 对已有数据的堆，对堆得数据进行从小到大排序
 * 
 * @param heap: 堆
 * 
 * @return 0:排序成功
 *        -1:堆不存在
 *        -3:堆中没有数据
 */
int heap_sort(struct heap *heap)
{
    int num = heap->num;

    if (heap == NULL)
        return -1;
    
    /*堆中没有数据*/
    if (heap->num < 1)
        return -3;

    /*将堆顶数据放至数组最后，然后从堆顶开始堆化*/
    while (heap->num > 1)
    {
        heap->num--;
        heap_swap(heap->array, heap->num, 0);
        heapify(heap,0);
    }

    heap->num = num;

    return 0;
}

/**
 * 清空堆中所有节点数据
 * 
 * @param heap: 堆
 * 
 * @return NULL
 */
void heap_empty(struct heap *heap)
{
    if (heap == NULL || heap->num < 1)
        return;

    /*将堆顶数据放至数组最后，然后从堆顶开始堆化*/
    for (; heap->num > 0; heap->num--)
    {
        heap->valuefree(&heap->array[heap->num - 1]);
        heap->array[heap->num - 1].key = NULL;
        heap->array[heap->num - 1].value = NULL;
    }
}

/**
 * 销毁堆
 * 
 * @param heap: 堆
 * 
 * @return NULL
 */
void heap_destroy(struct heap **heap)
{
    heap_empty(*heap);
    HEAP_FREE(*heap);
    *heap = NULL;
}


/*******************************************************************************************
 *                                          使用示例
 *******************************************************************************************/
struct test_node
{
    char key[10];
    char value[10];
};

static int node_value_free_sample(struct heap_node *node)
{
    struct test_node *node_temp = NULL;

    /*根据key在test_node结构体中的偏移地址,找到二叉查找树节点实际指向的结构体首地址*/
	node_temp = container(node->key, struct test_node, key);
    /*如果节点所指向数据空间为动态申请的则需要释放*/
    HEAP_FREE(node_temp);
	/*将二叉树中指向这块内存的节点key 和 value 赋为空*/
    node->key = NULL;
    node->value = NULL;
    
	return 0;
}


struct heap *heap_test = NULL;
char heap_node_read[10][10];

void heap_sample(void)
{
    int i = 0, key[10] = {0};
    struct test_node *node_temp = NULL;

    heap_test = heap_creat_default(node_value_free_sample);
    
    for (i=0; i<10; i++)
    {
        key[i] = rand() % 10;
    }
    
    
    
	/*插入 -- 查询*/
    for (i=0; i<10; i++)
    {
        node_temp = HEAP_MALLOC(sizeof(*node_temp));
		memset(node_temp, 0, sizeof(*node_temp));
        sprintf(node_temp->key, "AAA%d", key[i]);
		sprintf(node_temp->value, "%d", key[i]);
        heap_insert(heap_test, node_temp->key, node_temp->value);
    }
	for (i=0; i<10; i++)
    {
        memset(heap_node_read[i], 0, 10);
        memcpy(heap_node_read[i], heap_test->array[i].value, 10);
    }
    
    
    
    
    /*排序 -- 查询*/
    heap_sort(heap_test);
    for (i=0; i<10; i++)
    {
        memset(heap_node_read[i], 0, 10);
        memcpy(heap_node_read[i], heap_test->array[i].value, 10);
    }
    
    
    
    
    /*建堆 -- 查询*/
    heap_build(heap_test);
    for (i=0; i<10; i++)
    {
        memset(heap_node_read[i], 0, 10);
        memcpy(heap_node_read[i], heap_test->array[i].value, 10);
    }
    /*排序 -- 查询*/
    heap_sort(heap_test);
    for (i=0; i<10; i++)
    {
        memset(heap_node_read[i], 0, 10);
        memcpy(heap_node_read[i], heap_test->array[i].value, 10);
    }
    
    
    
    /*清空 -- 插入 -- 查询*/
    heap_empty(heap_test);
    for (i=0; i<10; i++)
    {
        node_temp = HEAP_MALLOC(sizeof(*node_temp));
		memset(node_temp, 0, sizeof(*node_temp));
        sprintf(node_temp->key, "AAA%d", key[i]);
		sprintf(node_temp->value, "%d", key[i] + 10);
        heap_insert(heap_test, node_temp->key, node_temp->value);
    }
	for (i=0; i<10; i++)
    {
        memset(heap_node_read[i], 0, 10);
        memcpy(heap_node_read[i], heap_test->array[i].value, 10);
    }
    
    
    
    /*删除最大 -- 查询*/
    heap_delete_max(heap_test);
    for (i=0; i<10; i++)
    {
        memset(heap_node_read[i], 0, 10);
        memcpy(heap_node_read[i], heap_test->array[i].value, 10);
    }
    
    heap_destroy(&heap_test);
}

