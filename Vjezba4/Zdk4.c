#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024
#define MAX_SIZE 100

#define FILE_ERROR -1
#define MEMORY_ERROR -2

typedef struct Polynomial {
	int coefficient;
	int exponent;
	struct Polynomial* next;
} Polynomial;

int StrtoPoly(Polynomial*, char*);
int AddPoly(Polynomial*, Polynomial*);
int PrintPoly(Polynomial*);
int PrintTerm(Polynomial*);
int SumPoly(Polynomial*, Polynomial*, Polynomial*);
int PolyProd(Polynomial*, Polynomial*, Polynomial*);
int DeletePoly(Polynomial*);

int main()
{
	Polynomial Head1 = {0, 0, NULL};
	Polynomial Head2 = {0, 0, NULL};
	Polynomial Sum = {0, 0, NULL};
	Polynomial Prod = {0, 0, NULL};
	char buffer[MAX_LINE] = { 0 };

	FILE* fp1 = fopen("polinom1.txt", "r");
	if (fp1 == NULL) {
		printf("Error opening file!\n");
		return FILE_ERROR;
	}

	fgets(buffer, MAX_LINE, fp1);
	StrtoPoly(&Head1, buffer);

	FILE* fp2 = fopen("polinom2.txt", "r");
	if (fp2 == NULL) {
		printf("Error opening file!\n");
		return FILE_ERROR;
	}
	fgets(buffer, MAX_LINE, fp2);
	StrtoPoly(&Head2, buffer);

	printf("First polynomial: \n");
	PrintPoly(Head1.next);
	printf("Second polynomial: \n");
	PrintPoly(Head2.next);

	SumPoly(&Head1, &Head2, &Sum);
	printf("Sum of polynomials: \n");
	PrintPoly(Sum.next);

	PolyProd(&Head1, &Head2, &Prod);
	printf("Product of polynomials: \n");
	PrintPoly(Prod.next);

	fclose(fp1);
	fclose(fp2);

	DeletePoly(&Head1);
	DeletePoly(&Head2);
	DeletePoly(&Sum);
	DeletePoly(&Prod);

	return 0;
}

int StrtoPoly(Polynomial * Head, char *buffer)
{
	int i = 0, size = 0;
	Polynomial* temp = NULL;
	while (strlen(buffer) > 0)
	{
		temp = (Polynomial*)malloc(sizeof(Polynomial));
		if (temp == NULL)
			return MEMORY_ERROR;
		if(sscanf(buffer, "%d %d %n", &temp->coefficient, &temp->exponent, &size) != 2)
		{
			free(temp);
			break;
		}
		AddPoly(Head, temp);
		buffer += size;
	}
	return 0;
}
int AddPoly(Polynomial* Head, Polynomial* newPoly)
{
	Polynomial* prev = Head;
	Polynomial* temp = NULL;

	if (newPoly->coefficient == 0)
	{
		free(newPoly);
		return 0;
	}
	while (prev->next != NULL && prev->next->exponent > newPoly->exponent)
	{
		prev = prev->next;
	}
	if (prev->next != NULL && newPoly->exponent == prev->next->exponent)
	{
		prev->next->coefficient += newPoly->coefficient;
		free(newPoly);
		if (prev->next->coefficient == 0)
		{
			temp = prev->next;
			prev->next = prev->next->next;
			free(temp);
		}
	}
	else
	{
		newPoly->next = prev->next;
		prev->next = newPoly;
	}
	return 0;
}
int PrintTerm(Polynomial* term)
{
	term->coefficient > 0 ? printf("%d ", term->coefficient) : printf("- %d", -term->coefficient);
	if(term->exponent ==1) printf("x ");
	else if (term->exponent != 0) printf("x^%d ", term->exponent);

	return 0;
}
int PrintPoly(Polynomial* Head)
{
	Polynomial* temp = Head;
	if(temp != NULL)
	{
		PrintTerm(temp);
		temp = temp->next;
	}
	while (temp != NULL)
	{
		if (temp->coefficient > 0)printf("+ ");
		   PrintTerm(temp);
		   temp = temp->next;
	}
	printf("\n");
	return 0;
}
int SumPoly(Polynomial* Head1, Polynomial* Head2, Polynomial* Sum)
{
	Polynomial* temp =	NULL;
	Polynomial* curr1 = Head1->next;
	Polynomial* curr2 = Head2->next;
	while (curr1 != NULL)
	{
		temp = (Polynomial*)malloc(sizeof(Polynomial));
		if (temp == NULL)
			return MEMORY_ERROR;
		temp->coefficient = curr1->coefficient;
		temp->exponent = curr1->exponent;
		temp->next = NULL;
		AddPoly(Sum, temp);
		curr1 = curr1->next;
	}
	while (curr2 != NULL)
	{
		temp = (Polynomial*)malloc(sizeof(Polynomial));
		if (temp == NULL)
			return MEMORY_ERROR;
		temp->coefficient = curr2->coefficient;
		temp->exponent = curr2->exponent;
		temp->next = NULL;
		AddPoly(Sum, temp);
		curr2 = curr2->next;
	}

	return 0;
}
int PolyProd(Polynomial* Head1, Polynomial* Head2, Polynomial* Prod)
{
	Polynomial* temp = NULL;
	Polynomial* curr1 = Head1;
	Polynomial* curr2 = Head2;

	while (curr1 != NULL)
	{
		curr2= Head2->next;
		while (curr2 != NULL)
		{
			temp = (Polynomial*)malloc(sizeof(Polynomial));
			if (temp == NULL) return MEMORY_ERROR;

			temp->coefficient = curr1->coefficient * curr2->coefficient;
			temp->exponent = curr1->exponent + curr2->exponent;

			AddPoly(Prod, temp);
			curr2 = curr2->next;
		}
		curr1 = curr1->next;
	}
	return 0;
}
int DeletePoly(Polynomial* Head)
{
	Polynomial* temp = NULL;
	Polynomial* curr = Head->next;
	while (curr != NULL)
	{
		temp = curr;
	    curr = curr->next;
		free(temp);
	}
	Head->next = NULL;
	return 0;
}