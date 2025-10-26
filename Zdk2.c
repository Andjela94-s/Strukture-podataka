#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE 1024
#define MAX_SIZE 50
#define MALLOC_ERROR -1
#define FILE_ERROR -2

struct Person;
typedef struct Person* Position;				//deklaracija strukture i njene pozicije
typedef struct Person {
	char name[MAX_SIZE];
	char surname[MAX_SIZE];
	int yob;
	Position next;
}_person;
																			//deklaracija funkcija
int AddToFrontofTheList(Position);
int AddToBackofTheList(Position);
int PrintTheList(Position);
int AddafterLastname(Position, char*);
int AddbeforeLastname(Position, char*);
int SortList(Position);
int AddtoFile(Position);
int ReadFromFile(Position);

Position FindThePerson(Position, char*);
int DeleteFromTheList(Position, char*);
Position CreatePerson();
int deleteList(Position);
int main()
{
	char op;
	_person Head = { .name = " ",.surname = " ",.yob = 0,.next = NULL };			 //inicijalizacija početka liste
	char surname[MAX_SIZE];														 //varijabla za prezime koje se traži u listi
	printf("a-add to front of the list\nb-add to back of the list\nc-add before the person\ne-add after the person\np-print the list\nf-find the person\nd-delete the person\ns-sort the list\nw-write into a file\nr-read from the list\nq-exit the program\n");
	
	do {
		scanf(" %c", &op);					//unos opcije
		getchar();									//čišćenje buffera

		switch (op)
		{
		case 'a':
			AddToFrontofTheList(&Head);				//poziv funkcije za dodavanje na početak liste
			break;
		case 'b':
			AddToBackofTheList(&Head);				//poziv funkcije za dodavanje na kraj liste
			break;
		case 'c':
						printf("Enter surname to add before: ");	//unos prezimena prije kojeg se dodaje nova osoba
			scanf_s("%s", surname, MAX_SIZE);
			AddbeforeLastname(Head.next, surname);	
			break;
		case 'e':
			printf("Enter surname to add after: ");		//unos prezimena nakon kojeg se dodaje nova osoba
			scanf_s("%s", surname, MAX_SIZE);
			AddafterLastname(Head.next, surname);		
			break;
		case 's':
			SortList(&Head);							
			break;
		case 'w':
			AddtoFile(Head.next);						
			break;
		case 'r':
			readFromFile(&Head);						
			break;
		case 'p':
			PrintTheList(Head.next);					
			break;
		case 'f':
			printf("Enter surname to find: ");			//unos prezimena koje se traži
			scanf_s("%s", surname, MAX_SIZE);
			Position found = FindThePerson(Head.next, surname);		
			if (found != NULL) {																	
				printf("Person found: %s %s %d\n", found->name, found->surname, found->yob);
			}
			else {																
				printf("Person with surname %s not found.\n", surname);
			}
			break;
		case 'd':
			printf("Enter surname to delete: ");			//unos prezimena koje se briše
			scanf_s("%s", surname, MAX_SIZE);
			DeleteFromTheList(&Head, surname);			
			break;
		default:
			printf("Invalid option.\n");					//ispis ako je unesena neispravna operacija
			break;
		}
	} while (op != 'q');								//petlja se vrti dok se ne unese opcija za izlaz iz programa

	deleteList(&Head);									//poziv funkcije za brisanje cijele liste prije izlaza iz programa

	return 0;
}

Position CreatePerson()
{
	Position person = NULL;									//alokacija memorije za novu osobu
	person = (Position)malloc(1*sizeof(_person));
	if(!person) return NULL;

	printf("Enter name: ");										
	scanf_s("%s", person->name,MAX_SIZE);

	printf("Enter surname: ");									
	scanf_s("%s", person->surname,MAX_SIZE);

	printf("Enter year of birth: ");							
	scanf_s("%d", &person->yob);

	return person;
}

int AddToFrontofTheList(Position Head)
{
	Position newPerson = CreatePerson();				//stvaranje nove osobe
	if (!newPerson) return MALLOC_ERROR;			

	newPerson->next = Head->next;					//spajanje nove osobe s prvim članom liste
	Head->next = newPerson;								//postavljanje nove osobe kao prvi element liste

	return 0;
}

int PrintTheList(Position Head)
{ 
	while (Head != NULL) {					//petlja ide kroz listu dok sve članove ne ispiše
		printf("%s %s %d\n", Head->name, Head->surname, Head->yob);
		Head = Head->next;	
   }
	return 0;
}

int AddToBackofTheList(Position Head)
{
	Position newPersonQ = CreatePerson();							//stvaranje nove osobe
	if (!newPersonQ) return MALLOC_ERROR;				
	while (Head->next != NULL) {
		Head= Head->next;										
	}
	newPersonQ->next = Head->next;					//spajanje nove osobe na kraj liste		
	Head->next = newPersonQ;

	return 0;
}

Position FindThePerson(Position Current, char *Surname)
{
	while(Current !=NULL && strcmp(Current->surname, Surname))		//petlja ide kroz listu dok ne nađe traženo prezime
		Current = Current->next;	
		return Current;
}
Position findPersonPrev(Position current, char* sur)
{
	while (current->next != NULL && strcmp(current->next->surname, sur))
		current = current->next;

	if (current->next == NULL) return NULL;

	return current;
}

int DeleteFromTheList(Position Current, char* surname)
{
    Position temp = NULL, prev=NULL;

	prev = findPersonPrev(Current, surname);

		if(prev != NULL){
			temp = prev->next;                                   // spremanje pokazivača na osobu koja se briše
			prev->next = temp->next;                             // preskakanje osobe koja se briše u listi
			free(temp);                                           
			printf("Person with surname %s deleted.\n", surname); 
		 } 
		 else printf("Person with surname %s not found.\n", surname); 
	 return 0;
}

int deleteList(Position Head)
{
	Position temp = NULL;
	while (Head->next != NULL) {					//oslobađanje memorije tj. brisanje liste
		temp = Head->next;							
		Head->next = temp->next;					
		free(temp);									
	}
	return 0;
}
int AddafterLastname(Position Head, char* surname)
{
	Position newPerson = CreatePerson();				//stvaranje nove osobe
	if (!newPerson) return MALLOC_ERROR;
	Position found = FindThePerson(Head, surname);		//traženje prezimena nakon kojeg se dodaje nova osoba
	if (found != NULL) {
		newPerson->next = found->next;					//spajanje nove osobe nakon pronađene osobe
		found->next = newPerson;
	}
	else {
		printf("Person with surname %s not found.\n", surname);
		free(newPerson);									
	}
	return 0;
}
int AddbeforeLastname(Position Head, char* surname)
{
	Position newPerson = CreatePerson();				//stvaranje nove osobe
	if (!newPerson) return MALLOC_ERROR;
	Position prev = findPersonPrev(Head, surname);		//traženje prezimena prije kojeg se dodaje nova osoba
	if (prev != NULL) {
		newPerson->next = prev->next;					//spajanje nove osobe prije pronađene osobe
		prev->next = newPerson;
	}
	else {
		printf("Person with surname %s not found.\n", surname);
		free(newPerson);									
	}
	return 0;
}
int SortList(Position Head)
{
	Position q, p, sorted, temp;
	q = Head;
	p = Head->next;
	sorted = NULL;
	temp = NULL;
	while (p->next != sorted) {
		if (strcmp(p->surname, p->next->surname) > 0)			//uspoređivanje prezimena
		{
			temp = p->next;														
			p->next = temp->next;
			temp->next = p;
			if (p == q->next) {
				q->next = temp;
			}
			else {
				Position r = q;
				while (r->next != p) {
					r = r->next;
				}
				r->next = temp;
			}
			p = q->next;
		}
		else {
			p = p->next;
		}
	}
		printf("Sorted list:\n");
		PrintTheList(Head->next);
	
	return 0;
}
int AddtoFile(Position Head)
{
	FILE* fp = NULL;
	fp = fopen("persons.txt", "w");					//otvaranje datoteke za pisanje
	if (!fp) {
		printf("Error opening file.\n");
		return FILE_ERROR;
	}
	while (Head != NULL) {							//petlja ide kroz listu i upisuje sve članove u datoteku
		fprintf(fp, "%s %s %d\n", Head->name, Head->surname, Head->yob);
		Head = Head->next;
	}
	fclose(fp);										//zatvaranje datoteke
	return 0;
}
int readFromFile(Position Head)
{
	FILE* fp = NULL;
	fp = fopen("persons.txt", "r");					//otvaranje datoteke za čitanje
	if (!fp) {
		printf("Error opening file.\n");
		return FILE_ERROR;
	}
	while (!feof(fp)) {								//petlja ide kroz datoteku i čita sve članove u listu
		Position newPerson = (Position)malloc(1 * sizeof(_person));
		if (!newPerson) return MALLOC_ERROR;
		fscanf(fp, "%s %s %d\n", newPerson->name, newPerson->surname, &newPerson->yob);
		newPerson->next = Head->next;				//spajanje nove osobe na početak liste
		Head->next = newPerson;
	}
	fclose(fp);										//zatvaranje datoteke
	return 0;
}