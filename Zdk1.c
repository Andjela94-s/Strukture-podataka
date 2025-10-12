#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<stdlib.h>

#define MAX_SIZE 50
#define FILE_ERROR_OPEN -1
#define MAX_LINE 1024
#define MAX_POINTS 60
typedef struct Students{
	char name[MAX_SIZE];
	char surname[MAX_SIZE];
	double aps_points;
}_students;

double relative_points(_students*, int);

int main()
{
	_students* s;
	int n=0;
	FILE* fp;
	fp = fopen("Studenti.txt", "r");	//otvaranje datoteke

	if (fp == NULL) {
		printf("Greska pri otvaranju datoteke.");		//provjera je li otvorena datoteka
		return FILE_ERROR_OPEN;
	}
	char buffer[MAX_LINE] = { 0 };
	while (fgets(buffer, sizeof(buffer), fp) != NULL)
		n++;

	s = (_students*)malloc(n * sizeof(_students));		//dinamička alokacija strukture student

	    rewind(fp);

	while (!feof(fp)) {
		for (int i = 0;i < n;i++) {
			fscanf(fp, "%s\t%s\t%lf", s[i].name, s[i].surname, &s[i].aps_points);
		}
	}
	printf("IME\tPREZIME\tAPS_BODOVI\tREL_BODOVI\n");
	for (int i = 0;i <n;i++) {

		printf("%s\t%s\t%.2lf\t%.2lf\n",s[i].name, s[i].surname, s[i].aps_points,relative_points(s,i+1));
	}

	free(s);
	fclose(fp);

	return 0;
}

double relative_points(_students *s, int n)
{
	double rel_poin=0.0;
	for (int i = 0;i < n;i++) {
		rel_poin = (s[i].aps_points / MAX_POINTS) * 100;
	}
		return rel_poin;
}