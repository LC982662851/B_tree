//
// Created by liangchen on 2022/1/7.
//

#ifndef WAN0_BTREE_H
#define WAN0_BTREE_H
#define MIN_T 3
#define MAX_T (MIN_T * 2)
#define BTREE_NODE_SIZE sizeof(BTreeNodedata)
#define BTREE_SIZE sizeof(BTreedata)

typedef struct BTreeNodedata BTreeNodedata;
typedef struct BTreeNodedata *BTreeNode;
typedef struct BTreedata BTreedata;
typedef struct BTreedata *BTree;

/*
 * B树结点结构体
 */
struct BTreeNodedata
{
    int n;	//关键字个数
    int leaf;	//是否是叶子结点，1为叶子结点，0反之
    int key[MAX_T - 1];	//关键字，这里的关键字为了简便编程设为int
    BTreeNode child[MAX_T];	//子结点
};

/*
 * B树的结构体
 */
struct BTreedata
{
    BTreeNode	root;	//B树的根结点
};

//#define BTREE_NODE_SIZE sizeof(BTreeNodedata)
//#define BTREE_SIZE sizeof(BTreedata)

BTreeNode  allocate_node();	//为结点分配空间
void btree_create(BTree tree);	//初始化树
void btree_search(BTreeNode node, int key);	//寻找关键字位置
void btree_split_child(BTreeNode node, int location);	//分裂子结点
void btree_insert_nonfull(BTreeNode node, int key);	//向未满结点插入关键字
void btree_insert(BTree tree, int key);	//向树插入关键字
void display_node(BTreeNode *node_first, int n);	//显示以结点node_first为父结点的树
void display_btree(BTree tree);	//显示整棵树
BTreeNode btree_minimum(BTreeNode node);	//以node为根结点，寻找最小关键字
BTreeNode btree_maximum(BTreeNode node);	//以node为根结点，寻找最大关键字
void btree_min(BTree tree);	//在整棵树中寻找最小关键字
void btree_max(BTree tree);	//在整棵树中寻找最大关键字
void btree_left(BTreeNode parent, BTreeNode node, BTreeNode othernode, int location);	//将父结点、右兄弟、该结点的关键字调整
void btree_right(BTreeNode parent, BTreeNode node, BTreeNode othernode, int location);	//将父结点、左兄弟、该结点的关键字调整
int btree_merge_child(BTreeNode parent, int location);	//合并子结点，并返回下降子结点的位置
void btree_delete_leaf(BTreeNode node, int location);	//删除叶子结点关键字
int btree_delete_node_in(BTreeNode r_node, int i);	//删除内结点关键字，并返回下降子结点的位置
void btree_delete_node(BTreeNode r_node, int key);	//删除以r_node为根结点的树中关键字
void btree_delete(BTree tree, int key);	//删除树中的关键字

#endif //WAN0_BTREE_H
