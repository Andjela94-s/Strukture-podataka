#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 100

#define EMPTYSTACK 1
#define EXIT_SUCCESS 0
#define MALLOC_ERROR -1
#define SCANF_ERROR -2

struct directory;
typedef struct directory* DirPosition;
typedef struct directory {
	char name[MAXLINE];
	DirPosition next;
	DirPosition subdir;
} Directory;

struct stack;
typedef struct stack* StackPosition;
typedef struct stack {
	DirPosition dir;
	StackPosition next;
} Stack;

DirPosition CreateDirectory(char*);
int Push(StackPosition, DirPosition);
int Pop(StackPosition);
int AddDirectory(DirPosition,DirPosition);
int ChangeDirectory(StackPosition, char*);
int PrintDirectories(DirPosition, int);
int DeleteAll(DirPosition);
int DeleteStack(StackPosition);

int main() {
	char input = 0;
	char dirName[MAXLINE] = { 0 };
	Directory headDir = { .name = "", .next = NULL, .subdir = NULL };
	DirPosition rootDir = NULL;
	Stack stackHead = { .dir = NULL, .next = NULL };

	rootDir = CreateDirectory("C:");
	if (!rootDir) {
		return MALLOC_ERROR;
	}

	headDir.next = rootDir;
	Push(&stackHead, rootDir);

	printf("Welcome to the simple file system!\n");
	printf("Available commands:\n");
	printf("1. Create Directory\n");
	printf("2. Change Directory\n");
	printf("3. Go to parent Directory\n");
	printf("4. View Directory content\n");
	printf("5. Exit\n");

	do
	{
		printf("Enter command:\n");
		if (scanf(" %c", &input) != 1) {
			return SCANF_ERROR;
		}
		switch (input)
		{
			case '1':
			printf("md ");
			if(scanf(" %s", dirName)!=1){
				return SCANF_ERROR;
			}
			DirPosition newDir = CreateDirectory(dirName);
			if (!newDir) {
				return MALLOC_ERROR;
			}
			AddDirectory(stackHead.next->dir, newDir);
			printf("Directory created successfully!\n");
			break;
			case '2':
				printf("cd ");
				if(scanf(" %s", dirName)!=1){
					return SCANF_ERROR;
				}
				ChangeDirectory(&stackHead, dirName);
				break;
			case '3':
				if (Pop(&stackHead)) {
					printf("Already at root directory!\n");
				}
				else {
					printf("Moved to parent directory!\n");
				}
				break;
			case '4':
				printf("Directory content:\n");
				PrintDirectories(stackHead.next->dir, 0);
				break;
			case '5':
				printf("Exiting the program...\n");
				break;
			default:
				printf("Invalid command! Please try again.\n");
				break;
		}
	} while (input != '5');

	DeleteAll(&headDir);
	DeleteStack(&stackHead);

	return EXIT_SUCCESS;
}
DirPosition CreateDirectory(char* name) {
	
	DirPosition newDir = NULL;
	newDir = (DirPosition)malloc(sizeof(Directory));
	if (!newDir) {
		return NULL;
	}
	strcpy(newDir->name, name);
	newDir->next = NULL;
	newDir->subdir = NULL;
	return newDir;
}
int Push(StackPosition stackHead, DirPosition dir) {
	StackPosition newElem = NULL;
	newElem = (StackPosition)malloc(sizeof(Stack));
	if (!newElem) {
		return MALLOC_ERROR;
	}
	newElem->dir = dir;
	newElem->next = stackHead->next;
	stackHead->next = newElem;
	return EXIT_SUCCESS;
}
int Pop(StackPosition stackHead) {
	StackPosition temp = NULL;
	if (!stackHead->next->next) {
		return EMPTYSTACK;
	}
	temp = stackHead->next;
	stackHead->next = stackHead->next->next;
	free(temp);
	return EXIT_SUCCESS;
}
int AddDirectory(DirPosition parent, DirPosition newDir) {
	parent->next = parent->subdir;
	parent->subdir = newDir;

	return EXIT_SUCCESS;
}
int ChangeDirectory(StackPosition stackHead, char* dirName) {
	DirPosition temp = stackHead->next->dir->subdir;
	while (temp != NULL && strcmp(temp->name,dirName) != 0) {
		temp = temp->next;
	}
	if (temp != NULL) {
		printf("Changed directory to %s\n", dirName);
		Push(stackHead,temp);
	}
	else {
		printf("Directory not found!\n");
	}

	return EXIT_SUCCESS;
}
int PrintDirectories(DirPosition dir, int level) {
	int i = 0;
	DirPosition tempSubdir = NULL;

	if (dir->subdir != NULL) {
		tempSubdir = dir->subdir;
		while (dir != NULL) {
			for (i = 0; i < level; i++) {
				printf("  ");
			}
			printf("%s\n", tempSubdir->name);
			PrintDirectories(tempSubdir, level + 1);
			tempSubdir = tempSubdir->next;
		}
	}
	return EXIT_SUCCESS;
}
int DeleteAll(DirPosition dir) {
	DirPosition temp = dir->subdir;
	DirPosition next = NULL;
	while (temp) {
		next = temp->next;
		DeleteAll(temp);
		free(temp);
		temp = next;
	}
	return EXIT_SUCCESS;
}
int DeleteStack(StackPosition stackHead) {
	StackPosition temp = stackHead->next;
	StackPosition next = NULL;
	while (temp) {
		next = temp->next;
		free(temp);
		temp = next;
	}
	return EXIT_SUCCESS;
}