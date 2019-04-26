#include "common.h"

// Circular queue
// History does not save that might be recursive(like '!!', '!n')
command history[hisBuf];
int ptr = 0;

int parse(char* args) {
	int length = 0, status = 0;
	char* word;
	int i;

	// Check history
	if (history[ptr].argc != 0) {
		for (i = 0; i < history[ptr].argc; i++)
			free(history[ptr].argv[i]);
		history[ptr].argc = 0;
	}

	// Check length
	word = strtok(args, " ");
	while (word != NULL) {

		if (length == 0) {
			// Check reserved words
			status = reservedWord(word);

			// When history excute, do not save(prevent recursion)
			if (status % 10 == 3 || status == -1)
				return status;
		}

		if (word[0] == '&')
			setWait(1);

		// Save command
		history[ptr].argv[length] = (char*)malloc(sizeof(char)*strlen(word));
		strcpy(history[ptr].argv[length], word);

		word = strtok(NULL, " ");
		length++;
	}

	history[ptr].argc = length;

	// Move pointer
	ptr = (ptr + 1) % hisBuf;

	return status;
}

void printHist() {
	int i, j, cnt;
	i = ptr;
	for (cnt = 0; cnt < hisBuf; cnt++) {
		if (history[i].argc != 0) {
			printf("%d ", hisBuf - cnt);
			for (j = 0; j < history[i].argc; j++)
				printf("%s ", history[i].argv[j]);
			printf("\n");
		}
		i = (i + 1) % hisBuf;
	}
}

int getHisPtr() {
	return ptr;
}

command getHis(int ptr) {
	return history[ptr];
}

int reservedWord(char* word) {
	if (strcmp(word, "quit") == 0 || strcmp(word, "exit") == 0)
		return 1;
	else if (strcmp(word, "history") == 0)
		return 2;
	else if (strcmp(word, "!!") == 0)
		return 3;
	else if (word[0]=='!') {
		//parse
		int i = atoi(word+1);
		if (i == 0) {
			printf("Wrong Input\n");
			return -1;
		}

		if ((i - 1) / hisBuf != 0) {
			printf("Integer is too big(MAX:%d)\n", hisBuf);
			return -1;
		}

		return (i - 1) * 10 + 3;
	}
	else if (strcmp(word, "cd") == 0) {
		return 4;
	}
	else if (strcmp(word, "") == 0);
	// normal excute
	else
		return 0;
	return -1;
}

void echoCom(command com) {
	int i = 0;
	for (i = 0; i < com.argc; i++)
		printf("%s ", com.argv[i]);
	printf("\n");
}

void initParser() {
	int i;
	for (i = 0; i < hisBuf; i++) {
		history[i].argc = 0;
	}
}

void freeParser() {
	int i, j;
	for (i = 0; i < hisBuf; i++) {
		if (history[i].argc != 0) {
			for (j = 0; j < history[i].argc; j++)
				free(history[i].argv[j]);
			history[i].argc = 0;
		}
	}
}