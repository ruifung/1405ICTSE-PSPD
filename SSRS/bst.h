#ifndef BST
#define BST

typedef struct {
	char *string;
	void *data;
	BST_NODE *left;
	BST_NODE *right;
} BST_NODE;

bool bst_addNode(BST_NODE **rootNode, wchar_t *string, void *data);
BST_NODE *bst_search(wchar_t *string);
int bst_getChilds(BST_NODE *node, BST_NODE **storageLocation);
bool bst_deallocate(BST_NODE *node);
#endif
