#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

#define EXIT_SUCCESS 0
#define FILE_OPEN_ERROR -1
#define MALLOC_ERROR -2
#define SCANF_ERROR -3

struct article;
typedef struct article* ArticlePosition;
typedef struct article {
	char name[20];
	int quantity;
	int price;
	ArticlePosition next;
} Article;

struct receipt;
typedef struct receipt* ReceiptPosition;
typedef struct receipt {
	char date[10];
	Article headArticle;
	ReceiptPosition next;
} Receipt;

int addArticle(ArticlePosition head, ArticlePosition article);
int addReceipt(ReceiptPosition head, ReceiptPosition receipt);
int readReceipt(char* fileName, ReceiptPosition headReceipt);
int printArticles(ArticlePosition article);
int printReceipts(ReceiptPosition receipt);
int getArticleInPeriod(ReceiptPosition currentReceipt, char* article, char* startDate, char* endDate, int* income, int* count);
int LowestPriceArticle(ReceiptPosition headreceipt, char* articlename, float* lowestprice);
int HighestPriceArticle(ReceiptPosition headreceipt, char* articlename, float* highestprice);
int MostFrequentArticle(ReceiptPosition headreceipt, char* articlename);
int MostExpensiveReceipt(ReceiptPosition headreceipt, char* date, float* highestprice);
int MostAffordableReceipt(ReceiptPosition headreceipt, char* date, float* lowestprice);
int deleteArticles(ArticlePosition articleHead);
int deleteReceipts(ReceiptPosition receiptHead);

int main()
{
	int income = 0, count = 0;
	char article[100] = { 0 };
	char startDate[20] = { 0 }, endDate[20] = { 0 };
	char currReceipt[MAX_LINE] = { 0 };
	FILE* filePointer = NULL;
	Receipt headReceipt = {
		.date = {0},
		.headArticle = NULL,
		.next = NULL
	};

	filePointer = fopen("receipts.txt", "r");
	if (filePointer == NULL) {
		printf("File could not be opened.\n");
		return FILE_OPEN_ERROR;
	}

	while (!feof(filePointer)) {
		fgets(currReceipt, MAX_LINE, filePointer);
		currReceipt[strcspn(currReceipt, "\n")] = 0; // Removes '\n'
		printf("%s\n", currReceipt);
		readReceipt(currReceipt, &headReceipt);
	}

	printReceipts(headReceipt.next);

	char option;
	printf("Enter what do you want:\n");
	printf("a) Get article income in period\n");
	printf("b) Get lowest and highest price of article\n");
	printf("c) Get most frequent article\n");
	printf("d) Get most expensive and most affordable receipt\n");
	printf("e) Exit\n");

	option = getchar();
	switch (option)
	{
	case 'a':
		printf("\nEnter article name: ");
		if (scanf("%s", article) != 1) return SCANF_ERROR;
		printf("Enter start date (YYYY-MM-DD): ");
		if (scanf("%s", startDate) != 1) return SCANF_ERROR;
		printf("Enter end date (YYYY-MM-DD): ");
		if (scanf("%s", endDate) != 1) return SCANF_ERROR;
		getArticleInPeriod(headReceipt.next, article, startDate, endDate, &income, &count);
		printf("Income from article %s between %s and %s is %d, count is %d\n", article, startDate, endDate, income, count);
		break;
	case 'b':
		printf("\nEnter article name: ");
		if (scanf("%s", article) != 1) return SCANF_ERROR;
		float lowestprice = -1.0f, highestprice = -1.0f;
		LowestPriceArticle(headReceipt.next, article, &lowestprice);
		HighestPriceArticle(headReceipt.next, article, &highestprice);
		if (lowestprice < 0) printf("Article %s not found in any receipt.\n", article);
		else
			printf("Lowest price of article %s is %.2f, highest price is %.2f\n", article, lowestprice, highestprice);
		break;
	case 'c':
		printf("\nEnter article name: ");
		if (scanf("%s", article) != 1) return SCANF_ERROR;
		int frequency = MostFrequentArticle(headReceipt.next, article);
		printf("Most frequent article %s appears %d times\n", article, frequency);
		break;
	case 'd':
		printf("\nEnter date (YYYY-MM-DD): ");
		if (scanf("%s", startDate) != 1) return SCANF_ERROR;
		float mostexpensive = -1.0f, mostaffordable = -1.0f;
		MostExpensiveReceipt(headReceipt.next, startDate, &mostexpensive);
		MostAffordableReceipt(headReceipt.next, startDate, &mostaffordable);
		if (mostexpensive < 0) {
			printf("No expensive receipt found on %s.\n", startDate);
		}
		else if (mostaffordable < 0) {
			printf("No affordable receipt found on %s.\n", startDate);
		}
		else {
			printf("Most expensive receipt on %s is %.2f, most affordable is %.2f\n", startDate, mostexpensive, mostaffordable);
		}
		break;
	case 'e':
		printf("Exiting program.\n");
		break;
	default:
		printf("Invalid option!\n");
		break;
	}

	deleteReceipts(&headReceipt);
	fclose(filePointer);

	return 0;
}

int addArticle(ArticlePosition head, ArticlePosition article)
{
	ArticlePosition current = head;

	while (current->next != NULL && strcmp(current->next->name, article->name) == -1)
		current = current->next;

	article->next = current->next;
	current->next = article;

	return 0;
}

int addReceipt(ReceiptPosition head, ReceiptPosition receipt)
{
	ReceiptPosition current = head;

	while (current->next != NULL && strcmp(current->next->date, receipt->date) <= 0)
		current = current->next;

	receipt->next = current->next;
	current->next = receipt;

	return 0;
}

int readReceipt(char* fileName, ReceiptPosition headReceipt)
{
	char dateString[20] = { 0 };
	char articleString[MAX_LINE] = { 0 };
	int count = 0;
	FILE* filePointer = NULL;
	ReceiptPosition newReceipt = NULL;
	ArticlePosition newArticle = NULL;

	filePointer = fopen(fileName, "r");
	if (filePointer == NULL) {
		printf("File could not be opened.\n");
		return FILE_OPEN_ERROR;
	}

	newReceipt = (ReceiptPosition)malloc(sizeof(Receipt));
	if (newReceipt == NULL) {
		printf("Malloc error.\n");
		return MALLOC_ERROR;
	}

	fgets(dateString, 20, filePointer);
	dateString[strcspn(dateString, "\n")] = 0;

	strcpy(newReceipt->date, dateString);
	newReceipt->headArticle.next = NULL;

	while (!feof(filePointer))
	{
		newArticle = (ArticlePosition)malloc(sizeof(Article));
		if (newArticle == NULL) {
			printf("Malloc error.\n");
			return MALLOC_ERROR;
		}

		if (fscanf(filePointer, "%[^,], %d, %d ",
			newArticle->name, &newArticle->quantity, &newArticle->price) != 3) {
			printf("fscanf error.\n");
			return SCANF_ERROR;
		}

		addArticle(&newReceipt->headArticle, newArticle);
	}

	addReceipt(headReceipt, newReceipt);

	fclose(filePointer);

	return 0;
}

int printArticles(ArticlePosition current)
{
	while (current != NULL) {
		printf("\t%s, %d, %d\n", current->name, current->quantity, current->price);
		current = current->next;
	}

	return 0;
}

int printReceipts(ReceiptPosition current)
{
	while (current != NULL) {
		printf("Receipt from date %s:\n", current->date);
		printArticles(current->headArticle.next);
		current = current->next;
	}

	return 0;
}

int getArticleInPeriod(ReceiptPosition currentReceipt, char* article, char* startDate, char* endDate, int* income, int* count)
{
	ArticlePosition currentArticle = NULL;

	// Move to first receipt with date equal to or greater than startDate
	while (currentReceipt != NULL && strcmp(currentReceipt->date, startDate) == -1)
		currentReceipt = currentReceipt->next;

	while (currentReceipt != NULL && strcmp(currentReceipt->date, endDate) <= 0) {
		currentArticle = currentReceipt->headArticle.next;
		while (currentArticle != NULL) {
			if (strcmp(currentArticle->name, article) == 0) {
				*income += currentArticle->quantity * currentArticle->price;
				*count += currentArticle->quantity;
			}
			currentArticle = currentArticle->next;
		}
		currentReceipt = currentReceipt->next;
	}

	return EXIT_SUCCESS;
}

int LowestPriceArticle(ReceiptPosition headreceipt, char* articlename, float* lowestprice) {
	ArticlePosition currArticle = NULL;
	ReceiptPosition currReceipt = headreceipt;
	*lowestprice = -1.0f;
	while (currReceipt != NULL) {
		currArticle = currReceipt->headArticle.next;
		while (currArticle != NULL) {
			if (strcmp(currArticle->name, articlename) == 0) {
				if (*lowestprice < 0 || currArticle->price < *lowestprice) {
					*lowestprice = currArticle->price;
				}
			}
			currArticle = currArticle->next;
		}
		currReceipt = currReceipt->next;
	}
	return EXIT_SUCCESS;
}

int HighestPriceArticle(ReceiptPosition headreceipt, char* articlename, float* highestprice) {
	ArticlePosition currArticle = NULL;
	ReceiptPosition currReceipt = headreceipt;
	*highestprice = -1.0f;
	while (currReceipt != NULL) {
		currArticle = currReceipt->headArticle.next;
		while (currArticle != NULL) {
			if (strcmp(currArticle->name, articlename) == 0) {
				if (*highestprice < 0 || currArticle->price > *highestprice) {
					*highestprice = currArticle->price;
				}
			}
			currArticle = currArticle->next;
		}
		currReceipt = currReceipt->next;
	}
	return EXIT_SUCCESS;
}

int MostFrequentArticle(ReceiptPosition headreceipt, char* articlename) {
	ArticlePosition currArticle = NULL;
	ReceiptPosition currReceipt = headreceipt;
	int frequency = 0;
	while (currReceipt != NULL) {
		currArticle = currReceipt->headArticle.next;
		while (currArticle != NULL) {
			if (strcmp(currArticle->name, articlename) == 0) {
				frequency += currArticle->quantity;
			}
			currArticle = currArticle->next;
		}
		currReceipt = currReceipt->next;
	}
	return frequency;
}

int MostExpensiveReceipt(ReceiptPosition headreceipt, char* date, float* highestprice) {
	ReceiptPosition currReceipt = headreceipt;
	*highestprice = 0.0f;
	while (currReceipt != NULL) {
		if (strcmp(currReceipt->date, date) == 0) {
			float total = 0.0f;
			ArticlePosition currArticle = currReceipt->headArticle.next;
			while (currArticle != NULL) {
				total += currArticle->quantity * currArticle->price;
				currArticle = currArticle->next;
			}
			if (total > *highestprice) {
				*highestprice = total;
			}
		}
		currReceipt = currReceipt->next;
	}
	return EXIT_SUCCESS;
}

int MostAffordableReceipt(ReceiptPosition headreceipt, char* date, float* lowestprice) {
	ReceiptPosition currReceipt = headreceipt;
	*lowestprice = -1.0f;
	while (currReceipt != NULL) {
		if (strcmp(currReceipt->date, date) == 0) {
			float total = 0.0f;
			ArticlePosition currArticle = currReceipt->headArticle.next;
			while (currArticle != NULL) {
				total += currArticle->quantity * currArticle->price;
				currArticle = currArticle->next;
			}
			if (*lowestprice < 0 || total < *lowestprice) {
				*lowestprice = total;
			}
		}
		currReceipt = currReceipt->next;
	}
	return EXIT_SUCCESS;
}

int deleteArticles(ArticlePosition articleHead)
{
	ArticlePosition temp = NULL;

	while (articleHead->next != NULL) {
		temp = articleHead->next;
		articleHead->next = temp->next;
		free(temp);
	}

	return 0;
}
int deleteReceipts(ReceiptPosition receiptHead)
{
	ReceiptPosition temp = NULL;

	while (receiptHead->next != NULL) {
		temp = receiptHead->next;
		receiptHead->next = temp->next;
		deleteArticles(&temp->headArticle);
		free(temp);
	}

	return 0;
}

