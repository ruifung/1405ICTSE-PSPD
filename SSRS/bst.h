#ifndef BST
#define BST
#define BST_CMPR_ARGS const void *key1, const uint key1len, const void *key2, const uint key2len

typedef unsigned int uint;
typedef int(*BST_CMPR)(BST_CMPR_ARGS);
typedef struct BST_NODE_S BST_NODE;
struct BST_NODE_S {
	unsigned int keylen;
	void *key;
	void *data;
	BST_NODE *left;
	BST_NODE *right;
};

bool bst_addNode(BST_NODE **node, void *key, uint keylen, void *data, BST_CMPR cmpr);
BST_NODE *bst_search(BST_NODE *node, void *key, uint keylen, BST_CMPR cmpr);
uint bst_countChilds(BST_NODE *node);
void bst_getChilds(BST_NODE *node, BST_NODE *storageLocation,uint maxCount);
void bst_deallocate(BST_NODE *node);

//String Wrappers
bool bst_addNodeStr(BST_NODE **node, char *string, void *data);
BST_NODE *bst_searchStr(BST_NODE *node, char *string);
#endif
