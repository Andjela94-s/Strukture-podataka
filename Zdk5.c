#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 100
#define FILE_OPEN_ERROR -1
#define MALLOC_ERROR -2
#define STACK_UNDERFLOW -3

typedef struct StackItem {
	float value;
	struct StackItem* next;
}_stackitem;

typedef struct Stack {
	_stackitem* top;
}_stack;

int push(_stack* stack, float value);
int pop(_stack* stack, float* value);

int main() {
	int size = 0;
	float first=0, second=0, result=0;
	char current[10] = { 0 };
	char postfix[MAXSIZE] = { 0 };
	char* postfixPtr = postfix;
	FILE* fp = NULL;
	_stack stack = { .top=NULL};

	fp = fopen("postfix.txt", "r");
	if (fp == NULL) {
		printf("Error opening file!\n");
		return FILE_OPEN_ERROR;
	}

	fgets(postfix, MAXSIZE, fp);
	printf("Postfix expression: %s\n", postfix);
	while (*postfixPtr != '\0' && *postfixPtr != '\n') {
		if (sscanf(postfixPtr, "%s", current) != 1) {
			break;
		}
		postfixPtr += strlen(current) + 1;
		if (sscanf(current, "%f", &first) == 1) {
			push(&stack, first);
		}
		else {
			pop(&stack, &second);
			pop(&stack, &first);
			switch (current[0]) {
			case '+':
				result = first + second;
				break;
			case '-':
				result = first - second;
				break;
			case '*':
				result = first * second;
				break;
			case '/':
				result = first / second;
				break;
			default:
				printf("Unknown operator: %s\n", current);
				return -1;
			}
			push(&stack, result);
		}
	}
	pop(&stack, &result);
	printf("Result: %.2f\n", result);
	fclose(fp);

	return 0;
}

int push(_stack* stack, float value) {
	_stackitem* newItem = (_stackitem*)malloc(sizeof(_stackitem));
	if (newItem == NULL) {
		return MALLOC_ERROR; // Memory allocation error
	}
	newItem->value = value;
	newItem->next = stack->top;
	stack->top = newItem;
	return 0; // Success
}

int pop(_stack* stack, float* value) {
	if (stack->top == NULL) {
		return STACK_UNDERFLOW; // Stack underflow error
	}
	_stackitem* temp = stack->top;
	*value = temp->value;
	stack->top = stack->top->next;
	free(temp);
	return 0; // Success
}