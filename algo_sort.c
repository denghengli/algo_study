/*
 * Copyright (c) 20019-2020, wanweiyingchuang
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-12-1    denghengli   the first version
 */
#include "algo_sort.h"

/**
 * dynamically create a dynamic array, including the array header and data space.
 * 
 * @param size: array size
 * @return NULL:malloc fail
 *        !NULL:success
 */
struct sort_array* sort_array_creat(unsigned int size)
{
    struct sort_array *array = NULL;

    if (size <= 0)
        return NULL;
        
	/* malloc an array control struct */
    array = ARRAY_SORT_MALLOC(sizeof(*array));
    if (array == NULL)
    {
        return NULL;
    }
	
	/* calloc an array */
	int *p = ARRAY_SORT_CALLOC(1, size * sizeof(int));
	if (p == NULL)
		return NULL;
	
    array->num = 0;
	array->size = size;
	array->p = p;
    
    return array;
}

/**
 * init a dynamic array, malloc a data space.
 * 
 * @param array: 
 * @param size: array size
 * @return -1:array is null or malloc fail
 *          0:success
 */
int sort_array_init(struct sort_array *array, unsigned int size)
{
    if (array ==NULL)
        return -1;

    /* calloc an array */
	int *p = ARRAY_SORT_CALLOC(1, size * sizeof(int));
	if (p == NULL)
		return NULL;
	
    array->num = 0;
	array->size = size;
	array->p = p;

    return 0;
}

/**
 * bubble sort.
 * 
 * @param array: 
 * @return -1:array is null or num <= 1
 *          0:success
 */
int bubble_sort(struct sort_array *array)
{
    int i,j,temp=0,swap_flag=0;

    if (array == NULL || array->num <= 1)
        return -1;

    for (i=0; i<array->num; i++)
    {
        swap_flag = 0;
        for (j=0; j<array->num-i-1; j++)
        {
            if (array->p[j] > array->p[j+1])
            {
                temp = array->p[j];
                array->p[j] = array->p[j+1];
                array->p[j+1] = temp;
                swap_flag = 1;
            }
        }
        if (!swap_flag)
            break;
    }

    return 0;
}

/**
 * insertion sort.
 * 
 * @param array: 
 * @return -1:array is null or num <= 1
 *          0:success
 */
int insertion_sort(struct sort_array *array)
{
    int i,j,temp=0;

    if (array == NULL || array->num <= 1)
        return -1;

    for (i=1; i<array->num; i++)
    {
        temp = array->p[i];
        for (j=i-1; j>=0; j--)
        {
            if (array->p[j] > temp)
            {
                array->p[j+1] = array->p[j]; /* move data */
            }
            else
            {
                break;
            }
        }
        array->p[j+1] = temp; /* inseert data */
    }
	
	return 0;
}

/**
 * selection sort.
 * 
 * @param array: 
 * @return -1:array is null or num <= 1
 *          0:success
 */
int selection_sort(struct sort_array *array)
{
    int i,j,temp=0,idex=0;

    if (array == NULL || array->num <= 1)
        return -1;

    for (i=0; i<array->num-1; i++)
    {
        idex = i;
        for (j=i+1; j<array->num; j++)
        {
            if (array->p[j] < array->p[idex])
            {
                idex = j;
            }
        }

        if (idex == i)
            continue;
        
        temp = array->p[i];
        array->p[i] = array->p[idex];
        array->p[idex] = temp;
    }
	
	return 0;
}

/**
 * quick sort.
 * 
 * @param array: 
 * @return -1:array is null or num <= 1
 *          0:success
 */
int quick_sort(int *array, int l, int r)
{
	int i = l, j = r, x = array[l];//区间内的第一个坑
    //分治
    if(l < r)
    {
		int i = l, j = r, x = array[l];//区间内的第一个坑
		while(i < j)
		{
			while (i < j && array[j] >= x) j--;//从右向左找小于x的数填array[i]
			if (i < j) array[i++] = array[j];

			while (i < j && array[i] < x) i++;//从左往右找大于x的数填array[j]
			if (i < j) array[j--] = array[i];
		}
		
		array[i] = x;//i等于j的时候退出，将选取的基准填到最后一个坑上
		quick_sort(array, l, i-1);//递归调用
		quick_sort(array, i+1, r);
    }
	
	return 0;
}

void sort_test(void)
{
	int i;
	struct sort_array bubble;
	struct sort_array insertion;
	struct sort_array selection;
	struct sort_array quick;
	int test1[20]={0},test2[20]={0},test3[20]={0},test4[20]={0};
	
	sort_array_init(&bubble, 20);
	sort_array_init(&insertion, 20);
	sort_array_init(&selection, 20);
	sort_array_init(&quick, 20);
	bubble.num = 20;
	insertion.num = 20;
	selection.num = 20;
	quick.num = 20;
	
	for (i = 0; i < 20; i++)
	{
		bubble.p[i] = rand() % 100;
		insertion.p[i] = rand() % 100;
		selection.p[i] = rand() % 100;
		quick.p[i] = rand() % 100;
	}
	
	bubble_sort(&bubble);
	insertion_sort(&insertion);
	selection_sort(&selection);
	quick_sort(quick.p, 0, 19);
	
	for (i = 0; i < 20; i++)
	{
		test1[i] = bubble.p[i];
		test2[i] = insertion.p[i];
		test3[i] = selection.p[i];
		test4[i] = quick.p[i];
	}
}


