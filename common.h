#pragma once
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// Holding header

// main
#define MAX_LENGTH 80

typedef struct _command {
	int argc;
	char* argv[MAX_LENGTH / 2 + 1];
}command;

// parser.c
#define hisBuf 10
int parse(char* args);
int reservedWord(char* word);

void printHist();
int getHisPtr();
command getHis(int ptr);
void echoCom(command com);

void initParser();
void freeParser();

// excute.c
int excute(int argc, char *argv[]);
int excuteR(int argc, char *argv[], int status);
void setWait(int nw);

void prompt();

void initExcute();
void freeExcute();