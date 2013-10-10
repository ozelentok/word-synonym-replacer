#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "BinaryTreeString.h"
/*
 * @Parm tree - Tree to add new node to
 * @Param word - Word(key) of new node
 * @Param synonyms - Synonyms of word(key) of new node
 * @Param numberOfSynonyms - Number of synonyms
 */
int addToBinaryTreeStr(BinaryTreeStr* tree, char* word, char* synonyms[], int numberOfSynonyms) {
	int cmpResult;
	if(tree == NULL) {
		return 1;
	}
	if(tree->word == NULL) {
		tree->word = word;
		tree->synonyms = synonyms;
		tree->numberOfSynonyms = numberOfSynonyms;
		tree->left = NULL;
		tree->right = NULL;
		return 0;
	}
	cmpResult = strcmp(word, tree->word);
	if (cmpResult < 0) {
		if(tree->left == NULL) {
			tree->left = malloc(sizeof(BinaryTreeStr));
			tree->left->word = NULL;
		}
		return addToBinaryTreeStr(tree->left, word, synonyms, numberOfSynonyms);
	}
	if(cmpResult > 0) {
		if(tree->right == NULL) {
			tree->right = malloc(sizeof(BinaryTreeStr));
			tree->right->word = NULL;
		}
		return addToBinaryTreeStr(tree->right, word, synonyms, numberOfSynonyms);
	}
	return 0;
}
