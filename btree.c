//
// Created by liangchen on 2022/1/7.
//

#include "btree.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>

/*
 * 为新结点分配空间
 */
BTreeNode
allocate_node()
{
    BTreeNode node = (BTreeNode) malloc (BTREE_NODE_SIZE);

    return node;
}

/*
 * 生成一棵空树
 * 关键字个数为0，且为叶子结点
 */
void
btree_create(BTree tree)
{
    BTreeNode r_node = allocate_node();

    (r_node)->n = 0;
    (r_node)->leaf = 1;

    (tree)->root = r_node;
}

/*
 * 在以node为根结点的树中，寻找关键字位置
 * 返回关键字所在结点，并将关键字位置保存在location
 */
void
btree_search(BTreeNode node, int key)
{

    int j = 0;

    /*
     * 遍历当前结点，寻找恰当的关键字，如果找到相等的关键字，返回结点并将关键字位置保存在location
     * 如果没找到相等结点，且该结点为叶子结点，则报错
     * 否则递归寻找
     */
    while(j < node->n && key > node->key[j])
        j++;
    if(j < node->n && key == node->key[j])
    {
        printf("the %d key's location is %d in the node %p\n", key, j, node);
    }
    else if(node->leaf)
    {
        printf("error:there is no a key\n");
    }
    else  btree_search(node->child[j], key);
}

/*
 * 分裂父结点node中位置为location的子结点的满结点
 */
void
btree_split_child(BTreeNode node, int location)
{
    /* 建立新的空结点 */
    BTreeNode newnode = allocate_node();
    BTreeNode childnode = node->child[location];

    int i = 0;

    /* 初始化空结点newnode，将子结点childnode的信息复制到新结点node中 */
    newnode->leaf = childnode->leaf;
    newnode->n = MIN_T - 1;

    /* 将子结点childnode后T-1个关键字复制到新结点中，并改变子结点的n值 */
    for(i = 0;i <= MIN_T - 2;i++)
        newnode->key[i] = childnode->key[i + MIN_T];

    childnode->n = MIN_T - 1;

    /* 如果子结点非叶子结点，则相应的将子结点的结点点复制到新结点中 */
    if(!childnode->leaf)
        for(i = 0;i <= MIN_T - 1;i++)
            newnode->child[i] = childnode->child[i + MIN_T];

    /* 将父结点对应的关键字以及子结点位置向后移动一位 */
    for(i = node->n;i > location;i--)
    {
        node->key[i] = node->key[i - 1];
        node->child[i+1] = node->child[i];
    }

    /* 为父结点增加新的关键字和子结点，并修改n值 */
    node->child[location + 1] = newnode;
    node->key[location] = childnode->key[MIN_T - 1];
    node->n = node->n + 1;

}

/*
 * 对非满结点进行插入关键字
 */
void
btree_insert_nonfull(BTreeNode node, int key)
{
    int i = node->n - 1;

    if(node->leaf)
    {
        /* 该结点为叶子结点时，找到对应位置，将关键字插入，并对结点node做出修改 */
        while(i >=0 && key < node->key[i])
        {
            node->key[i+1] = node->key[i];
            i--;
        }

        node->key[i+1] = key;
        node->n = node->n + 1;
    }
    else
    {
        /* 非叶子结点时，查找对应子结点，判断其是否为满结点，是，则分裂，否递归插入 */
        while(i >=0 && key < node->key[i])
            i--;
        i++;
        if(node->child[i]->n == MAX_T - 1)
        {
            btree_split_child(node, i);
            if(key > node->key[i])
                i++;
        }

        btree_insert_nonfull(node->child[i], key);
    }
}

/*
 * 对整棵树进行插入关键字
 * 当树为有且只有一个关键字，且已满时，需要建立新的结点作为树的根结点，
 * 而当原树的根结点作为新结点的子结点，进行分裂操作
 * 否则，直接进行非满结点插入操作
 */
void
btree_insert(BTree tree, int key)
{
    BTreeNode r_node = tree->root;

    if(r_node->n == MAX_T - 1)
    {
        BTreeNode r_node_new = allocate_node();

        r_node_new->leaf = 0;
        r_node_new->n = 0;
        r_node_new->child[0] = r_node;
        tree->root = r_node_new;
        btree_split_child(r_node_new, 0);
        btree_insert_nonfull(r_node_new, key);
    }
    else btree_insert_nonfull(r_node, key);
}

/*
 * 为了验证插入以及删除结果正确，添加输出函数
 * 输出以parent为父结点的子树的所有关键字
 * 这里将所有的同一层的结点放入到一个数组中，方便输出
 * 第一个参数node_first作为每一层结点数组的起始地址
 * n为该层结点数
 */
void
display_node(BTreeNode *node_first, int n)
{
    int i = 0, j = 0, k = 0,all = 0;
    BTreeNode *node = node_first;

    /* 将该层的结点所有的关键字输出，不同结点以“  ”为分隔，每层以“$$”为分隔	*/
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < (*(node + i))->n; j++)
        {
            printf("%d ", (*(node + i))->key[j]);
        }
        all = all + (*(node + i))->n + 1;
        //printf(" %p ", *(node + i));
        printf("  ");
    }
    printf("$$\n");

    if(!(*node)->leaf)
    {
        BTreeNode nodes[all];
        i = 0;
        for(j = 0; j < n; j++)
        {
            for(k = 0; k <= (*(node + j))->n; k++)
            {
                nodes[i] = (*(node + j))->child[k];
                i++;
            }
        }
        display_node(nodes, all);
    }
}


/*
 * 为了验证插入和删除操作的正确性，添加输出函数
 * 将整棵树输出
 */
void
display_btree(BTree tree)
{
    BTreeNode r_node = tree->root;

    display_node(&r_node, 1);
}

/*
 * 返回以node为根结点树的最小关键字的结点，关键字的位置肯定为0
 */
BTreeNode
btree_minimum(BTreeNode node)
{
    BTreeNode newnode = node;

    if(newnode->n < 1)
    {
        printf("this is null tree\n");
        return NULL;
    }

    if(node->leaf)
        return newnode;
    else
        newnode = btree_minimum(node->child[0]);

    return newnode;
}


/*
 * 返回以node为根结点树的最大关键字的结点，关键字的位置肯定为该结点的n-1值
 */
BTreeNode
btree_maximum(BTreeNode node)
{
    BTreeNode newnode = node;

    if(newnode->n < 1)
    {
        printf("this is null tree\n");
        return NULL;
    }

    if(node->leaf)
        return newnode;
    else
        newnode = btree_maximum(node->child[node->n]);

    return newnode;
}

/*
 * 输出整棵树的最小关键字
 */
void
btree_min(BTree tree)
{
    BTreeNode r_node = tree->root;
    BTreeNode n_node = btree_minimum(r_node);

    printf("the min is %d\n", n_node->key[0]);
}

/*
 * 输出整棵树的最大关键字
 */
void
btree_max(BTree tree)
{
    BTreeNode r_node = tree->root;
    BTreeNode n_node = btree_maximum(r_node);

    printf("the max is %d\n", n_node->key[n_node->n - 1]);
}

/*
 * 当下降的结点node的关键字个数为T-1时，
 * 为了满足下降过程中，遇到的结点的关键字个数大于等于T，
 * 对结点parent、node、othernode三个结点的关键字做调整。
 * 当node在other左侧时，即node的右结点时（父结点的右子结点）， * 在T+1位置，增加一个关键字，其值为父结点对应的关键字值，
 * 将父结点对应关键字值赋值为右子结点中的第一个关键字。
 * 将右子结点的关键字和子结点（如果有的话）向前移动一位
 * 修改右子结点以及该结点的n值
 */
void
btree_left(BTreeNode parent, BTreeNode node, BTreeNode othernode, int location)
{
    int i = 0;
    node->key[node->n] = parent->key[location];
    parent->key[location] = othernode->key[0];

    for(i = 0; i <= othernode->n - 2; i++)
        othernode->key[i] = othernode->key[i + 1];

    if(!othernode->leaf)
    {
        node->child[node->n + 1] = othernode->child[0];
        for(i = 0; i <= othernode->n - 1; i++)
            othernode->child[i] = othernode->child[i + 1];

    }

    node->n = node->n + 1;
    othernode->n = othernode->n - 1;
}

/*
 * 当下降的结点node的关键字个数为T-1时，
 * 为了满足下降过程中，遇到的结点的关键字个数大于等于T，
 * 对结点parent、node、othernode三个结点的关键字做调整。
 * 当node在other右侧时，即node的左结点时（父结点的左子结点），
 * node结点的关键字和子结点（如果有的话）向后移动一位,
 * 在第一个位置增加一个关键字，其值为父结点对应的关键字值，
 * 将父结点对应关键字值赋值为左子结点中的最后一个关键字。
 * 修改左子结点和该结点的n值
 */
void
btree_right(BTreeNode parent, BTreeNode node, BTreeNode othernode, int location)
{
    int i = 0;

    for(i = node->n - 1; i >= 0; i--)
        othernode->key[i+1] = othernode->key[i];

    node->key[0] = parent->key[location];
    parent->key[location] = othernode->key[othernode->n];

    if(!node->leaf)
    {
        node->child[0] = othernode->child[othernode->n + 1];
        for(i = othernode->n; i >= 0; i--)
            othernode->child[i + 1] = othernode->child[i];

    }

    node->n = node->n + 1;
    othernode->n = othernode->n - 1;
}

/*
 * 合并两个关键字个数为T-1父结点为parent位置为location的子结点
 * 以父结点对应的关键字为中间值连接两个子结点
 * 并返回需要下降的子结点位置
 */
int
btree_merge_child(BTreeNode parent, int location)
{
    int i;
    BTreeNode	lnode = NULL;
    BTreeNode	rnode = NULL;

    if(location == parent->n)
        location--;

    lnode = parent->child[location];
    rnode = parent->child[location + 1];

    /* 将父结点对应的关键字以及右兄弟所有的关键字复制该结点，同时修改左子的n值 */
    lnode->key[lnode->n] = parent->key[location];
    for(i = 0; i < rnode->n; i++)
    {
        lnode->key[MIN_T + i] = rnode->key[i];
        lnode->n++;
    }

    /* 如果有子结点同样复制到该结点 */
    if(!rnode->leaf)
        for(i = 0; i <= rnode->n; i++)
            lnode->child[MIN_T + i] = rnode->child[i];

    rnode->n= 0;
    lnode->n = MAX_T - 1;

    /* 对父结点相应的关键字和子结点位置发生变化 */
    for(i = location; i < parent->n - 1; i++)
    {
        parent->key[i] = parent->key[i + 1];
        parent->child[i + 1] = parent->child[i + 2];
    }

    /* 调整父结点的n值 */
    parent->n = parent->n - 1;
    rnode = NULL;

    return location;
}

/*
 * 对叶子结点node位置为location的关键字删除
 * 直接将位置location后的关键字向前移动一位
 */
void
btree_delete_leaf(BTreeNode node, int location)
{
    int i = 0;

    for(i = location; i < node->n - 1; i++)
        node->key[i] = node->key[i + 1];

    node->n = node->n - 1;
}

/*
 * 删除该层数组坐标为i的关键字
 */
int
btree_delete_node_in(BTreeNode r_node, int i)
{

    BTreeNode lnode = r_node->child[i];
    BTreeNode rnode = r_node->child[i + 1];
    int temp = 0;

    /*
    * 当前于该位置的关键字的左子结点关键字个数大于等于T时，
    * 寻找该位置的关键的前驱（左子结点的最大关键字）
    */
    if(lnode->n >= MIN_T)
    {
        BTreeNode newnode = btree_maximum(lnode);
        temp = r_node->key[i];
        r_node->key[i] = newnode->key[newnode->n - 1];
        newnode->key[newnode->n - 1] = temp;
    }
        /*
         * 相反的，若右子结点符合条件，则找寻后继（即右子结点的最小关键字）
         */
    else if(rnode->n >= MIN_T)
    {
        BTreeNode newnode = btree_minimum(rnode);
        temp = r_node->key[i];
        r_node->key[i] = newnode->key[0];
        newnode->key[0] = temp;

        i++;
    }
        /*
         * 当左右子结点都不符合条件，则合并两个子结点
         */
    else	i = btree_merge_child(r_node, i);

    return i;
}

/*
 * 删除以r_node为根结点的树的关键字key
 */
void
btree_delete_node(BTreeNode r_node, int key)
{
    int i = 0;


    /* 寻找关键字位置，或者下降的子结点位置 */
    while(i < r_node->n && key > r_node->key[i])
        i++;

    /* 若再该层且为叶子结点删除结点，否则下降寻找结点删除 */
    if(i < r_node->n && key == r_node->key[i])
        if(r_node->leaf)
            btree_delete_leaf(r_node, i);
        else
        {
            i = btree_delete_node_in(r_node, i);

            btree_delete_node(r_node->child[i], key);
        }
    else
    {
        if(r_node->leaf)
            printf("there is no the key %d!!\n", key);
        else
        {
            if(r_node->child[i]->n >= MIN_T){
                btree_delete_node(r_node->child[i], key);}
            else
            {
                if(i > 0 && r_node->child[i - 1]->n >= MIN_T)
                {
                    btree_right(r_node, r_node->child[i], r_node->child[i - 1], i);}
                else if(i < r_node->n && r_node->child[i + 1]->n >= MIN_T)
                    btree_left(r_node, r_node->child[i], r_node->child[i + 1], i);
                else
                    i = btree_merge_child(r_node, i);

                btree_delete_node(r_node->child[i], key);
            }
        }
    }
}

/*
 * 删除树内的关键字key，如果根结点为空，则替换根结点
 */
void
btree_delete(BTree tree, int key)
{
    BTreeNode r_node = tree->root;
    btree_delete_node(r_node, key);
    if(tree->root->n == 0 && tree->root->leaf == 0)
        tree->root = tree->root->child[0];
}