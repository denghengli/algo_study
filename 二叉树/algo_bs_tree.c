/*
 * Copyright (c) 20019-2020, wanweiyingchuang
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-01-07     denghengli   the first version
 */

#include "algo_bs_tree.h"


/**
 * 二叉树key比较, key_cmp:传入的要比较的key, key_becmp:被比较的key
 * 
 * @return > 0 : key_cmp > key_becmp
 * @return = 0 : key_cmp = key_becmp
 * @return < 0 : key_cmp < key_becmp
 * 
 */
static int bstree_keycmp_default(struct bs_tree *bstree, const void *key_cmp, const void *key_becmp)
{
    return strcmp(key_cmp, key_becmp);
}

/**
 * 动态创建一个二叉查找树.
 * 
 * @return NULL:创建失败
 *        !NULL:创建成功
 */
struct bs_tree *bs_tree_creat(bstree_keycmp keycmp, bstree_value_free valuefree)
{
    struct bs_tree *bstree = NULL;

    if (keycmp == NULL)
        return NULL;

    /*申请二叉查找树结构空间*/
    bstree = BS_TREE_MALLOC(sizeof(*bstree));
    if (bstree == NULL)
        return NULL;

    bstree->num       = 0;
    bstree->keycmp    = keycmp;
    bstree->valuefree = valuefree;
    bstree->root      = NULL;
    
    return bstree;
}

/**
 * 使用默认 key比较函数 动态创建一个二叉查找树.
 * 
 * @return NULL:创建失败
 *        !NULL:创建成功
 */
struct bs_tree *bs_tree_creat_default(bstree_value_free valuefree)
{
    return bs_tree_creat(bstree_keycmp_default, valuefree);
}


/**
 * 向一个二叉查找树插入一个节点.支持相同key值的节点插入
 * 
 * @param bstree: 二叉查找树
 * @param key: 关键值
 * @param value: 节点数据
 * 
 * @return 0:插入成功
 *        -1:二叉查找树不存在 或 key为空 或 value为空
 *        -2:节点空间申请失败
 *        -3:插入失败
 */
int bs_tree_insert(struct bs_tree *bstree, void *key, void *value)
{
    struct bs_tree_node *new_node = NULL;
    struct bs_tree_node *f_node = NULL;
    int res = 0;

    if (bstree == NULL || key == NULL || value == NULL)
        return -1;

    /*申请二叉查找树节点空间*/
    new_node = (struct bs_tree_node*)BS_TREE_MALLOC(sizeof(*new_node));
    if (new_node == NULL)
        return -2;
    
    new_node->key = key;
    new_node->value = value;
    new_node->left_child = NULL;
    new_node->right_child = NULL;
    
    /*数为空，插入到根节点*/
    if (BSTREE_IS_EMPTY(bstree))
    {
        bstree->root = new_node;
        bstree->num ++;
        return 0;
    }
    else
    {
        f_node = bstree->root;
        while (f_node != NULL)
        {
            res = bstree->keycmp(bstree, key, f_node->key);
            if (res >= 0) /*去右子树中查找,支持相同key值的节点插入*/
            {
                if (f_node->right_child == NULL)
                {
                    f_node->right_child = new_node;
                    bstree->num ++;
                    return 0;
                }
                f_node = f_node->right_child;
            }
            else if (res < 0)/*去左子树中查找*/
            {
                if (f_node->left_child == NULL)
                {
                    f_node->left_child = new_node;
                    bstree->num ++;
                    return 0;
                }
                f_node = f_node->left_child;
            }
        }
    }

    return -3;
}


/**
 * 删除一个节点.
 * 
 * @param bstree: 二叉查找树
 * @param key: 删除节点关键值
 * 
 * @return 0:删除成功
 *        -1:二叉查找树不存在 或 key为空
 *        -2:节点不存在
 */
int bs_tree_delete(struct bs_tree *bstree, void *key)
{
    struct bs_tree_node *del_node = NULL;//要删除节点
    struct bs_tree_node *f_delnode = NULL;//要删除节点的父节点
    struct bs_tree_node *minnode = NULL;//最小节点
    struct bs_tree_node *f_minnode = NULL;//最小节点的父节点
    struct bs_tree_node *temp = NULL;
    int finsh_flg = 0, res = 0;

    if (bstree == NULL || key == NULL)
        return -1;
    
    /*查找要删除的节点*/
    del_node = bstree->root;
    while ((del_node != NULL) && ((res = bstree->keycmp(bstree, key, del_node->key)) != 0))
    {
        f_delnode = del_node;
        if (res > 0)
        {
            del_node = del_node->right_child;
        }
        else
        {
            del_node = del_node->left_child;
        }
    }

    /*要删除的节点不存在*/
    if (del_node == NULL)
        return -2;

    do
    {           
        if (bstree->keycmp(bstree, key, del_node->key) == 0)
        {
            /*如果删除的节点有两个子节点，则需要找到该节点的右子树中最小的节点，把他替换到要删除的节点上，然后删除这个最小节点
            *
            *1、f_min就为del，min为叶子节点，删除过程为：先将min的数据替换到del，再将f_min的右节点指向min的右节点（NULL），最后删除min
            *2、f_min就为del，min还有右子树（不会再有左子树），删除过程为：先将min的数据替换到del，再将f_min的右节点指向min的右节点（！NULL），最后删除min
            *3、f_min不为del，min为叶子节点，删除过程为：先将min的数据替换到del，再将f_min的左节点指向min的右节点（NULL），最后删除min
            *4、f_min不为del，min还有右子树（不会再有左子树），删除过程为：先将min的数据替换到del，再将f_min的左节点指向min的右节点（！NULL），最后删除min
            */
            if ((del_node->left_child != NULL) && (del_node->right_child != NULL))
            {
                f_minnode = del_node;
                minnode = del_node->right_child;
                while (minnode->left_child != NULL)//查找最小节点
                {
                    f_minnode = minnode;
                    minnode = minnode->left_child;
                }

                /*先删除要删除节点上的数据,数据空间为动态申请的需要在这里先释放*/
                bstree->valuefree(del_node);

                del_node->key = minnode->key;
                del_node->value = minnode->value;
                if (f_minnode == del_node)
                {
                    f_minnode->right_child = minnode->right_child;
                }
                else
                {
                    f_minnode->left_child = minnode->right_child;
                }

                /*交换最小节点和删除节点,因为真正删除的是最小节点,
                删除节点的父节点的子节点还是删除节点*/
                temp = minnode;
                minnode = del_node;
                del_node = temp;
            }
            /*要删除的节点只有左子树*/
            else if (del_node->left_child != NULL)
            {
                /*先删除要删除节点上的数据,数据空间为动态申请的需要在这里先释放*/
                bstree->valuefree(del_node);
                
                minnode = del_node->left_child;

                finsh_flg = 1;
            }
            /*要删除的节点只有右子树*/
            else if (del_node->right_child != NULL)
            {
                /*先删除要删除节点上的数据,数据空间为动态申请的需要在这里先释放*/
                bstree->valuefree(del_node);

                minnode = del_node->right_child;
            }
            /*要删除节点没有子节点*/
            else
            {
                /*先删除要删除节点上的数据,数据空间为动态申请的需要在这里先释放*/
                bstree->valuefree(del_node);

                minnode->left_child = NULL;
                minnode->right_child = NULL;
                minnode = NULL;
            }

            if (del_node == bstree->root)//删除的是头结点
            {
                bstree->root = minnode;
            }
            else
            {
                if (del_node == f_delnode->left_child)//删除节点在删除节点父节点的左子树上
                {
                    f_delnode->left_child = minnode;
                }
                else if (del_node == f_delnode->right_child)//删除节点在删除节点父节点的右子树上
                {
                    f_delnode->right_child = minnode;
                }
            }

            BS_TREE_FREE(del_node);
            del_node = minnode;/*将删除节点指向填补的节点,继续判断删除节点的右子树是否还有相同key值的节点*/

            bstree->num --;    
        }
        else
        {
            del_node = del_node->right_child;/*右子树不为空但key不相同*/
        }
                
	/*1、删除节点只有左子树时则直接退出,因为没有右子树就不会有相同的key节点
	  2、删除节点右子树不为空 或 右子树节点为空但是key值相同 则继续查找相同key值的节点
	*/
    } while ( ((del_node->right_child != NULL) || ((del_node->right_child == NULL) && (bstree->keycmp(bstree, key, del_node->key) == 0))) && !finsh_flg);
      
    return 0;
}

/**
 * 修改一个节点.注意事项:
 * 1、会先释放节点指向的就数据空间(这里如果是realloc更大的数据空间,容易造成指针泄露,且是不知道整个数据结构的大小的) 
 * 2、修改的节点必须为新动态分配的空间
 * 3、如果修改的节点有多个key值相同的节点,只会修改最新查找的一个
 *
 * @param bstree: 二叉查找树
 * @param key: 修改节点关键值
 * @param value: 修改节点数据
 * 
 * @return 0:修改成功
 *        -1:二叉查找树不存在 或 key为空 或value为空
 *        -2:节点不存在
 */
int bs_tree_modify(struct bs_tree *bstree, void *key, void *value)
{
    struct bs_tree_node *mody_node = NULL;
    int res = -2;
    
    if (bstree == NULL || key == NULL || value == NULL)
        return -1;

    /*查找要修改的节点*/
    mody_node = bstree->root;
    while (mody_node != NULL)
    {
        res = bstree->keycmp(bstree, key, mody_node->key);
        if (res > 0)
        {
            mody_node = mody_node->right_child;
        }
        else if (res < 0)
        {
            mody_node = mody_node->left_child;
        }
        else
        {
            bstree->valuefree(mody_node);   
            mody_node->key = key;
            mody_node->value = value;
            res = 0;
            break;
        }
    }

    return res;
}

/**
 * 根据key查找节点数据.
 * 
 * @param bstree: 二叉查找树
 * @param key: 查找节点关键值
 * 
 * @return 0:查找成功
 *        -1:二叉查找树不存在 或 key为空 或value为空
 *        -2:节点不存在
 */
int bs_tree_search(struct bs_tree *bstree, void *key, struct double_list *dlist)
{
    struct bs_tree_node *ser_node = NULL;
    int res = -2;
    
    if (bstree == NULL || key == NULL || BSTREE_IS_EMPTY(bstree))
        return -1;

    /*查找要查找的节点*/
    ser_node = bstree->root;
    while (ser_node != NULL)
    {
        res = bstree->keycmp(bstree, key, ser_node->key);
        if (res > 0)
        {
            ser_node = ser_node->right_child;
        }
        else if (res < 0)
        {
            ser_node = ser_node->left_child;
        }
        else
        {
            double_list_add_node_tail(dlist, ser_node->value);
            ser_node = ser_node->right_child;
            res = 0;
        }
    }

    return res;
}

/**
 * 查找二叉树中的最小节点数据.
 * 
 * @param bstree: 二叉查找树
 * 
 * @return 0:查找成功
 *        -1:二叉查找树不存在 或 key为空 或value为空
 *        -2:节点不存在
 */
void * bs_tree_search_min(struct bs_tree *bstree)
{
    struct bs_tree_node *ser_node = NULL;
    
    if (bstree == NULL || BSTREE_IS_EMPTY(bstree))
        return NULL;

    /*查找要查找的节点*/
    ser_node = bstree->root;
    while ((ser_node != NULL) && (ser_node->left_child != NULL))
    {
        ser_node = ser_node->left_child;
    }

    if (ser_node != NULL)
    {
        return ser_node->value;
    }

    return NULL;
}

/**
 * 查找二叉树中的最大节点数据.
 * 
 * @param bstree: 二叉查找树
 * 
 * @return 0:查找成功
 *        -1:二叉查找树不存在 或 key为空 或value为空
 *        -2:节点不存在
 */
void * bs_tree_search_max(struct bs_tree *bstree)
{
    struct bs_tree_node *ser_node = NULL;
    
    if (bstree == NULL || BSTREE_IS_EMPTY(bstree))
        return NULL;

    /*查找要查找的节点*/
    ser_node = bstree->root;
    while ((ser_node != NULL) && (ser_node->right_child != NULL))
    {
        ser_node = ser_node->right_child;
    }

    if (ser_node != NULL)
    {
        return ser_node->value;
    }

    return NULL;
}

/**
 * 中序遍历二叉树,并将节点数据放入双向链表中
 * 
 * @param bstree: 二叉查找树
 * 
 * @return NULL:查找失败
 *        !NULL:查找成功.返回节点数据
 */
int bs_tree_inorder(struct bs_tree *bstree, struct bs_tree_node *node, struct double_list *dlist)
{    
    if (bstree == NULL || BSTREE_IS_EMPTY(bstree))
        return NULL;

    if (node == NULL)
    {
        return NULL;
    }

    bs_tree_inorder(bstree, node->left_child, dlist);
    double_list_add_node_tail(dlist, node->value);
    bs_tree_inorder(bstree, node->right_child, dlist);

    return dlist->len;
}

///**
// * 清空二叉树节点数据
// * 
// * @param bstree: 二叉查找树
// * 
// * @return 
// */
//void bs_tree_node_empty(struct bs_tree *bstree, struct bs_tree_node *node)
//{
//    if (bstree == NULL || BSTREE_IS_EMPTY(bstree))
//        return;

//    if (node == NULL)
//    {
//        return;
//    }

//    bs_tree_node_empty(bstree, node->left_child);
//    bs_tree_node_empty(bstree, node->right_child);
//    bstree->valuefree(node);
//    BS_TREE_FREE(node);
//	node = NULL;
////	bstree->num--;
//}

///**
// * 销毁一颗二叉查找树
// * 
// * @param bstree: 二叉查找树
// * 
// * @return 
// */
//void bs_tree_destroy(struct bs_tree *bstree)
//{
//    bs_tree_node_empty(bstree, bstree->root);
//    BS_TREE_FREE(bstree);
//}

/**
 * 清空二叉树节点数据
 * 
 * @param bstree: 二叉查找树
 * 
 * @return 
 */
void bs_tree_node_empty(struct bs_tree **bstree, struct bs_tree_node **node)
{
    if (*bstree == NULL || BSTREE_IS_EMPTY((*bstree)))
        return;

    if (*node == NULL)
    {
        return;
    }

    bs_tree_node_empty(bstree, &(*node)->left_child);
    bs_tree_node_empty(bstree, &(*node)->right_child);
    (*bstree)->valuefree(*node);
    (*bstree)->num--;
    BS_TREE_FREE(*node);
	*node = NULL;
}

/**
 * 销毁一颗二叉查找树
 * 
 * @param bstree: 二叉查找树
 * 
 * @return 
 */
void bs_tree_destroy(struct bs_tree **bstree)
{
    bs_tree_node_empty(bstree, &(*bstree)->root);
    BS_TREE_FREE(*bstree);
    *bstree = NULL;
}

/*******************************************************************************************
 *                                          使用示例
 *******************************************************************************************/
struct test_node
{
    char key[10];
    char value[10];
};

static int node_value_free_sample(struct bs_tree_node *node)
{
    struct test_node *node_temp = NULL;

    /*根据key在test_node结构体中的偏移地址,找到二叉查找树节点实际指向的结构体首地址*/
	node_temp = container(node->key, struct test_node, key);
    /*如果节点所指向数据空间为动态申请的则需要释放*/
    BS_TREE_FREE(node_temp);
	/*将二叉树中指向这块内存的节点key 和 value 赋为空*/
    node->key = NULL;
    node->value = NULL;
    
	return 0;
}


struct bs_tree *bs_tree_test = NULL;
char tree_node_read[10][10];
struct double_list *dlist_test = NULL;

void bs_tree_sample(void)
{
    int i = 0, j = 0, key[10] = {0};
    struct test_node *node_temp = NULL;
	char rd_key[10] = {10}, del_key[10] = {0};
    struct double_list_node *dlist_node = NULL;

    bs_tree_test = bs_tree_creat_default(node_value_free_sample);
	dlist_test = double_list_creat();
    
    for (i=0; i<10; i++)
    {
        key[i] = rand() % 10;
    }
    
	/*插入 -- 查询*/
    for (i=0; i<10; i++)
    {
        node_temp = BS_TREE_MALLOC(sizeof(*node_temp));
		memset(node_temp, 0, sizeof(*node_temp));
        sprintf(node_temp->key, "AAA%d", key[i]);
		sprintf(node_temp->value, "%d", key[i]);
        bs_tree_insert(bs_tree_test, node_temp->key, node_temp->value);//支持多个节点插入
    }
	for (i=0; i<10; i++)
    {
        memset(tree_node_read[i], 0, 10);
        memset(rd_key, 0, sizeof(rd_key));
        sprintf(rd_key, "AAA%d", key[i]);
        
        bs_tree_search(bs_tree_test, rd_key, dlist_test);//相同key值的数据会被放入双向链表中
        dlist_node = dlist_test->head;
        for (j=0; j<dlist_test->len; j++)//相同key值的节点数据
        {
            memcpy(tree_node_read[j], dlist_node->value, 10);
            dlist_node = dlist_node->next;
        }
        double_list_node_empty(dlist_test, 0);//清空双向链表节点
    }
    
    /*中序遍历*/
    bs_tree_inorder(bs_tree_test, bs_tree_test->root, dlist_test);
    dlist_node = dlist_test->head;
	for (i=0; i<dlist_test->len; i++)
    {
		memcpy(tree_node_read[i], dlist_node->value, 10);
        dlist_node = dlist_node->next;
    }
    double_list_node_empty(dlist_test, 0);
    
    
    
	/*清空 -- 插入 -- 查询*/
    bs_tree_node_empty(&bs_tree_test, &(bs_tree_test)->root);
    for (i=0; i<10; i++)
    {
        node_temp = BS_TREE_MALLOC(sizeof(*node_temp));
		memset(node_temp, 0, sizeof(*node_temp));
        sprintf(node_temp->key, "AAA%d", key[i]);
		sprintf(node_temp->value, "%d", key[i] + 10);
        bs_tree_insert(bs_tree_test, node_temp->key, node_temp->value);
    }
	for (i=0; i<10; i++)
    {
        memset(tree_node_read[i], 0, 10);
        memset(rd_key, 0, sizeof(rd_key));
        sprintf(rd_key, "AAA%d", key[i]);
        
        bs_tree_search(bs_tree_test, rd_key, dlist_test);//相同key值的数据会被放入双向链表中
        dlist_node = dlist_test->head;
        for (j=0; j<dlist_test->len; j++)//相同key值的节点数据
        {
            memcpy(tree_node_read[j], dlist_node->value, 10);
            dlist_node = dlist_node->next;
        }
        double_list_node_empty(dlist_test, 0);//清空双向链表节点
    }
	/*中序遍历*/
    bs_tree_inorder(bs_tree_test, bs_tree_test->root, dlist_test);
    dlist_node = dlist_test->head;
	for (i=0; i<dlist_test->len; i++)
    {
		memcpy(tree_node_read[i], dlist_node->value, 10);
        dlist_node = dlist_node->next;
    }
    double_list_node_empty(dlist_test, 0);
	
	
    
	/*修改 -- 查询*/
    for (i=0; i<10; i++)
    {
        node_temp = BS_TREE_MALLOC(sizeof(*node_temp));
		memset(node_temp, 0, sizeof(*node_temp));
        sprintf(node_temp->key, "AAA%d", key[i]);
		sprintf(node_temp->value, "%d", key[i] + 20);
        bs_tree_modify(bs_tree_test, node_temp->key, node_temp->value);
    }
	for (i=0; i<10; i++)
    {
        memset(tree_node_read[i], 0, 10);
        memset(rd_key, 0, sizeof(rd_key));
        sprintf(rd_key, "AAA%d", key[i]);
        
        bs_tree_search(bs_tree_test, rd_key, dlist_test);//相同key值的数据会被放入双向链表中
        dlist_node = dlist_test->head;
        for (j=0; j<dlist_test->len; j++)//相同key值的节点数据
        {
            memcpy(tree_node_read[j], dlist_node->value, 10);
            dlist_node = dlist_node->next;
        }
        double_list_node_empty(dlist_test, 0);//清空双向链表节点
    }
	
    /*中序遍历*/
    bs_tree_inorder(bs_tree_test, bs_tree_test->root, dlist_test);
    dlist_node = dlist_test->head;
	for (i=0; i<dlist_test->len; i++)
    {
		memcpy(tree_node_read[i], dlist_node->value, 10);
        dlist_node = dlist_node->next;
    }
    double_list_node_empty(dlist_test, 0);

    
    
    
    
	/*删除 -- 查询*/
    sprintf(del_key, "AAA%d", key[0]);
    bs_tree_delete(bs_tree_test, del_key);
    for (i=0; i<10; i++)
    {
        memset(tree_node_read[i], 0, 10);
    }
    bs_tree_inorder(bs_tree_test, bs_tree_test->root, dlist_test);/*中序遍历*/
    dlist_node = dlist_test->head;
	for (i=0; i<dlist_test->len; i++)
    {
		memcpy(tree_node_read[i], dlist_node->value, 10);
        dlist_node = dlist_node->next;
    }
    double_list_node_empty(dlist_test, 0);
    /*删除 -- 查询*/
    sprintf(del_key, "AAA%d", key[1]);
    bs_tree_delete(bs_tree_test, del_key);
    for (i=0; i<10; i++)
    {
        memset(tree_node_read[i], 0, 10);
    }
    bs_tree_inorder(bs_tree_test, bs_tree_test->root, dlist_test);/*中序遍历*/
    dlist_node = dlist_test->head;
	for (i=0; i<dlist_test->len; i++)
    {
		memcpy(tree_node_read[i], dlist_node->value, 10);
        dlist_node = dlist_node->next;
    }
    double_list_node_empty(dlist_test, 0);
    /*删除 -- 查询*/
    sprintf(del_key, "AAA%d", key[2]);
    bs_tree_delete(bs_tree_test, del_key);
    for (i=0; i<10; i++)
    {
        memset(tree_node_read[i], 0, 10);
    }
    bs_tree_inorder(bs_tree_test, bs_tree_test->root, dlist_test);/*中序遍历*/
    dlist_node = dlist_test->head;
	for (i=0; i<dlist_test->len; i++)
    {
		memcpy(tree_node_read[i], dlist_node->value, 10);
        dlist_node = dlist_node->next;
    }
    double_list_node_empty(dlist_test, 0);
    
    
    /*释放二叉树 和 双向链表结构*/
    double_list_destroy(dlist_test, 0);
    bs_tree_destroy(&bs_tree_test);
}

