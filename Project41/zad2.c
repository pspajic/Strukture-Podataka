/*Definirati strukturu osoba (ime, prezime, godina rodjenja) i napisati program koji:
 a) dinamicki dodaje novi element na pocetak liste,
 b) ispisuje listu,
 c) dinami�ki dodaje novi element na kraj liste,
 d) pronalazi element u listi (po prezimenu),
 e) bri�e odre�eni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.
*/

#define CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

typedef struct _osoba* position;
typedef struct _osoba {
	char	ime[50];
	char	prezime[50];
	int		godina;

	position next;
}osoba;

position nova_osoba(char* ime, char* prezime, int godina);
int dodaj_na_pocetak(position head, char* ime, char* prezime, int godina);
int dodaj_na_kraj(position head, char* ime, char* prezime, int godina);
void ispis(position first);
position pronadi_zadnji(position head);
position pronadi_po_prezimenu(position first, char* prezime);
int izbrisi_prvi(position head);

int main()
{
	osoba head = { .ime = "", .prezime = "", .godina = 0, .next = NULL }; // postavimo clanove heada

	dodaj_na_pocetak(&head, "Mate", "Matic", 1997);
	dodaj_na_pocetak(&head, "Luka", "Lukic", 2001);
	dodaj_na_pocetak(&head, "Ivo", "Ivic", 2002);
	dodaj_na_kraj(&head, "Ana", "Anic", 2003);

	//ispis liste
	printf("Ispis svih osoba na listi:\n");
	ispis(head.next);

	// pretraga po prezimenu
	position trazenaOsoba = pronadi_po_prezimenu(head.next, "Ivic");
	if (trazenaOsoba)
		printf("\nPronadjena osoba: %s %s, rodjena %d\n\n", trazenaOsoba->ime, trazenaOsoba->prezime, trazenaOsoba->godina);
	else
		printf("\nTrazena osoba se prezimenom Ivic nije pronadjena.\n\n");

	// brisanje prvog
	izbrisi_prvi(&head);
	printf("Lista nakon brisanja prve osobe:\n");
	ispis(head.next);
	
	return 0;
}

position nova_osoba(char* ime, char* prezime, int godina)
{
	position  novaOsoba = NULL; // postavimo varijablu novaOsoba na NULL

	novaOsoba = (position*)malloc(sizeof(osoba)); // memorija za novu osobu

	if (!novaOsoba) {
		printf("Malloc nije bio uspjesan u funkciji nova_osoba()!\n");  // ako se ne ne alocira memorija za novu osobu ispise da nije usjelo
	}

	//nova osoba
	strcpy(novaOsoba->ime, ime);
	strcpy(novaOsoba->prezime, prezime);
	novaOsoba->godina = godina;
	novaOsoba->next = NULL;

	//vracamo novu osobu
	return novaOsoba;
}

int dodaj_na_pocetak(position head, char* ime, char* prezime, int godina)
{
	position novaOsoba = NULL; //postavimo varijablu na NULL

	novaOsoba = nova_osoba(ime, prezime, godina); //pozivamo funciju nova_osoba() 

	if (!novaOsoba)
	{
		return -1; //ako je nova osoba NULL vrati se -1
	}

	novaOsoba->next = head->next;
	head->next = novaOsoba;

	return EXIT_SUCCESS;
}

int dodaj_na_kraj(position head, char* ime, char* prezime, int godina)
{
	position novaOsoba = NULL;
	position posljednji = NULL;

	novaOsoba = nova_osoba(ime, prezime, godina);

	if (!novaOsoba)
		return -1;

	posljednji = pronadi_zadnji(head);
	posljednji->next=novaOsoba;

	return EXIT_SUCCESS;
}

void ispis(position first)
{
	position temp = first; // postavimo temp na prvi clan liste

	//ispis clan po clan
	while (temp) {
		printf("Ime: %s \nPrezime: %s \nGodina rodjenja: %d\n\n", temp->ime, temp->prezime, temp->godina);
		temp = temp->next;
	}

}

position pronadi_zadnji(position head)
{
	position temp = head;

	//petlja se vrti dok postoji sljedeci clan
	while (temp->next) {
		temp = temp->next;
	}

	//vraca zadnji clan
	return temp;
}

position pronadi_po_prezimenu(position first, char* prezime)
{
	position temp = first;

	// petlja se vrsi dok ne pronadje prezime tada vraca tu osobu s return temp, a ako ne pronadje vrati null
	while (temp) {
		if (strcmp(temp->prezime, prezime) == 0) {
			return temp;
		}
		temp = temp->next;
	}
	return NULL;
}

int izbrisi_prvi(position head)
{
	if (!head->next) {
		printf("Lista je prazna, nema elemenata za brisanje!");
		return -1;
	}

	position izbrisati = head->next;
	head->next = izbrisati->next;

	free(izbrisati);

	return 0;
}