#include <malloc.h>
#include "btree.h"


int main()
{
    BTree tree = (BTree) malloc (BTREE_SIZE);
    tree->root	= (BTreeNode) malloc (BTREE_NODE_SIZE);

    int keys[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,25,24,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
                  41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,
                  82,81,83,84,85,86,87,88,89,90,91,92,99,94,93,95,96,97,98,100};
    int i = 0;

    btree_create(tree);
    for(i = 0; i <= 99; i++){
        btree_insert(tree, keys[i]);
        //display_btree(&tree);
    }
    btree_max(tree);
    display_btree(tree);
    btree_max(tree);
    btree_min(tree);
    btree_search(tree->root,5);

    btree_search(tree->root,33);

//
    btree_delete(tree, 100);
    display_btree(tree);
//    btree_delete(tree, 94);
//    display_btree(tree);

    btree_max(tree);
    btree_min(tree);

    free(tree);
    return 0;
}