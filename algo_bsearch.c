/*
 * Copyright (c) 20019-2020, wanweiyingchuang
 *
 *二分查找算法的变形问题
 *	1、查找第一个等于给定数值的元素
 *	2、查找最后一个等于给定数值的元素
 *	3、查找第一个大于等于给定数值的元素
 *	4、查找第一个小于等于给定数值的元素
 * 
 * Change Logs:
 * Date           Author       Notes
 * 2019-12-23    denghengli   the first version
 */
 

/**
 * binary search.
 * 
 * @param array: search array
 * @param size: array size
 * @param search_value: search value
 * @return 1
 */
int binary_search(int array[], int size, int search_value)
{
	int low = 0;
	int high = size - 1;
	int mid = 0;
	
	/* 注意结束条件,不是low<high */
	while(low <= high)
	{
		/* 注意size太大导致溢出,所以不为(high+low)/2 */
		mid = low + (high - low) / 2; 
		if (array[mid] == search_value)
		{
			return mid;
		}
		else if (array[mid] < search_value)
		{
			low = mid + 1;
		}
		else
		{
			high = mid - 1;
		}
	}
	
	return -1;
}

/**
 * 查找第一个等于给定数值的元素.
 * 
 * @param array: search array
 * @param size: array size
 * @param search_value: search value
 * @return 1
 */
int binary_search_variant1(int array[], int size, int search_value)
{
	int low = 0;
	int high = size - 1;
	int mid = 0;
	
	/* 注意结束条件,不是low<high */
	while(low <= high)
	{
		/* 注意size太大导致溢出,所以不为(high+low)/2 */
		mid = low + (high - low) / 2; 

		if (array[mid] < search_value)
		{
			low = mid + 1;
		}
		else if (array[mid] > search_value)
		{
			high = mid - 1;
		}
		else
		{
			if (mid == 0 || (array[mid - 1] != search_value))
			{
				return mid;
			}
			else
			{
				high = mid - 1;
			}
		}
	}
	
	return -1;
}


/**
 * 查找最后一个等于给定数值的元素.
 * 
 * @param array: search array
 * @param size: array size
 * @param search_value: search value
 * @return 1
 */
int binary_search_variant2(int array[], int size, int search_value)
{
	int low = 0;
	int high = size - 1;
	int mid = 0;
	
	/* 注意结束条件,不是low<high */
	while(low <= high)
	{
		/* 注意size太大导致溢出,所以不为(high+low)/2 */
		mid = low + (high - low) / 2; 

		if (array[mid] < search_value)
		{
			low = mid + 1;
		}
		else if (array[mid] > search_value)
		{
			high = mid - 1;
		}
		else
		{
			if (mid == (size - 1) || (array[mid + 1] != search_value))
			{
				return mid;
			}
			else
			{
				low = mid + 1;
			}
		}
	}
	
	return -1;
}


/**
 * 查找第一个大于等于给定数值的元素.
 * 
 * @param array: search array
 * @param size: array size
 * @param search_value: search value
 * @return 1
 */
int binary_search_variant3(int array[], int size, int search_value)
{
	int low = 0;
	int high = size - 1;
	int mid = 0;
	
	/* 注意结束条件,不是low<high */
	while(low <= high)
	{
		/* 注意size太大导致溢出,所以不为(high+low)/2 */
		mid = low + (high - low) / 2; 

		if (search_value > array[mid])
		{
			low = mid + 1;
		}
		else
		{
			if (mid == 0 || (array[mid - 1] < search_value))
			{
				return mid;
			}
			else
			{
				high = mid - 1;
			}
		}
	}
	
	return -1;
}


/**
 * 查找最后一个小于等于给定数值的元素.
 * 
 * @param array: search array
 * @param size: array size
 * @param search_value: search value
 * @return 1
 */
int binary_search_variant4(int array[], int size, int search_value)
{
	int low = 0;
	int high = size - 1;
	int mid = 0;
	
	/* 注意结束条件,不是low<high */
	while(low <= high)
	{
		/* 注意size太大导致溢出,所以不为(high+low)/2 */
		mid = low + (high - low) / 2; 

		/*往前找,落在前区*/
		if (search_value < array[mid])
		{
			high = mid - 1;
		}
		else
		{
			/*array[mid]<=search_value, 如果mid后面一个元素大于search_value*/
			if (mid == (size - 1) || (array[mid + 1] > search_value))
			{
				return mid;
			}
			else
			{
				low = mid + 1;
			}
		}
	}
	
	return -1;
}

