/*
Napisati program koji cita datoteku racuni.txt u kojoj su zapisani nazivi svih datoteka koji
predstavljaju pojedini racun. Na pocetku svake datoteke je zapisan datum u kojem vremenu je
racun izdat u formatu YYYY-MM-DD. Svaki sljedeæi red u datoteci predstavlja artikl u formatu
naziv, kolicina, cijena. Potrebno je formirati vezanu listu raèuna sortiranu po datumu. Svaki èvor
vezane liste sadržava vezanu listu artikala sortiranu po nazivu artikla. Nakon toga potrebno je
omoguciti upit kojim æe korisnik saznati koliko je novaca sveukupno potrošeno na specificni
artikl u odredjenom vremenskom razdoblju i u kojoj je kolicini isti kupljen.
*/

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define MAX_LINE 1024
#define USPJEH 0
#define POGRESKA_OTVARANJE_DATOTEKE -1
#define POGRESKA_MALLOC -2
#define POGRESKA_SCANF -3

struct clanak;
typedef struct clanak* PozicijaClanka;
typedef struct clanak {
	char ime[50];
	int kolicina;
	int cijena;

	PozicijaClanka sljedeci;
}Clanak;

struct racun;
typedef struct racun* PozicijaRacuna;
typedef struct racun {
	char datum[11];
	Clanak glavniClanak;

	PozicijaRacuna sljedeci;
}Racun;

int dodajClanak(PozicijaClanka glava, PozicijaClanka clanak);
int dodajRacun(PozicijaRacuna glava, PozicijaRacuna racun);
int procitajRacun(char* imeDatoteke, PozicijaRacuna glavaRacuna);
int ispisiClanke(PozicijaClanka clanak);
int ispisiRacune(PozicijaRacuna racun);
int dohvatiClanakURazdoblju(PozicijaRacuna trenutniRacun, char* clanak, char* pocetniDatum, char* zavrsniDatum, int* prihod, int* kolicina);
int obrisiClanke(PozicijaClanka glavaClanka);
int obrisiRacune(PozicijaRacuna glavaRacuna);
int usporediDatume(char* datum1, char* datum2);

int main()
{
	int prihod = 0, kolicina = 0;
	char clanak[50] = { 0 };
	char pocetniDatum[20] = { 0 }, zavrsniDatum[20] = { 0 };
	char trenutniRacun[MAX_LINE] = { 0 };
	FILE* pokazivacNaDatoteku = NULL;
	Racun glavaRacuna = { .datum = {0}, .glavniClanak = {0}, .sljedeci = NULL };

	pokazivacNaDatoteku = fopen("racuni.txt", "r");
	if (pokazivacNaDatoteku == NULL) {
		printf("Datoteka se nije mogla otvoriti!\n");
		return POGRESKA_OTVARANJE_DATOTEKE;
	}

	while (fgets(trenutniRacun, MAX_LINE, pokazivacNaDatoteku) != NULL) {
		trenutniRacun[strcspn(trenutniRacun, "\n")] = 0; //Ukloni \n
		printf("%s\n", trenutniRacun);
		procitajRacun(trenutniRacun, &glavaRacuna);
	}

	//Ispis svih racuna
	printf("\nIspis svih racuna:\n");
	ispisiRacune(glavaRacuna.sljedeci);

	//Unos podataka za upit
	printf("\nUnesite ime artikla: ");
	if (scanf("%s", clanak) != 1) return POGRESKA_SCANF;
	printf("Unesite pocetni datum (YYYY-MM-DD):  ");
	if (scanf("%s", pocetniDatum) != 1) return POGRESKA_SCANF;
	printf("Unesite zavrsni datum (YYYY-MM-DD):  ");
	if (scanf("%s", zavrsniDatum) != 1) return POGRESKA_SCANF;

	//Dohvati podatke o artiklu u razdoblju
	dohvatiClanakURazdoblju(&glavaRacuna, clanak, pocetniDatum, zavrsniDatum, &prihod, &kolicina);

	//Ispis ukupnog prihoda i kolicine
	printf("Prihod od %d artikla %s: %d\n", kolicina, clanak, prihod);

	//Ocisti memoriju
	obrisiRacune(&glavaRacuna);
	fclose(pokazivacNaDatoteku); //Zatvori datoteku

	return 0;
}

int dodajClanak(PozicijaClanka glava, PozicijaClanka clanak)
{
	PozicijaClanka trenutni = glava;

	while (trenutni->sljedeci != NULL && strcmp(trenutni->sljedeci->ime, clanak->ime) < 0) {
		trenutni = trenutni->sljedeci;
	}

	clanak->sljedeci = trenutni->sljedeci;
	trenutni->sljedeci = clanak;

	return USPJEH;
}

int dodajRacun(PozicijaRacuna glava, PozicijaRacuna racun)
{
	PozicijaRacuna trenutni = glava;

	while (trenutni->sljedeci != NULL && usporediDatume(trenutni->sljedeci->datum, racun->datum) < 0) {
		trenutni = trenutni->sljedeci;
	}

	racun->sljedeci = trenutni->sljedeci;
	trenutni->sljedeci = racun;
	return USPJEH;
}

int procitajRacun(char* imeDatoteke, PozicijaRacuna glavaRacuna)
{
	char datumString[20];
	FILE* pokazivacNaDatoteku = fopen(imeDatoteke, "r");

	if (pokazivacNaDatoteku == NULL) {
		printf("Datoteka se nije mogla otvoriti.\n");
		return POGRESKA_OTVARANJE_DATOTEKE;
	}

	PozicijaRacuna noviRacun = (PozicijaRacuna)malloc(sizeof(Racun));
	fgets(datumString, 20, pokazivacNaDatoteku);
	datumString[strcspn(datumString, "\n")] = 0; //Uklanjanje novog reda
	strcpy(noviRacun->datum, datumString);
	noviRacun->glavniClanak.sljedeci = NULL;
	noviRacun->sljedeci = NULL;

	PozicijaClanka noviClanak = NULL;

	noviClanak = (PozicijaClanka)malloc(sizeof(Clanak));
	while (fscanf(pokazivacNaDatoteku, "%49[^,], %d, %d\n", noviClanak->ime, &noviClanak->kolicina, &noviClanak->cijena) == 3) {
		dodajClanak(&noviRacun->glavniClanak, noviClanak);
		noviClanak = (PozicijaClanka)malloc(sizeof(Clanak)); // Alociranje za sledeæi artikal
	}
	free(noviClanak); // Osloboditi memoriju za posljednji neiskoristeni pokazivaè


	dodajRacun(glavaRacuna, noviRacun);
	fclose(pokazivacNaDatoteku);
	return USPJEH;
}

//Funkcija za ispis svih artikala
int ispisiClanke(PozicijaClanka trenutni)
{
	while (trenutni != NULL) {
		printf("\t%s, %d, %d\n", trenutni->ime, trenutni->kolicina, trenutni->cijena);
		trenutni = trenutni->sljedeci;
	}

	return USPJEH;
}

//Funkcija za ispis svih racuna
int ispisiRacune(PozicijaRacuna trenutni)
{
	while (trenutni != NULL) {
		printf("Racun od datuma %s:\n", trenutni->datum);
		ispisiClanke(trenutni->glavniClanak.sljedeci);
		trenutni = trenutni->sljedeci;
	}

	return USPJEH;
}

int dohvatiClanakURazdoblju(PozicijaRacuna trenutniRacun, char* clanak, char* pocetniDatum, char* zavrsniDatum, int* prihod, int* kolicina)
{
	while (trenutniRacun != NULL && usporediDatume(trenutniRacun->datum, pocetniDatum) < 0)
		trenutniRacun = trenutniRacun->sljedeci;

	while (trenutniRacun != NULL && usporediDatume(trenutniRacun->datum, zavrsniDatum) <= 0) {
		PozicijaClanka trenutniClanak = trenutniRacun->glavniClanak.sljedeci; // Ažuriraj za trenutni raèun

		while (trenutniClanak != NULL) {
			if (strcmp(trenutniClanak->ime, clanak) == 0) {
				*prihod += trenutniClanak->kolicina * trenutniClanak->cijena;
				*kolicina += trenutniClanak->kolicina;
			}
			trenutniClanak = trenutniClanak->sljedeci;
		}
		trenutniRacun = trenutniRacun->sljedeci;
	}

	return USPJEH;
}

int obrisiClanke(PozicijaClanka glavaClanka)
{
	PozicijaClanka trenutni, temp;
	trenutni = glavaClanka->sljedeci;

	while (trenutni != NULL) {
		temp = trenutni;
		trenutni = trenutni->sljedeci;
		free(temp);
	}

	glavaClanka->sljedeci = NULL;

	return USPJEH;
}

int obrisiRacune(PozicijaRacuna glavaRacuna)
{
	PozicijaRacuna trenutni, temp;
	trenutni = glavaRacuna->sljedeci;

	while (trenutni != NULL) {
		obrisiClanke(&trenutni->glavniClanak);
		temp = trenutni;
		trenutni = trenutni->sljedeci;
		free(temp);
	}

	glavaRacuna->sljedeci = NULL;
	
	return USPJEH;
}

int usporediDatume(char* datum1, char* datum2)
{
	return strcmp(datum1, datum2);
}