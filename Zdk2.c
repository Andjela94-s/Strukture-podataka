#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE 1024
#define MAX_SIZE 50
#define MALLOC_ERROR -1

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
Position FindThePerson(Position, char*);
int DeleteFromTheList(Position, char*);
Position CreatePerson();
int deleteList(Position);
int main()
{
	char op;
	_person Head = { .name = " ",.surname = " ",.yob = 0,.next = NULL };			 //inicijalizacija početka liste
	char surname[MAX_SIZE];														 //varijabla za prezime koje se traži u listi
	printf("a-add to front of the list\nb-add to back of the list\np-print the list\nf-find the person\nd-delete the person\nq-exit the program\n");
	
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
		case 'p':
			PrintTheList(Head.next);					//poziv funkcije za ispis liste
			break;
		case 'f':
			printf("Enter surname to find: ");			//unos prezimena koje se traži
			scanf_s("%s", surname, MAX_SIZE);
			Position found = FindThePerson(Head.next, surname);	//poziv funkcije za pronalazak osobe u listi
			if (found != NULL) {												//ispis ako je osoba pronađena
				printf("Person found: %s %s %d\n", found->name, found->surname, found->yob);
			}
			else {																	//ispis ako osoba nije pronađena
				printf("Person with surname %s not found.\n", surname);
			}
			break;
		case 'd':
			printf("Enter surname to delete: ");			//unos prezimena koje se briše
			scanf_s("%s", surname, MAX_SIZE);
			DeleteFromTheList(&Head, surname);			//poziv funkcije za brisanje osobe iz liste
			break;
		default:
			printf("Invalid option.\n");					//ispis ako je unesena nevažeća opcija
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

	printf("Enter name: ");										//unos imena
	scanf_s("%s", person->name,MAX_SIZE);

	printf("Enter surname: ");									//unos prezimena
	scanf_s("%s", person->surname,MAX_SIZE);

	printf("Enter year of birth: ");								//unos godine rodjenja	
	scanf_s("%d", &person->yob);

	return person;
}

int AddToFrontofTheList(Position Head)
{
	Position newPerson = CreatePerson();				//stvaranje nove osobe
	if (!newPerson) return MALLOC_ERROR;			//provjera je li došlo do greške alokacije

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
	if (!newPersonQ) return MALLOC_ERROR;						//provjera je li došlo do greške alokacije
	while (Head->next != NULL) {
		Head= Head->next;												//petlja ide do kraja liste
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
			free(temp);                                             // oslobađanje memorije
			printf("Person with surname %s deleted.\n", surname); // ispis ako je osoba obrisana
		 } 
		 else printf("Person with surname %s not found.\n", surname); // ispis ako osoba nije pronađena
	 return 0;
}

int deleteList(Position Head)
{
	Position temp = NULL;
	while (Head->next != NULL) {					//petlja ide kroz listu i briše svaki član
		temp = Head->next;							
		Head->next = temp->next;					
		free(temp);									
	}
	return 0;
}
