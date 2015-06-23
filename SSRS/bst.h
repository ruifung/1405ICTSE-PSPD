#ifndef BST
#define BST

typedef struct BST_NODE_S BST_NODE;
struct BST_NODE_S {
	char *string;
	void *data;
	BST_NODE *left;
	BST_NODE *right;
};

bool bst_addNode(BST_NODE **rootNode, char *string, void *data);
BST_NODE *bst_search(BST_NODE *node, char *string);
unsigned int bst_countChilds(BST_NODE *node);
void bst_getChilds(BST_NODE *node, BST_NODE *storageLocation,unsigned int maxCount);
bool bst_deallocate(BST_NODE *node);
#endif
