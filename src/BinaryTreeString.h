#ifndef BINARYTREESTRING_H_
#define BINARYTREESTRING_H_
typedef struct btreestr BinaryTreeStr;
struct btreestr {
	char* word;
	char** synonyms;
	int numberOfSynonyms;
	BinaryTreeStr *left;
	BinaryTreeStr *right;
};
int addToBinaryTreeStr(BinaryTreeStr* tree, char* word, char* synonyms[], int numberOfSynonyms);
BinaryTreeStr* searchInBinaryTreeStr(BinaryTreeStr* tree, const char* word);
#endif /* BINARYTREESTRING_H_ */
