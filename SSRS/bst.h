#ifndef BST
#define BST

typedef struct BST_NODE_S{
	char *string;
	void *data;
	struct BST_NODE_S *left;
	struct BST_NODE_S *right;
} BST_NODE;

bool bst_addNode(BST_NODE **rootNode, wchar_t *string, void *data);
BST_NODE *bst_search(wchar_t *string);
int bst_getChilds(BST_NODE *node, BST_NODE **storageLocation);
bool bst_deallocate(BST_NODE *node);
#endif
