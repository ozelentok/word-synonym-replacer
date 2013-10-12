#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BinaryTreeString.h"
/*
 * @Parma tree - Tree to add new node to
 * @Param word - Word(key) of new node
 * @Param synonyms - Synonyms of word(key) of new node
 * @Param numberOfSynonyms - Number of synonyms
 * @Returns 0 if the key(word) already exists in the tree
 * 1 if the word and it's synonyms has been added
 * -1 if if memory allocation failed
 */
int addToBinaryTreeStr(BinaryTreeStr* tree, char* word, char* synonyms[], int numberOfSynonyms) {
	int cmpResult;
	if(tree == NULL) {
		return -1;
	}
	if(tree->word == NULL) {
		tree->word = word;
		tree->synonyms = synonyms;
		tree->numberOfSynonyms = numberOfSynonyms;
		tree->left = NULL;
		tree->right = NULL;
		return 1;
	}
	cmpResult = strcmp(word, tree->word);
	if (cmpResult < 0) {
		if(tree->left == NULL) {
			tree->left = malloc(sizeof(BinaryTreeStr));
			if(tree->left == NULL) {
				return -1;
			}
			tree->left->word = NULL;
		}
		return addToBinaryTreeStr(tree->left, word, synonyms, numberOfSynonyms);
	}
	if(cmpResult > 0) {
		if(tree->right == NULL) {
			tree->right = malloc(sizeof(BinaryTreeStr));
			if(tree->right == NULL) {
				return 1;
			}
			tree->right->word = NULL;
		}
		return addToBinaryTreeStr(tree->right, word, synonyms, numberOfSynonyms);
	}
	return 0;
}
/*
 * @Parma tree - Tree to sarch in
 * @Param word - key(word) to search with
 * @Returns the node if the word was found
 * NULL if the word was not found
 */
BinaryTreeStr* searchInBinaryTreeStr(BinaryTreeStr* tree, const char* word) {
	int cmpResult;
	if(tree == NULL) {
		return NULL;
	}
	cmpResult = strcmp(word, tree->word);
	if (cmpResult < 0) {
		return searchInBinaryTreeStr(tree->left, word);
	}
	if(cmpResult > 0) {
		return searchInBinaryTreeStr(tree->right, word);
	}
	return tree;
}
