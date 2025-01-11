/*
Napisati program koji cita datoteku drzave.txt u kojoj su zapisani nazivi pojedinih drzava. Uz
ime drzave u datoteci se nalazi i ime dodatne datoteke u kojoj se nalaze gradovi pojedine
drzave. Svaka datoteka koja predstavlja drzavu sadrzi popis gradova u formatu naziv_grada,
broj_stanovnika.
a) Potrebno je formirati sortiranu vezanu listu drzava po nazivu. Svaki cvor vezane liste
sadrzi stablo gradova sortirano po broju stanovnika, zatim po nazivu grada.
b) Potrebno je formirati stablo drzava sortirano po nazivu. Svaki cvor stabla sadrzi vezanu
listu gradova sortiranu po broju stanovnika, zatim po nazivu grada.
Nakon formiranja podataka potrebno je ispisati drzave i gradove te omoguæiti korisniku putem
tastature pretragu gradova odredjene drzave koji imaju broj stanovnika veci od unosa na
tastaturi.
*/

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define EXIT_SUCCESS 0
#define FILE_OPEN_ERROR -1
#define MALLOC_ERROR -2
#define SCANF_ERROR -3

//struktura za stablo gradova
struct grad;
typedef struct grad* PozicijaGrada;
typedef struct grad {
	char ime[50];
	int brojStanovnika;
	PozicijaGrada lijevo;
	PozicijaGrada desno;
}Grad;

struct drzava;
typedef struct drzava* PozicijaDrzave;
typedef struct drzava {
	char ime[50];
	PozicijaGrada gradovi;
	PozicijaDrzave sljedeci;
}Drzava;

//funkcije za rad sa stablima gradova
PozicijaGrada dodajGrad(PozicijaGrada trenutni, PozicijaGrada nooviGrad);
int ispisiGradove(PozicijaGrada trenutni);
int ispisiGradoveVeceOd(PozicijaGrada trenutni, int granicaBrojStanovnika);

//funkcije za rad s listom drzava
int dodajDrzavu(PozicijaDrzave glava, PozicijaDrzave novaDrzava);
int ucitajDrzaveIzDatoteke(PozicijaDrzave glava, const char* imeDatoteke);
int ispisiDrzave(PozicijaDrzave glava);
PozicijaDrzave pronadiDrzavu(PozicijaDrzave glava, const char* imeDrzave);

int main()
{
	Drzava glava = { .ime = {0}, .gradovi = NULL, .sljedeci = NULL };
	char imeDrzave[50] = { 0 };
	int granicaBrojStanovnika = 0;

	if (ucitajDrzaveIzDatoteke(&glava, "drzave.txt") != EXIT_SUCCESS) {
		printf("Pogreska pri ucitavanju drzava!\n");
		return FILE_OPEN_ERROR;
	}

	printf("Popis drzava i gradova:\n");
	ispisiDrzave(glava.sljedeci);

	printf("\nUnesite ime drzave za pretragu: ");
	scanf("%49s", imeDrzave);

	PozicijaDrzave trazenaDrzava = pronadiDrzavu(&glava, imeDrzave);
	if (!trazenaDrzava) {
		printf("Drzava nije pronadjena!\n");
		return EXIT_SUCCESS;
	}

	printf("Unesite minimalni broj stanovnika: ");
	scanf("%d", &granicaBrojStanovnika);

	printf("Gradovi u drzavi %s vise od %d stanovnika:\n", imeDrzave, granicaBrojStanovnika);
	ispisiGradoveVeceOd(trazenaDrzava->gradovi, granicaBrojStanovnika);

	return EXIT_SUCCESS;
}

PozicijaGrada dodajGrad(PozicijaGrada trenutni, PozicijaGrada noviGrad) 
{
	if (!trenutni) return noviGrad;

	if (noviGrad->brojStanovnika < trenutni->brojStanovnika) {
		trenutni->lijevo = dodajGrad(trenutni->lijevo, noviGrad);
	}
	else if (noviGrad->brojStanovnika > trenutni->brojStanovnika) {
		trenutni->desno = dodajGrad(trenutni->desno, noviGrad);
	}
	else {
		if (strcmp(noviGrad->ime, trenutni->ime) < 0)
			trenutni->lijevo = dodajGrad(trenutni->lijevo, noviGrad);
		else
			trenutni->desno = dodajGrad(trenutni->desno, noviGrad);
	}

	return trenutni;
}

int ispisiGradove(PozicijaGrada trenutni)
{
	if (!trenutni) return EXIT_SUCCESS;
	ispisiGradove(trenutni->lijevo);
	printf("\t%s, %d stanovnika\n", trenutni->ime, trenutni->brojStanovnika);
	ispisiGradove(trenutni->desno);

	return EXIT_SUCCESS;
}

int ispisiGradoveVeceOd(PozicijaGrada trenutni, int granicaBrojStanovnika) 
{
	if (!trenutni) return EXIT_SUCCESS;

	if (trenutni->brojStanovnika > granicaBrojStanovnika)
		printf("\t%s, %d stanovnika\n", trenutni->ime, trenutni->brojStanovnika);

	ispisiGradoveVeceOd(trenutni->lijevo, granicaBrojStanovnika);
	ispisiGradoveVeceOd(trenutni->desno, granicaBrojStanovnika);

	return EXIT_SUCCESS;
}

int dodajDrzavu(PozicijaDrzave glava, PozicijaDrzave novaDrzava)
{
	while (glava->sljedeci && strcmp(glava->sljedeci->ime, novaDrzava->ime) < 0)
		glava = glava->sljedeci;

	novaDrzava->sljedeci = glava->sljedeci;
	glava->sljedeci = novaDrzava;

	return EXIT_SUCCESS;
}

int ucitajDrzaveIzDatoteke(PozicijaDrzave glava, const char* imeDatoteke)
{
	FILE* file = fopen(imeDatoteke, "r");
	if (!file) return FILE_OPEN_ERROR;

	char imeDrzave[50] = { 0 }, datotekaGradova[50] = { 0 };
	while (fscanf(file, " %49[^,], %49s", imeDrzave, datotekaGradova) == 2) {
		PozicijaDrzave novaDrzava = (PozicijaDrzave)malloc(sizeof(Drzava));
		if (!novaDrzava) return MALLOC_ERROR;

		strcpy(novaDrzava->ime, imeDrzave);
		novaDrzava->gradovi = NULL;
		novaDrzava->sljedeci = NULL;

		FILE* gradFile = fopen(datotekaGradova, "r");
		if (!gradFile) return FILE_OPEN_ERROR;

		char imeGrada[50] = { 0 };
		int brojStanovnika = 0;
		while (fscanf(gradFile, " %49[^,], %d", imeGrada, &brojStanovnika) == 2) {
			PozicijaGrada noviGrad = (PozicijaGrada)malloc(sizeof(Grad));
			if (!noviGrad) return MALLOC_ERROR;

			strcpy(noviGrad->ime, imeGrada);
			noviGrad->brojStanovnika = brojStanovnika;
			noviGrad->lijevo = noviGrad->desno = NULL;

			novaDrzava->gradovi = dodajGrad(novaDrzava->gradovi, noviGrad);
		}
		fclose(gradFile);

		dodajDrzavu(glava, novaDrzava);
	}
	fclose(file);

	return EXIT_SUCCESS;
}

int ispisiDrzave(PozicijaDrzave glava)
{
	while (glava) {
		printf("Drzava: %s\n", glava->ime);
		ispisiGradove(glava->gradovi);
		glava = glava->sljedeci;
	}

	return EXIT_SUCCESS;
}

PozicijaDrzave pronadiDrzavu(PozicijaDrzave glava, const char* imeDrzave)
{
	while (glava && strcmp(glava->ime, imeDrzave) != 0)
		glava = glava->sljedeci;

	return glava;
}