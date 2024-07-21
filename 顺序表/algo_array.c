/*
 * Copyright (c) 20019-2020, wanweiyingchuang
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-11-18     denghengli   the first version
 */

#include "algo_array.h"

/**
 * dynamically create a dynamic array, including the array header and data space.
 * 
 * @param size: array size
 * @param tpsz: array data type length
 * @return NULL:malloc fail
 *        !NULL:success
 */
struct dstrct_array* array_creat(unsigned int size, unsigned int tpsz)
{
    struct dstrct_array *array = NULL;

    if (size <= 0 || tpsz <= 0)
        return NULL;
        
	/* malloc an array control struct */
    array = ARRAR_MALLOC(sizeof(*array));
    if (array == NULL)
    {
        return NULL;
    }
	
	/* calloc an array */
	void *p = ARRAR_CALLOC(1, (size * tpsz));
	if (p == NULL)
		return NULL;
	
    array->used = 0;
	array->size = size;
	array->tpsz = tpsz;
	array->p = p;
    
    return array;
}

/**
 * init a dynamic array, malloc a data space.
 * 
 * @param array: 
 * @param size: array size
 * @param tpsz: array data type length
 * @return -1:array is null or malloc fail
 *          0:success
 */
int array_init(struct dstrct_array *array, unsigned int size, unsigned int tpsz)
{
	if (array == NULL)
		return -1;
	
	/* calloc an array */
	void *p = ARRAR_CALLOC(1, (size * tpsz));
	if (p == NULL)
		return -1;
	
    array->used = 0;
	array->size = size;
	array->tpsz = tpsz;
	array->p = p;
    
    return 0;
}


/**
 * insert a value to an array.
 * @note The array cannot be inserted after it is full, 
 *       and can only be inserted when the used is less than the size
 *       The purpose is to leave at least one space for carrying
 * @param array: 
 * @param pos: 1 - size
 * @param value: data
 * @return -1:array is null
 *         -2:pos overstep the boundary of array
 *         -3:used is greater than size
 *          0:success
 */
int array_insert(struct dstrct_array *array, unsigned int pos, void *const value)
{
	char *pbegin = NULL;
	void *pnew = NULL, *pold = NULL, *pinsert = NULL;
	unsigned int i = 0;
	
	if (array == NULL)
		return -1;
	
	/* adjust the pos did cross the array*/
	if (pos > array->size || pos <= 0)
		return -2;
	
	/* adjust used is not greater than size */
	if (array->used >= array->size)
		return -3;
	
	/* move the array data back one cell */
	pbegin = array->p;
	for (i=array->used; i>=pos; i--)
	{
		pnew = pbegin + i * array->tpsz;
		pold = pbegin + (i - 1) * array->tpsz;
		
		memcpy(pnew, pold, array->tpsz);
	}
	
	/* insert the data*/
	pinsert = pbegin + (pos - 1) * array->tpsz;
	memcpy(pinsert, value, array->tpsz);
	array->used++;
	
	return 0;
}

/**
 * delete the array data by value.
 *
 * @param array: delete array
 * @param value: delete value
 * @return -1:array is null
 *         -2:value is not matched
 *          0:success
 */
int array_del_value(struct dstrct_array *array, void *value)
{
	char *pbegin = NULL;
	void *pnew = NULL, *pold = NULL;
	int i = 0, res = 0;
	int check_flag = 1, check_succ = 0;
	
	if (array == NULL)
		return -1;
	
	pbegin = array->p;
	for (i=0; i<array->used; i++)
	{
		/* Check the match value */
		if (check_flag)
		{
			res = memcmp(pbegin + i * array->tpsz, value, array->tpsz);
			if (res == 0)
			{
				check_flag = 0;
				check_succ = 1;
			}
		}
		/* delete the check value and move one cell forward*/
		else
		{
			pnew = pbegin + (i - 1) * array->tpsz;
			pold = pbegin + i * array->tpsz;
			memcpy(pnew, pold, array->tpsz);
		}
	}
	/* match and deleat success*/
	if (check_succ)
	{
		array->used--;
		return 0;
	}
	
	return -2;
}

/**
 * delete the array data by pos.
 *
 * @param array: delete array
 * @param pos: the array data pos
 * @return -1:array is null
 *         -2:pos overstep the boundary of array
 *          0:success
 */
int array_del_pos(struct dstrct_array *array, unsigned int pos)
{
	char *pbegin = NULL;
	void *pnew = NULL, *pold = NULL;
	int i = 0;
	
	if (array == NULL)
		return -1;
	
	if (pos <= 0 || pos > array->used)
		return -2;
	
	pbegin = array->p;
	for (i=pos; i<=array->used; i++)
	{
		pnew = pbegin + (i - 1) * array->tpsz;
		pold = pbegin + i * array->tpsz;
		memcpy(pnew, pold, array->tpsz);
	}
	
	array->used--;
	
	return 0;
}

/**
 * modify a value.
 *
 * @param array: 
 * @param pos: 1 - size
 * @param value: data
 * @return -1:array is null
 *         -2:pos overstep the boundary of array
 *          0:success
 */
int array_modify(struct dstrct_array *array, unsigned int pos, void *const value)
{
	char *pbegin = NULL;
    void *pmodify = NULL;
	
	if (array == NULL)
		return -1;
	
	/* adjust the pos did cross the array*/
	if (pos > array->size || pos <= 0)
		return -2;
	
	pbegin = array->p;
	pmodify = pbegin + (pos - 1) * array->tpsz;
	
	memcpy(pmodify, value, array->tpsz);
	
	return 0;
}

/**
 * search the value pos.
 *
 * @param array: 
 * @param value: data
 * @return -1:array is null
 *         -2:pos overstep the boundary of array
 *          1 - size: the value pos
 */
int array_search_pos(struct dstrct_array *array, void* const value)
{
	char *pbegin = NULL;
    void *psearch = NULL;
	unsigned int i = 0, pos = 0;
	
	if (array == NULL)
		return -1;
	
	/* adjust the pos did cross the array*/
	if (pos > array->size || pos <= 0)
		return -2;
	
	pbegin = array->p;
	for (i=0; i<array->used; i++)
	{
		psearch = pbegin + i * array->tpsz;
		if (!memcmp(psearch, value, array->tpsz))
		{
			pos = i + 1;
			break;
		}
	}
	
	return pos;
}


/**
 * delete the array space.
 *
 * @return -1:array is null
 *          0:success
 */
int array_empty(struct dstrct_array *array)
{
	if (array ==NULL)
		return -1;

	ARRAY_FREE(array->p);
	array->p = NULL;
	array->size = 0;
	array->tpsz = 0;
	array->used = 0;
	
	return 0;
}

/**
 * delete the array space and array control struct.
 * @note The array space to be empty before destroy array
 * @param array: delete array
 * @return -1:array is null
 *          0:success
 */
int array_destroy(struct dstrct_array *array)
{
	if (array_empty(array) != 0)
		return -1;
	
	ARRAY_FREE(array);
	array = NULL;
	
	return 0;
}


/**
 * get the array used.
 *
 * @return used: > 0
 */
unsigned int array_used(struct dstrct_array *array)
{
	if (array == NULL)
		return 0;
	
	return array->used;
}

/**
 * get the array size.
 *
 * @return size: > 0
 */
unsigned int array_size(struct dstrct_array *array)
{
	if (array == NULL)
		return 0;
	
	return array->size;
}




struct array_struct_test
{
    int a;
    int b;
}array_struct_test[10];

int array_test[10] = {0};
void algo_array_test(void)
{
    struct dstrct_array *array_int = NULL;
    struct dstrct_array *array_str = NULL;
    unsigned int i = 0;
    unsigned int insert_value = 0x55, del_value = 0x55, mdy_value = 0x66;
    struct array_struct_test struct_test;
    
    /* int type test*/
    array_int = array_creat(10, sizeof(int));// int p[10]
    for (i=0;i<5;i++)
    {
        array_insert(array_int, i+1, (void*)&i);        
    }
    for (i=0;i<5;i++)
    {
        array_insert(array_int, 1, (void*)&insert_value);
    }
    for (i=0;i<array_int->used;i++)
    {
        array_test[i] = ((int*)array_int->p)[i];//1、((int*)array->p) 2、p[i]
    }
    
    array_del_value(array_int, (void*)&del_value);
    array_del_pos(array_int,2);
    array_del_pos(array_int,5);
    array_del_pos(array_int,6);
    array_modify(array_int, 5, (void*)&mdy_value);
    memset(array_test, 0, sizeof(array_test));
    for (i=0;i<array_int->used;i++)
    {
        array_test[i] = ((int*)array_int->p)[i];//1、((int*)array->p) 2、p[i]
    }
    
    
    /* srtruct type test*/
    array_str = array_creat(10, sizeof(struct array_struct_test)); // struct array_struct_test p[10]);
    for (i=0;i<5;i++)
    {
        struct_test.a = i;
        struct_test.b = i + 1;
        array_insert(array_str, i+1, (void*)&struct_test);        
    }
    for (i=0;i<5;i++)
    {
        array_insert(array_str, 1, (void*)&struct_test);
    }
    for (i=0;i<array_str->used;i++)
    {
        array_struct_test[i] = ((struct array_struct_test*)array_str->p)[i];//1、((struct array_struct_test*)array->p) 2、p[i]
    }
    
}

















