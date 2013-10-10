#include <stdio.h>
#include <stdlib.h>
#include "BinaryTreeString.h"
#define N (50)
int countCharsInString(char* str, char ch);
void parseWordsFile(FILE* file, BinaryTreeStr* root);
int readLine(char* buffer, int bufferSize, FILE* file);
int findKeyAndCopy(char* mainBuffer, int pos, char** wordPtr);
int copySynonym(char* mainBuffer, int pos, char** synonyms,int synonymIndex);

int main() {
	BinaryTreeStr btRoot = (BinaryTreeStr) { NULL, NULL, 0, NULL, NULL };
	FILE* synonymDatabase;
	FILE* textInput;
	FILE* textOutput;
	synonymDatabase = fopen("database.txt", "r");
	textInput = stdin;
	textOutput = stdout;
	parseWordsFile(synonymDatabase, &btRoot);
	return 0;
}
/*
 * Pares a the file and places the words and
 * its synonyms inside a Binary tree
 * @Param file - file to parse from
 * @Param root - binary tree to put data into
 */
void parseWordsFile(FILE* file, BinaryTreeStr* root) {
	char buffer[N];
	int bufferPos;
	char* currentWord;
	char** currentSynonyms;
	int synonymsCount;
	int i;
	while(readLine(buffer, N, file) != 0) {
		bufferPos = 0;
		findKeyAndCopy(buffer, bufferPos, &currentWord);
		synonymsCount = countCharsInString(buffer, ';');
		currentSynonyms = (char** ) malloc(sizeof(char*) * synonymsCount);
		for(i = 0; i < synonymsCount; i++) {
			bufferPos = copySynonym(buffer, bufferPos, currentSynonyms, i);
		}
		addToBinaryTreeStr(root, currentWord, currentSynonyms, synonymsCount);
	}
}
/* Copies for a synonym and places it in synonyms
 * the synonym ends when a ';' is read
 * The function will allocate space for the synonym
 * @Param mainBuffer - Buffer to copy the synonym from
 * @Param pos - Stating index to copy from
 * @Param synonyms - Synonym array(must be allocated before call)
 * @Param synonymIndex - Index in synonyms array to place the string
 * @Returns - returns index after ';' (next synonym)
 */
int copySynonym(char* mainBuffer, int pos, char** synonyms,int synonymIndex) {
	int startIndex = pos;
	int strLen = 0;
	int i;
	while(mainBuffer[pos] != ';') {
		strLen++; pos++;
	}
	synonyms[synonymIndex] = (char *) malloc(sizeof(char) * strLen);
	for(i = 0; startIndex < pos; startIndex++, i++) {
		synonyms[synonymIndex][i] = mainBuffer[startIndex];
	}
	synonyms[synonymIndex][i] = '\0';
	return pos + 1;
}
/* Searches for the key(first word in line) and places in wordPtr
 * the key ends when a ':' is read
 * The function will allocate space for the key
 * @Param mainBuffer - Buffer to seach for key
 * @Param pos - Stating index to search from
 * @Param wordPtr - pointer to pointer of the key
 * @Returns - returns index after ':' (first synonym)
 */
int findKeyAndCopy(char* mainBuffer, int pos, char** wordPtr) {
	char ch;
	int i;
	while(pos < N - 1) {
		ch = mainBuffer[pos];
		if (ch == ':') {
			*wordPtr = (char *) malloc(sizeof(char) * pos + 1);
			for(i = 0; i < pos; i++) {
				(*wordPtr)[i] = mainBuffer[i];
			}
			(*wordPtr)[pos] = '\0';
			break;
		}
		else {
			pos++;
		}
	}
	return pos + 1;
}
/* Reads a line from file in to buffer
 * reads until end of line, end of buffer or EOF
 * @Param buffer - Buffer to put string into
 * @Param bufferSize - Buffer's size
 * @Param file - File to read from
 * @Returns 0 if reached EOF, else 1
 */
int readLine(char* buffer, int bufferSize, FILE* file) {
	char ch;
	int bufferPos = 0;
	int limit = bufferSize - 1;
	while((ch = fgetc(file)) != EOF && ch != '\n' && bufferPos < limit) {
		buffer[bufferPos] = ch;
		bufferPos++;
	}
	buffer[bufferPos] = '\0';
	if (ch == EOF)
		return 0;
	return 1;
}
/* Counts the number of times the character appears in the string
 * @Param str - String to search in
 * @Param ch - Character to count in string
 * @Returns - Number of appearances of ch in str
 */
int countCharsInString(char* str, char ch) {
	int count = 0;
	int i = 0;
	while(str[i] != '\0') {
		if(str[i] == ch) {
			count++;
		}
		i++;
	}
	return count;
}
