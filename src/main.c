#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "BinaryTreeString.h"
#define N (100)
int countCharsInString(char* str, char ch);
void parseSynonymsFile(FILE* file, BinaryTreeStr* root);
int readLine(char* buffer, int bufferSize, FILE* file);
int readWord(char* buffer, int bufferSize, FILE* file);
int findKeyAndCopy(char* mainBuffer, int pos, char** wordPtr);
int copySynonym(char* mainBuffer, int pos, char** synonyms,int synonymIndex);
void replaceWordsInFile(FILE* input, FILE* output, BinaryTreeStr* synonymBankTree);
char* createNewFilename(char* filename);
int validateArguments(int argc, char** argv);

int main(int argc, char **argv) {
	BinaryTreeStr btRoot = (BinaryTreeStr) { NULL, NULL, 0, NULL, NULL };
	FILE* synonymDatabase;
	FILE* textInput;
	FILE* textOutput;
	char *inputFilename;
	char* outputFilename;
	/*
	if (!validateArguments(argc, argv)) {
		return 1;
	}
	inputFilename = argv[2];
	*/
	inputFilename = "textfile.txt";
	outputFilename = createNewFilename(inputFilename);
	synonymDatabase = fopen("database.txt", "r");
	textInput = fopen(inputFilename, "r");
	textOutput = fopen(outputFilename, "w");
	parseSynonymsFile(synonymDatabase, &btRoot);
	replaceWordsInFile(textInput, textOutput, &btRoot);
	return 0;
}

/*
 * check that 2 arguments exist
 * Database filepath and Text filepath
 */
int validateArguments(int argc, char** argv) {
	if(argc < 2) {
		printf("Not enough arguments");
		return 1;
	}
	return 0;
}

/*
 * Creates a string from filename, adding a letter to the start
 * @Param filename - string of filename
 * @Returns new string with modified filename
 */
char* createNewFilename(char* filename) {
	char* fileExtesion = ".mod";
	int len = strlen(filename);
	char* newName = (char *) malloc(sizeof(char) * len + 5);// 1 for '\0', 4 for '.mod' file extension
	if (newName == NULL)
		return NULL;
	strcpy(newName, filename);
	strcpy(newName + len, fileExtesion);
	return newName;
}

/*
 * Reads words from input and replaces them with synonyms
 * from Binary Tree - synonymBankTree
 * @Param input - Input file
 * @Param output - Output file
 * @Param synonymBankTree - Binary Search Tree of synonyms
 */
void replaceWordsInFile(FILE* input, FILE* output, BinaryTreeStr* synonymBankTree) {
	char buffer[N];
	BinaryTreeStr* binaryNode;
	int readResult;
	int synonymIndex;
	srand(time(NULL));
	while((readResult = readWord(buffer, N, input)) != 0) {
		binaryNode = searchInBinaryTreeStr(synonymBankTree, buffer);
		if(binaryNode == NULL)
			fputs(buffer, output);
		else {
			synonymIndex = rand() % binaryNode->numberOfSynonyms;
			fputs(binaryNode->synonyms[synonymIndex], output);
		}
		if(readResult == 2) {
			fputc(' ', output);
		}
		else if(readResult == 3) {
			fputc('\n', output);
		}
	}
}
/*
 * Pares a the file and places the words and
 * its synonyms inside a Binary tree
 * @Param file - file to parse from
 * @Param root - binary tree to put data into
 */
void parseSynonymsFile(FILE* file, BinaryTreeStr* root) {
	char buffer[N];
	int bufferPos;
	char* currentWord;
	char** currentSynonyms;
	int synonymsCount;
	int i;
	while(readLine(buffer, N, file) != 0) {
		bufferPos = 0;
		bufferPos = findKeyAndCopy(buffer, bufferPos, &currentWord);
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
	ch = fgetc(file);
	if(ch == EOF) {
		return 0;
	}
	do {
		buffer[bufferPos] = ch;
		bufferPos++;
		ch = fgetc(file);
	} while(ch != EOF && ch != '\n' && bufferPos < limit);
	buffer[bufferPos] = '\0';
	return 1;
}
/* Reads a word from file in to buffer
 * reads until word, end of buffer or EOF
 * @Param buffer - Buffer to put word into
 * @Param bufferSize - Buffer's size
 * @Param file - File to read from
 * @Returns 0 if reached EOF, 2 if reached space
 * 3 if reached newline, else 1
 */
int readWord(char* buffer, int bufferSize, FILE* file) {
	char ch;
	int bufferPos = 0;
	int limit = bufferSize - 1;
	ch = fgetc(file);
	if(ch == EOF) {
		return 0;
	}
	do {
		buffer[bufferPos] = ch;
		bufferPos++;
		ch = fgetc(file);
	} while(ch != EOF && ch != ' ' && ch != '\n' && bufferPos < limit);
	buffer[bufferPos] = '\0';
	if (ch == ' ')
		return 2;
	if (ch == '\n')
		return 3;
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
