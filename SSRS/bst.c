#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


#include "bst.h"

//INTERNAL USE FUNCTION PROTOTYPES.
bool bst_addBstNode(BST_NODE **node, BST_NODE newNode);
void bst_countChildsRecur(BST_NODE *node, unsigned int *count);
void bst_getChildsRecur(BST_NODE *node, unsigned int *currentIndex, BST_NODE *dataArray, unsigned int maxCount);

bool bst_addNode(BST_NODE **node, char *string, void *data) {
	BST_NODE nodeData;
	nodeData.string = string;
	nodeData.data = data;
	return bst_addBstNode(node, nodeData);
}

bool bst_addBstNode(BST_NODE **node, BST_NODE newNode) {
	if (*node == NULL) {
		*node = malloc(sizeof(BST_NODE));
		if (*node == NULL) return false;
		newNode.left = NULL;
		newNode.right = NULL;
		memcpy(*node, &newNode, sizeof(BST_NODE));
		return true;
	}
	else {
		int cmp = strcmp(newNode.string, (*node)->string);
		if (cmp < 0) return bst_addBstNode(&(*node)->left, newNode);
		else if (cmp > 0) return bst_addBstNode(&(*node)->right, newNode);
		else return false;
	}
}

BST_NODE *bst_search(BST_NODE *node,char *string) {
	if (node == NULL) return NULL;
	int cmp = strcmp(string, node->string);
	if (cmp == 0)
		return node;
	if (cmp < 0) 
		return bst_search(node->left, string);
	if (cmp > 0)
		return bst_search(node->right, string);
	return NULL;
}

//Stores all child nodes in storageLocation and returns count.
int bst_getChilds(BST_NODE *node, BST_NODE **storageLocation) {
	int count = 0;
	bst_countChildsRecur(node, &count);
	*storageLocation = malloc(sizeof(BST_NODE) * count);
	int index = 0;
	bst_getChildsRecur(node, &index, *storageLocation);
	return count;
}

void bst_countChildsRecur(BST_NODE *node, int *count) {
	*count++;
	if (node->left != NULL)
		bst_countChildsRecur(node, count);
	if (node->right != NULL)
		bst_countChildsRecur(node, count);
	return;
}

void bst_getChildsRecur(BST_NODE *node, int *currentIndex, BST_NODE *dataArray) {
	if (node == NULL) return;
	dataArray[*currentIndex] = *node;
	dataArray[*currentIndex].left = NULL;
	dataArray[*currentIndex].right = NULL;
	*currentIndex++;
	bst_getChildsRecur(node->left, currentIndex, dataArray);
	bst_getChildsRecur(node->right, currentIndex, dataArray);
}

//Frees all memory allocated to a node and its branches.
void bst_deallocate(BST_NODE *node) {
	if (node->left != NULL)
		bst_deallocate(node->left);
	if (node->right != NULL)
		bst_deallocate(node->right);
	free(node);
}