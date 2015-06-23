#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


#include "bst.h"

//INTERNAL USE FUNCTION PROTOTYPES.
bool bst_addBstNode(BST_NODE **node, BST_NODE newNode, BST_CMPR cmpr);
void bst_countChildsRecur(BST_NODE *node, uint *count);
void bst_getChildsRecur(BST_NODE *node, uint *currentIndex, BST_NODE *dataArray, unsigned int maxCount);

int bst_strcmp (BST_CMPR_ARGS) {
	return strcmp((char *)key1, (char *)key2);
}

bool bst_addNodeStr(BST_NODE **node, char *string, void *data) {
	bst_addNode(node, string, strlen(string), data, &bst_strcmp);
}

bool bst_addNode(BST_NODE **node, void *key, unsigned int keylen, void *data, BST_CMPR cmpr) {
	BST_NODE nodeData;
	nodeData.keylen = keylen;
	nodeData.key = malloc(keylen);
	memcpy(nodeData.key, key, keylen);
	nodeData.data = data;
	return bst_addBstNode(node, nodeData, cmpr);
}

bool bst_addBstNode(BST_NODE **node, BST_NODE newNode, BST_CMPR cmpr) {
	if (*node == NULL) {
		*node = malloc(sizeof(BST_NODE));
		if (*node == NULL) return false;
		newNode.left = NULL;
		newNode.right = NULL;
		memcpy(*node, &newNode, sizeof(BST_NODE));
		return true;
	}
	else {
		//Compares 2 keys using provided compare function
		int cmp = (*cmpr)(newNode.key, newNode.keylen, (*node)->key, (*node)->keylen);
		if (cmp < 0) return bst_addBstNode(&(*node)->left, newNode, cmpr);
		else if (cmp > 0) return bst_addBstNode(&(*node)->right, newNode, cmpr);
		else return false;
	}
}

BST_NODE *bst_searchStr(BST_NODE *node, char *string) {
	return bst_search(node, string, strlen(string), &bst_strcmp);
}

BST_NODE *bst_search(BST_NODE *node, void *key, uint keylen, BST_CMPR cmpr) {
	if (node == NULL) return NULL;
	int cmp = (*cmpr)(key, keylen, node->key, node->keylen);
	if (cmp == 0)
		return node;
	if (cmp < 0) 
		return bst_search(node->left, key, keylen, cmpr);
	if (cmp > 0)
		return bst_search(node->right, key, keylen, cmpr);
	return NULL;
}

//Stores all child nodes in storageLocation and returns count.
uint bst_countChilds(BST_NODE *node) {
	uint count = 0;
	bst_countChildsRecur(node, &count);
	return count;
}

void bst_getChilds(BST_NODE *node, BST_NODE *storageLocation, uint maxCount) {
	uint index = 0;
	bst_getChildsRecur(node, &index, storageLocation, maxCount);
}

void bst_countChildsRecur(BST_NODE *node, uint *count) {
	*count++;
	if (node->left != NULL)
		bst_countChildsRecur(node, count);
	if (node->right != NULL)
		bst_countChildsRecur(node, count);
	return;
}

void bst_getChildsRecur(BST_NODE *node, uint *currentIndex, BST_NODE *dataArray, uint maxCount) {
	if (node == NULL) return;
	dataArray[*currentIndex] = *node;
	dataArray[*currentIndex].left = NULL;
	dataArray[*currentIndex].right = NULL;
	*currentIndex++;
	bst_getChildsRecur(node->left, currentIndex, dataArray, maxCount);
	bst_getChildsRecur(node->right, currentIndex, dataArray, maxCount);
}

//Frees all memory allocated to a node and its branches.
void bst_deallocate(BST_NODE *node) {
	if (node->left != NULL)
		bst_deallocate(node->left);
	if (node->right != NULL)
		bst_deallocate(node->right);
	free(node->key);
	free(node);
}